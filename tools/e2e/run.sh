#!/usr/bin/env bash
# Usage: tools/e2e/run.sh [scenario ...]   (default: every scenario in tools/e2e/scenarios)
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../.." && pwd)"
work="$root/build-e2e"
report="$work/report"

"$here/setup.sh"
headless="$here/.mgba/build/mgba-headless"

if [[ $# -gt 0 ]]; then
    scenarios=("$@")
else
    scenarios=()
    for path in "$here/scenarios/"*.lua; do
        scenarios+=("$(basename "$path" .lua)")
    done
fi

rm -rf "$report"
mkdir -p "$report"
failed=0
sections=""

for name in "${scenarios[@]}"; do
    scenario="$here/scenarios/$name.lua"
    flags="$(sed -n 's/^-- flags: //p' "$scenario" | head -n 1)"
    about="$(sed -n 's/^-- about: //p' "$scenario" | head -n 1)"
    rom="e2e-$(printf '%s' "$flags" | shasum | cut -c 1-10)"

    echo "Building $rom for $name"
    make -C "$root" -j 4 TARGET="$rom" BUILD="build-$rom" USERFLAGS="$flags" > "$work/$rom.log" 2>&1 ||
        { echo "Build failed, see $work/$rom.log"; exit 1; }

    out="$report/$name"
    mkdir -p "$out"
    echo "Running $name"

    if ! OUT="$out" E2E="$here" RECORD=1 timeout 60 "$headless" --script "$scenario" "$root/$rom.gba" > "$out/mgba.log" 2>&1; then
        echo "  FAILED: the emulator did not finish, see $out/mgba.log"
        failed=1
        sections+="<h2>$name: FAILED</h2><p>The emulator did not finish. See $name/mgba.log.</p>"
        continue
    fi

    ffmpeg -loglevel error -y -framerate 15 -pattern_type glob -i "$out/rec_*.png" \
        -vf "scale=480:320:flags=neighbor,split[a][b];[a]palettegen[p];[b][p]paletteuse" "$out/$name.gif"
    rm -f "$out/"rec_*.png

    figures=""
    for shot in "$out/"*.png; do
        figures+="<figure><img src=\"$name/$(basename "$shot")\"><figcaption>$(basename "$shot" .png)</figcaption></figure>"
    done

    sections+="<h2>$name</h2><p>$about</p><img class=\"gif\" src=\"$name/$name.gif\"><div class=\"grid\">$figures</div>"
done

cat > "$report/index.html" <<EOF
<!doctype html><meta charset="utf-8"><title>Ditto's Climb e2e</title>
<style>
:root{--bg:#f6f4ee;--fg:#1d1a24;--muted:#6b6577;--card:#fff;--line:#e2ddd2}
@media (prefers-color-scheme:dark){:root{--bg:#16141b;--fg:#ece8f2;--muted:#9a93a8;--card:#201d27;--line:#322d3b}}
body{margin:0;background:var(--bg);color:var(--fg);font:15px/1.5 system-ui,sans-serif}
main{max-width:1040px;margin:0 auto;padding:24px 16px 64px}
h2{font-size:17px;margin:32px 0 4px}p{margin:0 0 12px;color:var(--muted)}
img{image-rendering:pixelated;border-radius:4px;display:block;max-width:100%}
.gif{width:480px;margin-bottom:12px}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(230px,1fr));gap:12px}
figure{margin:0;background:var(--card);border:1px solid var(--line);border-radius:8px;padding:8px}
figcaption{font-size:12px;color:var(--muted);margin-top:6px}
</style><main><h1>Ditto's Climb e2e</h1>$sections</main>
EOF

echo "Report: $report/index.html"
exit $failed

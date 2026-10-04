#!/usr/bin/env bash
# Builds a headless mGBA with Lua scripting into tools/e2e/.mgba (gitignored).
set -euo pipefail

here="$(cd "$(dirname "$0")" && pwd)"
mgba="$here/.mgba"
commit=c3c8e5e813f245028de118a56734e1dc0f35ce2a

if [[ -x "$mgba/build/mgba-headless" ]]; then
    exit 0
fi

lua="$(brew --prefix lua@5.4)"

rm -rf "$mgba"
git init -q "$mgba"
git -C "$mgba" fetch -q --depth 1 https://github.com/mgba-emu/mgba.git "$commit"
git -C "$mgba" checkout -q FETCH_HEAD
git -C "$mgba" apply "$here/headless-video-buffer.patch"

cmake -S "$mgba" -B "$mgba/build" -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_HEADLESS=ON -DBUILD_QT=OFF -DBUILD_SDL=OFF -DBUILD_SHARED=OFF -DBUILD_STATIC=ON \
    -DUSE_LUA=5.4 -DLUA_INCLUDE_DIR="$lua/include/lua" -DLUA_LIBRARY="$lua/lib/liblua.dylib" \
    -DUSE_FFMPEG=OFF -DUSE_DISCORD_RPC=OFF -DUSE_EDITLINE=OFF -DUSE_ELF=OFF > "$mgba/cmake.log" 2>&1
cmake --build "$mgba/build" --target mgba-headless -j 8 > "$mgba/build.log" 2>&1

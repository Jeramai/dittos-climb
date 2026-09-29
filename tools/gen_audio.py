#!/usr/bin/env python3
"""Generates the original chiptune music (.mod) and sound effects (.wav) in audio/."""

import math
import random
import struct
import wave
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
AUDIO = ROOT / "audio"

PERIODS = [
    856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
    428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,
    214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,
]

SCALES = {
    "major": [0, 2, 4, 5, 7, 9, 11],
    "minor": [0, 2, 3, 5, 7, 8, 10],
    "dorian": [0, 2, 3, 5, 7, 9, 10],
    "phrygian": [0, 1, 3, 5, 7, 8, 10],
    "harmonic": [0, 2, 3, 5, 7, 8, 11],
    "lydian": [0, 2, 4, 6, 7, 9, 11],
    "mixolydian": [0, 2, 4, 5, 7, 9, 10],
}

LEAD_RHYTHMS = [
    [0, 4, 6, 8, 12],
    [0, 2, 4, 8, 10, 12, 14],
    [0, 6, 8, 12],
    [0, 3, 6, 8, 11, 12],
    [0, 4, 8, 10, 12, 14],
    [0, 8],
]

DRUMS = {
    "none": "................",
    "sparse": "k.......s.......",
    "basic": "k...h...s...h...",
    "driving": "k.h.s.h.k.hks.h.",
    "march": "k.k.s...k.k.s.ss",
    "rock": "k.h.s.h.kkh.s.hh",
    "shuffle": "k..h.hs..h.hk.h.",
}

# Sample slots (1-based in the module): lead, harmony, bass, kick, snare, hat, thin lead.
LEAD, HARMONY, BASS, KICK, SNARE, HAT, THIN = range(1, 8)
DRUM_SAMPLE = {"k": KICK, "s": SNARE, "h": HAT}

TRACKS = {
    "title": dict(bpm=112, root=0, scale="major", prog=[0, 5, 3, 4], bridge=[3, 4, 0, 5], bass="walk",
                  harmony="arp", drums="basic", lead=LEAD, seed=1),
    "lab": dict(bpm=120, root=2, scale="dorian", prog=[0, 3, 0, 4], bridge=[5, 3, 4, 4], bass="pulse",
                harmony="stab", drums="basic", lead=THIN, seed=2),
    "forest": dict(bpm=104, root=7, scale="major", prog=[0, 3, 4, 0], bridge=[5, 3, 1, 4], bass="walk",
                   harmony="arp", drums="shuffle", lead=LEAD, seed=3),
    "cave": dict(bpm=88, root=9, scale="minor", prog=[0, 5, 6, 4], bridge=[3, 0, 5, 4], bass="long",
                 harmony="none", drums="sparse", lead=THIN, seed=4),
    "lake": dict(bpm=96, root=5, scale="lydian", prog=[0, 1, 0, 4], bridge=[3, 1, 5, 4], bass="long",
                 harmony="arp", drums="sparse", lead=LEAD, seed=5),
    "plant": dict(bpm=132, root=4, scale="minor", prog=[0, 0, 5, 6], bridge=[3, 3, 4, 4], bass="pulse",
                  harmony="stab", drums="driving", lead=THIN, seed=6),
    "volcano": dict(bpm=126, root=4, scale="phrygian", prog=[0, 1, 0, 6], bridge=[5, 6, 1, 0], bass="pulse",
                    harmony="stab", drums="rock", lead=LEAD, seed=7),
    "ice": dict(bpm=100, root=11, scale="harmonic", prog=[0, 5, 3, 4], bridge=[5, 3, 1, 4], bass="long",
                harmony="arp", drums="sparse", lead=THIN, seed=8),
    "chasm": dict(bpm=116, root=2, scale="mixolydian", prog=[0, 6, 3, 0], bridge=[4, 3, 6, 4], bass="walk",
                  harmony="arp", drums="basic", lead=LEAD, seed=9),
    "hideout": dict(bpm=124, root=1, scale="minor", prog=[0, 6, 5, 4], bridge=[3, 4, 0, 4], bass="pulse",
                    harmony="stab", drums="driving", lead=THIN, seed=10),
    "dojo": dict(bpm=136, root=10, scale="dorian", prog=[0, 3, 0, 4], bridge=[5, 4, 3, 4], bass="pulse",
                 harmony="stab", drums="march", lead=LEAD, seed=11),
    "tower": dict(bpm=76, root=8, scale="harmonic", prog=[0, 5, 1, 4], bridge=[3, 1, 0, 4], bass="long",
                  harmony="arp", drums="none", lead=THIN, seed=12),
    "den": dict(bpm=108, root=3, scale="minor", prog=[0, 5, 2, 6], bridge=[3, 4, 5, 6], bass="walk",
                harmony="arp", drums="basic", lead=LEAD, seed=13),
    "peak": dict(bpm=92, root=6, scale="lydian", prog=[0, 1, 5, 4], bridge=[3, 1, 0, 1], bass="long",
                 harmony="arp", drums="sparse", lead=THIN, seed=14),
    "boss": dict(bpm=150, root=9, scale="harmonic", prog=[0, 5, 6, 4], bridge=[3, 0, 1, 4], bass="pulse",
                 harmony="stab", drums="rock", lead=LEAD, seed=15),
    "final_boss": dict(bpm=144, root=1, scale="phrygian", prog=[0, 1, 6, 0], bridge=[5, 6, 1, 4], bass="pulse",
                       harmony="arp", drums="driving", lead=LEAD, seed=16),
    "ending": dict(bpm=84, root=0, scale="major", prog=[0, 3, 5, 4], bridge=[3, 4, 0, 0], bass="long",
                   harmony="arp", drums="sparse", lead=LEAD, seed=17),
}


def chord(scale, degree):
    return [scale[(degree + step) % 7] + 12 * ((degree + step) // 7) for step in (0, 2, 4)]


def to_index(octave, semitone):
    index = (octave - 1) * 12 + semitone
    while index < 0:
        index += 12
    while index > 35:
        index -= 12
    return index


class pattern:
    def __init__(self):
        self.cells = [[None] * 4 for _ in range(64)]

    def note(self, row, channel, sample, index, effect=0, param=0):
        self.cells[row][channel] = (sample, PERIODS[index], effect, param)

    def effect(self, row, channel, effect, param):
        sample, period, _, _ = self.cells[row][channel] or (0, 0, 0, 0)
        self.cells[row][channel] = (sample, period, effect, param)

    def encode(self):
        data = bytearray()
        for row in self.cells:
            for cell in row:
                sample, period, effect, param = cell or (0, 0, 0, 0)
                data += bytes([(sample & 0xf0) | (period >> 8), period & 0xff, ((sample & 0x0f) << 4) | effect,
                               param])
        return data


def compose_section(spec, progression, rng, variation):
    scale = SCALES[spec["scale"]]
    root = spec["root"]
    section = pattern()
    lead_degree = 7
    rhythm_a = LEAD_RHYTHMS[rng.randrange(len(LEAD_RHYTHMS))]
    rhythm_b = LEAD_RHYTHMS[rng.randrange(len(LEAD_RHYTHMS))]

    for bar, degree in enumerate(progression):
        base = bar * 16
        tones = chord(scale, degree)
        rhythm = rhythm_a if bar % 2 == 0 else rhythm_b

        if bar == 3:
            rhythm = [0, 4, 8] if variation else [0, 8]

        for position in rhythm:
            if position in (0, 8):
                targets = [degree + step for step in (0, 2, 4, 7)]
                lead_degree = min(targets, key=lambda value: abs(value - lead_degree))
            else:
                lead_degree += rng.choice([-2, -1, -1, 1, 1, 2])
            lead_degree = max(2, min(lead_degree, 14))
            octave, step = divmod(lead_degree, 7)
            semitone = root + scale[step] + 12 * octave
            section.note(base + position, 0, spec["lead"], to_index(2, semitone))

        if rng.random() < 0.3 and bar != 3:
            section.effect(base + 15, 0, 0xc, 0)

        bass_root = root + tones[0]
        if spec["bass"] == "pulse":
            for position in range(0, 16, 2):
                semitone = bass_root + (7 if position in (6, 14) else 0)
                section.note(base + position, 2, BASS, to_index(1, semitone))
        elif spec["bass"] == "walk":
            for position, tone in zip((0, 4, 8, 12), tones + [tones[1]]):
                section.note(base + position, 2, BASS, to_index(1, root + tone))
        else:
            section.note(base, 2, BASS, to_index(1, bass_root))
            section.note(base + 8, 2, BASS, to_index(1, bass_root + 7))

        if spec["harmony"] == "arp":
            for position in range(0, 16, 2):
                tone = tones[(position // 2) % 3]
                section.note(base + position, 1, HARMONY, to_index(2, root + tone))
        elif spec["harmony"] == "stab":
            for position in (2, 6, 10, 14):
                section.note(base + position, 1, HARMONY, to_index(2, root + tones[1]))
                section.effect(base + position + 1, 1, 0xc, 0)

        for position, hit in enumerate(DRUMS[spec["drums"]]):
            if hit in DRUM_SAMPLE:
                section.note(base + position, 3, DRUM_SAMPLE[hit], 12)

    return section


def one_cycle(shape, length=32, amplitude=90):
    data = []
    for index in range(length):
        phase = index / length
        if shape == "pulse50":
            value = amplitude if phase < 0.5 else -amplitude
        elif shape == "pulse25":
            value = amplitude if phase < 0.25 else -amplitude
        elif shape == "pulse12":
            value = amplitude if phase < 0.125 else -amplitude
        else:
            value = int(amplitude * (4 * abs(phase - 0.5) - 1))
        data.append(value)
    return data


def drum(kind, rate=8287):
    rng = random.Random(kind)
    data = []
    if kind == "kick":
        phase = 0.0
        for index in range(1400):
            t = index / rate
            frequency = 150 * math.exp(-t * 18) + 40
            phase += frequency / rate
            data.append(int(110 * math.sin(2 * math.pi * phase) * math.exp(-t * 9)))
    elif kind == "snare":
        for index in range(1100):
            t = index / rate
            tone = math.sin(2 * math.pi * 190 * t) * 0.4
            data.append(int(100 * (rng.uniform(-1, 1) * 0.8 + tone) * math.exp(-t * 16)))
    else:
        for index in range(350):
            t = index / rate
            data.append(int(70 * rng.uniform(-1, 1) * math.exp(-t * 60)))
    if len(data) % 2:
        data.append(0)
    return data


def write_mod(name, spec):
    rng = random.Random(spec["seed"])
    sections = [compose_section(spec, spec["prog"], rng, False), compose_section(spec, spec["prog"], rng, True),
                compose_section(spec, spec["bridge"], rng, False)]
    sections[0].effect(0, 3, 0xf, spec["bpm"])
    order = [0, 1, 2, 1]

    samples = [
        ("lead", one_cycle("pulse50", amplitude=70), 40, True),
        ("harmony", one_cycle("pulse25", amplitude=60), 22, True),
        ("bass", one_cycle("triangle", amplitude=110), 52, True),
        ("kick", drum("kick"), 60, False),
        ("snare", drum("snare"), 44, False),
        ("hat", drum("hat"), 30, False),
        ("thin", one_cycle("pulse12", amplitude=70), 38, True),
    ]

    data = bytearray(name.encode()[:20].ljust(20, b"\0"))

    for index in range(31):
        if index < len(samples):
            sample_name, pcm, volume, loop = samples[index]
            words = len(pcm) // 2
            data += sample_name.encode().ljust(22, b"\0")
            data += struct.pack(">HBBHH", words, 0, volume, 0, words if loop else 1)
        else:
            data += bytes(22) + struct.pack(">HBBHH", 0, 0, 0, 0, 1)

    data += bytes([len(order), 127])
    data += bytes(order + [0] * (128 - len(order)))
    data += b"M.K."

    for section in sections:
        data += section.encode()

    for _, pcm, _, _ in samples:
        data += bytes((value + 256) % 256 for value in pcm)

    (AUDIO / f"{name}.mod").write_bytes(bytes(data))


RATE = 16384


def envelope(t, length, attack=0.005):
    if t < attack:
        return t / attack
    return max(0.0, 1 - t / length)


def square(frequency, t):
    return 1.0 if (frequency * t) % 1 < 0.5 else -1.0


def tone_sweep(start, end, length, shape=square, volume=0.6):
    samples = []
    phase = 0.0
    for index in range(int(RATE * length)):
        t = index / RATE
        frequency = start + (end - start) * (t / length)
        phase += frequency / RATE
        value = 1.0 if phase % 1 < 0.5 else -1.0
        if shape == "triangle":
            value = 4 * abs(phase % 1 - 0.5) - 1
        samples.append(value * volume * envelope(t, length))
    return samples


def notes(frequencies, step, volume=0.5, shape="square"):
    samples = []
    for frequency in frequencies:
        samples += tone_sweep(frequency, frequency, step, shape, volume)
    return samples


def noise(length, volume=0.7, decay=8.0, seed=0):
    rng = random.Random(seed)
    return [rng.uniform(-1, 1) * volume * math.exp(-decay * index / RATE) for index in range(int(RATE * length))]


def mix(*layers):
    length = max(len(layer) for layer in layers)
    return [max(-1.0, min(1.0, sum(layer[index] if index < len(layer) else 0 for layer in layers)))
            for index in range(length)]


def write_wav(name, samples):
    with wave.open(str(AUDIO / f"{name}.wav"), "wb") as output:
        output.setnchannels(1)
        output.setsampwidth(1)
        output.setframerate(RATE)
        output.writeframes(bytes(int((value + 1) * 127.5) for value in samples))


def write_effects():
    effects = {
        "sfx_shot": tone_sweep(1400, 900, 0.05, volume=0.25),
        "sfx_struggle": mix(noise(0.08, 0.35, 30, 1), tone_sweep(220, 120, 0.08, "triangle", 0.4)),
        "sfx_hit": mix(noise(0.07, 0.5, 40, 2), tone_sweep(500, 180, 0.07, volume=0.35)),
        "sfx_super": mix(notes([880, 1320], 0.05, 0.4), noise(0.05, 0.3, 50, 3)),
        "sfx_weak": tone_sweep(160, 90, 0.12, "triangle", 0.6),
        "sfx_hurt": tone_sweep(700, 180, 0.16, volume=0.45),
        "sfx_faint": tone_sweep(600, 80, 0.4, volume=0.4),
        "sfx_transform": mix(notes([392, 523, 659, 784, 1047, 1319], 0.05, 0.35), noise(0.3, 0.1, 6, 4)),
        "sfx_door_lock": mix(noise(0.12, 0.6, 20, 5), tone_sweep(120, 60, 0.15, "triangle", 0.6)),
        "sfx_door_open": notes([523, 784], 0.07, 0.4),
        "sfx_stairs": notes([392, 494, 587, 784, 988], 0.07, 0.4),
        "sfx_pickup": notes([988, 1319], 0.08, 0.4),
        "sfx_key_item": notes([523, 659, 784, 1047, 784, 1047], 0.09, 0.4),
        "sfx_boss": notes([440, 330, 440, 330, 440, 330], 0.1, 0.45),
        "sfx_explosion": mix(noise(0.6, 0.9, 5, 6), tone_sweep(90, 30, 0.5, "triangle", 0.5)),
        "sfx_shock": [square(60, index / RATE) * square(900, index / RATE) * 0.35 * envelope(index / RATE, 0.2)
                      for index in range(int(RATE * 0.2))],
        "sfx_fall": tone_sweep(1200, 150, 0.6, "triangle", 0.5),
        "sfx_warp": [math.sin(2 * math.pi * (500 + 300 * math.sin(40 * index / RATE)) * index / RATE) * 0.4 *
                     envelope(index / RATE, 0.3) for index in range(int(RATE * 0.3))],
        "sfx_menu": tone_sweep(1200, 1200, 0.03, volume=0.3),
        "sfx_heal": notes([523, 659, 784, 1047], 0.06, 0.35, "triangle"),
    }

    for name, samples in effects.items():
        write_wav(name, samples)


def main():
    AUDIO.mkdir(exist_ok=True)

    for name, spec in TRACKS.items():
        write_mod(name, spec)

    write_effects()


if __name__ == "__main__":
    main()

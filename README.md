# M01-Converter

This is a CLI application written in C++ that takes in a save file (.sav) from the Korg M01 application for the DS, and
outputs MIDI (.mid) files corresponding to each of the songs stored in the save file.

It has two modes of running:

1. Matching the MIDI export feature of the 3DS version of the app
    - Verified byte-for-byte identical to native 3DS exports, including full-length built-in demo songs.

2. An export with "extended" features, such as automatically setting Patch Changes, and many other CC messages like
   Volume and Pan.

Thank you to [f4mi](https://bsky.app/profile/f4mi.bsky.social) for the inspiration, go check
out [their video on the topic](https://www.youtube.com/watch?v=7ptN-3RT8yA)!

## Usage

Download the version for your platform from [releases](https://github.com/Mystievous/M01-Converter/releases).

Then, extract the archive and bring your .sav file to the folder with the executable.

You can run the tool with the following command:

```sh
# Windows
./M01-Converter.exe SaveFile.sav

# Linux/Mac
./M01-Converter SaveFile.sav
```

Or, if you want to use the extended export, make sure the config.yml is next to the executable and add `-e`:

```sh
# Windows
./M01-Converter.exe -e SaveFile.sav

# Linux/Mac
./M01-Converter -e SaveFile.sav
```

## Features

- All song slots with data saved to them are exported to individual MIDI files with the same name as the song.
    - i.e. "MY SONG" will be exported to "MY SONG.mid"
- Each MIDI file has 9 tracks. A "Meta" MIDI track with Song Name and Tempo, and each Instrument/Track in the save file
  exported as a separate MIDI track, with its own channel (see Extended mode for configurable channels).
    - i.e. in Standard mode, Instrument 1 is on MIDI Track 1, with Channel 0.
- All "Master" settings of tempo and swing are properly reflected to the MIDI file.
- Overrides of the tempo from the "set" menu are also exported to the MIDI file as Tempo Change events.
- The program reads all important information from the save file. However most of it is unused in the standard MIDI
  export such as mix settings, any instrument settings like attack/release, and any drumkit settings like volume,
  timestretch, and pan. The extended export does bake some of the information into its MIDI files, see the section
  below.
    - You can see the full information that is read in the [SaveStructure.h](include/M01Core/SaveStructure.h) file.
    - You can also find my original pattern script for [ImHex](https://github.com/WerWolv/ImHex) that I used to reverse
      engineer the save file format at [M01_SaveFile.hexpat](M01_SaveFile.hexpat), which can be used to inspect your own
      save file in detail. As far as I can tell, almost everything in the save file is defined in there. There are still
      fields marked "unknown", but only a few ever have data in them, which doesn't seem to be a functional result of
      song data you can change in the app.

### Extended Export

Extended export mode is accessed by passing the `--extended` or `-e` flag to the program when running it. This mode adds
the following features:

- Reading patch information from a config YAML file, which by default is `config.yml` in the same directory as the
  executable.
    - This can be specified to a custom path with `--config [path]` or `-c [path]`.
    - A sample config file is included in the `template` directory, [config.yml](template/config.yml).
        - I have mapped this one to the General MIDI patch list, based on what I felt was close enough to the original
          sounds from the ones included in the Microsoft GS Wavetable Synth.
- Each instrument's track is given a name of the Bank (i.e. "M1", "01/W", "EX"), and the Patch Name (i.e. "Piano1",
  "A.Bass", "Trumpet") as specified in the config file.
    - i.e. "EX - HiResoLead"
- Each instrument is given CC messages for Volume and Pan
    - It also sets Attack and Release based on the save file, on CC 73 and 72 respectively. However, I have not seen
      this used on any of my synths.

## Config File

The config file is a YAML file that contains a list of "remapping" settings for each instrument in the original DS
application.

You can specify the following fields for each instrument:

- `bankMsb` - The MIDI MSB bank id, 0-127
- `bankLsb` - The MIDI LSB bank id, 0-127
- `program` - The program/patch number of the instrument, 0-127
- `channel` - A specific MIDI channel to use for the instrument, 0-15
    - i.e. out of the 16 channels, Drums are on channel 10 for General MIDI; you can set this to 9 to use that channel.
- `transposition` - If you need to transpose the instrument by a certain number of semitones, you can specify this here.
    - i.e. if an "Organ" sound is an octave too low on your synth, you can set this to +12 to transpose the written
      notes up an octave.
- `map` - This allows for individual notes to be remapped to different MIDI notes. See more below.

### Note Remapping

The Drum tracks in the original application are not mapped to real MIDI standard drum notes, but instead sequentially
from 60 to 71.

For instance, while a kick drum on the General MIDI standard is on note 36, it would read as note 60 from the DS
application. This behavior is present in MIDI exports from the 3DS version of the app.

To remap Drum notes to their proper MIDI note numbers, you can specify a `map` field in the config file. This is a list
of 12 note ids to remap to, corresponding to the 12 samples available in each drum track. The drum map *must* contain
all 12 entries.

For instance, the following config file would set the mapping for "M1 - DrumKit1" to the General MIDI standard:

```yaml
"Instruments":
  "M1":
    "Drum Kit":
      "DrumKit1":
        channel: 9
        map:
          - 51 # Ride -> Ride Cymbal 1
          - 57 # Crash -> Crash Cymbal 2
          - 56 # Cowbel -> Cowbell
          - 50 # TomH -> High Tom
          - 47 # TomM -> Low Mid Tom
          - 41 # TomL -> Low Floor Tom
          - 46 # HH-Op -> Open High Hat
          - 42 # HH-Cl -> Closed High Hat
          - 40 # Snare2 -> Electric Snare
          - 38 # Snare1 -> Acoustic Snare
          - 35 # Kick2 -> Acoustic Bass Drum
          - 36 # Kick1 -> Bass Drum 1
```

Notes can also be remapped by specifying specific note numbers:

```yaml
"Instruments":
  "EX":
    "Brass/Reed":
      "Trumpet":
        map:
          60: 62 # C4 -> D4
          72: 44 # C5 -> Ab2
```

## Future Plans

- The ability to write external MIDI files back into a M01 save file, or generate a save file from scratch.
    - This would require finding the "best match" for certain notes that don't line up with the game's step grid.
        - For instance, note starts must be on a 16th, but note ends can fall on an additional quarter subdivision.
        - Another example would be finding the closest swing value to the given midi file, which would be even more
          difficult.
- Building this tool into a standalone DS homebrew app that can read the save file directly from the DS, allowing for
  only the resulting MIDI files themselves to need exporting to a computer.
- Add support for 3DS save files in the tool, and for all the above plans.

## Nitty Gritty 3DS Parity Stuff

- Ticks Per Quarter note is set to 480
- Each "step" is one 16th note, so a "16 step" sequence is 4 beats.
- Swing:
    - The swing setting is a *16th note swing* i.e. every other step is swung.
    - Swing splits steps into pairs, with the first being stretched and the second being squeezed.
        - The length of the pair is always the same as two straight steps, it's just the midpoint that gets adjusted
          with swing.
        - The stretched step length is `round(120 * swing / 50)`, the squeezed one is the remainder within the length of
          the pair.
        - Substeps within a step (for note ends) are interpolated along the rounded length, and truncated.
            - In my first version, I was scaling the raw offset by `swing / 50` and rounding, which was close enough
              that I couldn't figure out why it wasn't exactly right. I had to use the rounded step length and truncate,
              which seems to be what the 3DS does.
- Note lengths have a constant 10 tick padding at the end of each note
    - i.e. when swing is 50%, a note that is 1 step long will be 110 ticks long, with 10 ticks of rest before the start
      of the next step.
- Note data can be present outside a pattern's "visible" range, i.e. if the pattern is shortened after writing notes to
  it, or by copying a range of notes extending past the end of the pattern.
    - This excess note data must be trimmed to only notes within the measure/pattern's "step size" from `MeasureInfo`
- When a song has bars with an odd number of steps, the bars following may be misaligned with the "true" timeline/grid.
- I modified the [midifile](https://github.com/craigsapp/midifile) library I'm using to use MIDI Running Status the same
  way as the 3DS export, as well as using integer math to calculate the tempo without rounding.
- midifile sorts each track with `qsort`, which is not consistent, so events on the same tick had arbitrary order. To
  match the 3DS export order, we use `markSequence()` before calling `sortTracks()`, which makes it use insertion order
  when sorting. This matches the 3DS export, and also ensures bank selects fire before their program changes.

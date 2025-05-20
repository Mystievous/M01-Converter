# M01-Converter

This is a CLI application written in C++ that takes in a save file (.sav) from the Korg M01D application for the DS,
and outputs MIDI (.mid) files corresponding to each of the songs stored in the save file.

It has two modes of running:

1. An (almost) exactly match to the MIDI export feature of the 3DS version of the app
    - It is not *exact* byte-for-byte identical, but it only differs in slight rounding differences and the order of
      messages that occur simultaneously (i.e. the order of the data for notes that make up a chord on the same beat).
2. An export with "extended" features, such as automatically setting Patch Changes, and many other CC messages like
   Volume and Pan.

Thank you to [f4mi](https://bsky.app/profile/f4mi.bsky.social) for the inspiration, go check
out [their video on the topic](https://www.youtube.com/watch?v=7ptN-3RT8yA)!

## Features

- All song slots with data saved to them are exported to individual MIDI files with the same name as the song.
    - i.e. "MY SONG" will be exported to "MY SONG.mid"
- Each MIDI file has 9 tracks. A "Meta" MIDI track with Song Name and Tempo, and each Instrument/Track in the save file
  exported as a separate MIDI track, with its own channel.
    - i.e. Instrument 1 is on MIDI Track 1, and Channel 0.
- All "Master" settings of tempo and swing are properly reflected to the MIDI file.
- Overrides of the tempo from the "set" menu are also exported to the MIDI file as Tempo Change events.
- The program reads almost all of the important information from the save file. However most of it is unused in the MIDI
  export such as mix settings, any instrument settings like attack/release, and any drumkit settings like volume,
  timestretch, and pan.
    - You can see the full information that is read in the [SaveStructure.h](include/SaveStructure.h) file.
    - You can also find my original pattern script for [ImHex](https://github.com/WerWolv/ImHex) that I used to reverse
      engineer the save file format at [SaveFile.hexpat](SaveFile.hexpat), which you can be used to inspect your own
      save file in detail. There are still some parts of the format that are left unparsed, but they are not relevant to
      the MIDI export.

### Extended Export

Extended export mode is accessed by passing the `--extended` or `-e` flag to the program when running it.
This mode adds the following features:

- Reading patch information from a config YAML file, which by default is `config.yaml` in the same directory as the
  executable.
    - This can be specified to a custom path with `--config [path]` or `-c [path]`.
    - A sample config file is included in the `template` directory, [config.yml](template/config.yml).
        - I have mapped this one to the General MIDI patch list, based on what I felt was close enough to the original
          sounds from the ones included in the Microsoft GS Wavetable Synth.
- Each instruments' track is given a name of the Bank (i.e. "M1", "01/W", "EX"), and the Patch Name (i.e. "Piano1",
  "A.Bass", "Trumpet") as specified in the config file.
    - i.e. "EX - HiResoLead"
- Each instrument is given CC messages for Volume and Pan
    - It also sets Attack and Release based on the save file, on CC 73 and 72 respectively. However, I have not seen
      this used on any of my synths.

## Config File

The config file is a YAML file that contains a list of "remapping" settings for each instrument in the original DS
application.

You can specify the following fields for each instrument:

- `bank` - The bank number, 0-127
- `subBank` - The sub-bank/"fine" bank number, 0-127
- `program` - The program/patch number of the instrument, 0-127
- `channel` - A specific MIDI channel to use for the instrument, 0-15
    - i.e. out of the 16 channels, Drums are on channel 10 for General MIDI; you can set this to 9 to use that channel.
- `transposition` - If you need to transpose the instrument by a certain number of semitones, you can specify this here.
    - i.e. if an "Organ" sound is an octave too low on your synth, you can set this to +12 to transpose the written
      notes up an octave.
- `map` - This allows for individual notes to be remapped to different MIDI notes. See more below.

### Note Remapping

The Drum tracks in the original application are not mapped to real MIDI drum notes, but instead sequentially from
60 to 71.

For instance, while a kick drum on the General MIDI standard is on note 36, it would read as note 60 from the DS
application.
This behavior is even present in MIDI exports from the 3DS version of the app.

To remap Drum notes to their proper MIDI note numbers, you can specify a `map` field in the config file.
This is a list of 12 note ids to remap to, corresponding to the 12 samples available in each drum track.
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

- Try to build this into a standalone DS homebrew app that can read the save file directly from the DS, allowing for
  only the MIDI files themselves to be needed to be moved to a computer.
- Maybe add support for 3DS save files as well, given the extra features of the "extended" export mode.

## Nitty Gritty 3DS Parity Stuff

- Ticks Per Quarter note is set to 480
- Each "step" is one 16th note, so a "16 step" sequence is 4 beats.
- The swing setting is a *16th note swing*
    - i.e. every other "step" is swung.
- Note lengths have a constant 10 tick padding at the end of each note
    - i.e. when swing is 50%, a note that is 1 step long will be 110 ticks long, with 10 ticks of rest before the start
      of the next step.
- When a song has bars with an odd number of steps, the bars following may be misaligned with the "true" timeline/grid.
- I modified the [midifile](https://github.com/craigsapp/midifile) library I'm using to use MIDI Running Status the same
  way as the 3DS export, as well as using integer math to calculate the tempo without rounding.
# midifile

Original Repo at https://github.com/craigsapp/midifile

### Modifications for parity with 3DS

- [MidiFile.cpp](MidiFile.cpp) - `MidiFile::write()` modified to use MIDI Running Status.
- [MidiMessage.cpp](MidiMessage.cpp) - `MidiMessage::setMetaTempo()` modified to use Integer math to calculate tempo.
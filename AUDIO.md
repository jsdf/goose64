
# exporting sound and music with the windows sound tools
- use windows 98. DSE.EXE crashes and gets 'out of memory' errors on windows xp or later
- if you do an action which tries to play a sound it will show an error dialog because the hardware isn't connected. just keep clicking 'cancel'

## to export sound effects
- convert audio to AIFF (big endian), 32000hz
- open and save all AIFF files in DSE.EXE to make sure they are formatted in a way that DREAM.EXE will understand
- use the sample bank editor to import all the files
- click 'compile bank' (produces .wbk and .ptr). the first time this will ask you to specify a file path but subsequently will just update the existing file

## to export the .bfx (binary sound effects) file
- edit > sound effects editor
- 'auto create', add all files
- 'load/save' > binary n64 sound effects (produces .bfx)

### to create and export music sequences (.bin file)
- export standard midi file from your DAW
- in n64 sound tools, file -> import midi. will probably fail to import it
- if it failed to import, use this tool to recreate a stripped-down version of the midi file (choose file, re-run cells, then click 'download' at the bottom): https://observablehq.com/@jsdf/parsing-midi-files-with-tonejs-midi
- load your sample bank file (shared with sound effects)
- add any instrument samples you need
- open the sequence parts, select all notes, assign a waveform to play for the note
- save the song file. the same filename and location will be used by the song binary file export
- export the binary song. it will have the same name as the song file, with a .bin extension. this is the file used by the n64 code
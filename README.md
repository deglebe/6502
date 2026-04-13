# 6502

6502 emulator based on the structure of professor gormanly's emulator for
cmpt422 organization and architecture

written in c99, uses tcc by default for compilation

## DONE

- [x] cpu init
- [x] memory init
- [x] clock listener/observer wiring
- [x] pulse logging for cpu and memory
- [x] hardware hex formatter `hexLog` with caller-provided buffer
- [x] memory registers + bus model
    - [x] mar (16 bit) and mdr (8 bit)
    - [x] `memory_reset` and full ram clear
    - [x] addressable-space logging
- [x] mmu
    - [x] cpu/memory-aware mmu object
    - [x] full mar srt and low/high byte bitmask updating
    - [x] little-endian mar load from two bytes
    - [x] read/write through mmu
    - [x] `writeImmediate` equivalent from lab assignment
- [x] loads static startup program
- [x] pulse-driven memory
    - [x] memory read/write requests execute on clock pulses
    - [x] mmu exposes cycle/readiness helpers

## TODO

- [ ] instructionset

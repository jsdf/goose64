# n64 sdk notes

## exeGCC compiler

the n64 sdk includes an old fork of the GCC compiler called [exeGCC](https://ultra64.ca/files/documentation/nintendo/Nintendo_64_EXEGCC_Compiler_User_Guide_NUS-06-0160-001A.pdf). this is provided as a set of commands [listed here](https://n64squid.com/homebrew/n64-sdk/software/mipse-ultra-gcc/). it also includes a program called 'exew32' which must be used to execute these binaries, passing the command as args to exew32.

### c language limitations
the exegcc compiler that comes with the n64 sdk only supports roughly the c89 standard of features, which means all variables have to be declared at the top of a function (no inline declaration in for-loops, for example). you can use blocks to define more variables later in a function.

## memory alignment requirements

the n64 rcp requires memory addresses passed to it to be 8 byte aligned or the rcp will hang. this typically manifests as the graphics freezing while the main cpu program continues to run (so you might still see logging output and even hear sound). exeGCC will handle enforching this alignment this automatically in most cases:

> With N64, arrays specified for DMA to the configured hardware must be aligned to 8 bytes or 16
bytes. In this case, alignment can be clearly specified for the variables In the regions where exeGCC performs DMA. The following syntax specifies a 16-byte boundary for the dram_stack
variable.
> `u64 dram_stack[SP_DRAM_STACK_SIZE64] __attribute__((aligned (16)));`
> However, this type of specification can result In hard-to-understand errors, not only because many
definitions are omitted, but also because operation errors can arise after revisions are made to
completely different functions and variables.
> For this reason, with exeGCC(N64), the environment variable n64align=on can be set so that non initialized variables larger than 16 bytes align with the 8 byte boundary, and variables larger than
1024 bytes align with lhe 16 byte boundary. This function basically eliminates the need to clearly
specify the alignment as in the previous example.

the n64align=on environment variable is set by default when using the n64 sdk's build scripts (setupgcc.bat), enabling this behavior. however, if there is a value smaller than 8 bytes which will be referenced by the rcp, care needs to be taken to create this alignment. when defining a static memory allocation (e.g. a variable), the gcc attribute mentioned above can also be used to achieve this alignment, however this is not generally needed due to the n64align behavior. a more subtle issue is when alignment is affected not just by variable definition but also by data layout within the variable, e.g. in a struct or array of structs. types defined by gbi.h (e.g. Mtx, Vtx) generally include a padding field to ensure they are at least 8 bytes long, but if you embed them in other structs it is still possible for their offset within that struct to not be 8 byte aligned. an alternative is to separately define your allocations of this shared-memory data from data which is only used by the main cpu. this may also be desirable for cache efficiency.

## data structures shared between main cpu and rcp via dma
to tell the graphics microcode where to render objects we need to allocate Mtx (matrix) structures containing the transform (position, rotation, scaling) of the object which will be used by the graphics microcode. the c code you write fills out these datastructures and sends (via displaylist commands) a memory address pointing to them (translated into a non-virtual, absolute hardware address) to the rcp so the microcode running on the rcp can read it when needed. the data is not copied, but directly read by the other processor. as a result, each structure which is used in a displaylist command for particular frame of rendering needs to persist in memory until that frame is done being processed by the rcp, even when your main cpu program no longer needs it.

you can think of these allocations as being part of an async/parallel processing task queue between the two processors. as you typically overlap rcp and cpu processing of subsquent frames, if you reuse the same structures in memory across multiple frames this can create concurrency errors in cases where the rcp gets more than one frame behind, as the structures may be updated by the main cpu program concurrently with the rcp reading them while processing the previous frame. if the memory locations used are static and stable per object then this may be subtle (e.g. reading a transform matrix intended for the same object but in the next frame). if the allocations are more dynamic, the effect will be more substantial (graphical corruption or hanging the rcp).

## running the n64 sdk on mac using crossover

as macOS removed support for 32-bit executables, open source wine (e.g. WineStable) currently isn't able to run the sdk binaries (e.g. the gcc compiler). crossover implements integration with macOS to run 32-bit windows binaries however.

first, make sure your 'wine' command points to the copy of wine included with crossover, e.g.

```
/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/CrossOver-Hosted Application/wine
```

then start Crossover.app and launch some program (e.g. cmd.exe). if you don't, the subsequent commands will still work, but will take an extra 2 seconds (seems like Crossover has some preloading mechanism as long as some app is running?).

run `./build.sh` to run the compiler. internally this uses wine to run make/gcc.

# ft_malloc

If you want the lsp to function properly update the `directory` parameter inside the  `/home/henri/Projects/ft_malloc/compile_commands.json` to the absolute path of the root directory of the project on your machine.

# Description

**Zones (t_zone)**: Manages large memory regions via mmap. Includes logic to munmap entire zones when they become empty.

**Blocks (t_block)**: Inline headers that track allocation size and state.

**Intrusive Free List**: When a block is freed, the empty payload area is used to store navigation pointers without increasing metadata overhead.

# Errors

The following errors are being handled.

#### mmap error

If the system call function mmap fails, which usually happens when there is no physical memory left, malloc and realloc return null.

#### heap metadata corruption

Heap metadata is the "header" information stored just before an allocated memory block that allows malloc and free to function correctly. When this metadata becomes inconsistent, it is typically because the user wrote data past the end of an allocated buffer, accidentally overwriting the management data of the next block.
If this heap metadata corruption error occurs a SIGABRT signal is sent which will return the exit code 134.
The following message will be output to stdout just like malloc does:
`malloc(): corrupted top size`

# Notes

High addresses
┌─────────────────────┐
│      Stack          │  ← grows downward
│         ↓           │
│                     │
│    (unmapped gap)   │
│                     │
│         ↑           │
│      Heap           │  ← grows upward
├─────────────────────┤
│      BSS            │  (uninitialised globals)
├─────────────────────┤
│      Data           │  (initialised globals)
├─────────────────────┤
│      Text (rodata)  │  (your code)
└─────────────────────┘
Low addresses

BSS, Data and Text have a fixed size and can not be deallocated
if a global variable is known at compile time and immutable then it will live in .rodata rather than .data.

`
constexpr int x = 42;   // known + immutable              → .rodata
int y = 42;             // known + mutable                → .data
int z;                  // known (implicitly 0) + mutable → .bss
int w = get_value();    // unknown + mutable              → stack or heap
`

On POSIX systems mmap return MAP_FAILED on error which is defined as (void*)-1:
  (void*)-1 => this operation returns a pointer address that is not existent in memory and is used to indicate that there is an error, it is used in cases where using NULL would not be a valid way to signify that an error has occured.

virtual memory maps pages to physical memory.
mmap returns a pointer to the start of a memory page.

# Links

https://circuitlabs.net/advanced-file-i-o-memory-mapped-files-mmap-munmap/
https://danluu.com/malloc-tutorial/
https://www.iditect.com/program-example/c--implementing-your-own-mallocfree-with-mmap-and-munmap.html
https://medium.com/a-42-journey/how-to-create-your-own-malloc-library-b86fedd39b96
https://levelup.gitconnected.com/malloc-from-scratch-dbc1bc23dfde
https://course.ccs.neu.edu/cs3650sp23/a07.html
https://cs.wellesley.edu/~cs240/f23/slides/malloc.pdf

og: https://www.yumpu.com/en/document/read/5857476/a-malloc-tutorial/7

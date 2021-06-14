# echidnaFS Specification

**Blocks start with block 0, and are addressed as quad-words.**

The file system should be addressable using 32-bit addressing too, in a "compatibility mode".

Of course, when addressing using 32-bit values the maximum number of blocks will be 2^32, same thing for file sizes and other values.

## block#0 to block#15

Reserved blocks. Always. A bigger block size will result in more reserved space.

**block#0** contains the **identity table** as follows:
```x86asm
times 4 db     ; reserved for jump instruction, or other purposes
db '_ECH_FS_'  ; echidnaFS signature
qword          ; total block count
qword          ; length of the main directory in blocks (usually 5% of the total blocks)
qword          ; bytes per block (MUST be a multiple of 512)
dword          ; reserved
qword[2]       ; UUID of partition
```

## block#16: Allocation table
Contains a chain of qwords. One per physical block, hence the size of the table (in blocks) can be calculated as: `(total_blocks * sizeof(uint64_t) + block_size - 1) / block_size`

Possible values for an entry:
* `0x00000000_00000000`: "Free-for-use" block.
* `0x00000000_00000001 -> 0xFFFFFFFF_FFFFFFEF`: Mark the next block in the chain.
* `0xFFFFFFFF_FFFFFFF0`: "Reserved block" (the **first 16 blocks** are always marked as **reserved**, along with the directory and allocation table blocks).
* `0xFFFFFFFF_FFFFFFFF`: "End-of-chain".

## Main directory

The main directory starts from the first block **after** the end of the **allocation table**. Its **length** is specified in the **ID table in block#0**.\
Contains entries representing files, directories, or other objects.

Directory ID values:
* `0x00000000_00000000`: "End-of-directory".
* `0x00000000_00000001 -> 0xFFFFFFFF_FFFFFFFD`: Normal directory entry.
* `0xFFFFFFFF_FFFFFFFE`: Deleted entry.
* `0xFFFFFFFF_FFFFFFFF`: Root directory.

Directory entry structure:
```x86asm
qword         ; Directory ID of the entry (see above)
byte          ; Object type (0=file, 1=directory, rest to be implemented)
times 201 db  ; Zero-terminated ascii string containing the name of the object. max. 200 chars + null byte.
qword         ; 64-bit unix atime
qword         ; 64-bit unix mtime
word          ; Permissions (-------rwxrwxrwx)
word          ; Owner ID
word          ; Group ID
qword         ; 64-bit unix ctime
qword         ; Starting block (if file), directory ID (if directory)
qword         ; File size in bytes (0 if directory, shouldn't matter anyways)
```

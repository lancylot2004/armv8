///
/// const.h
/// Constants for the rest of the program.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef COMMON_CONST_H
#define COMMON_CONST_H

/// The virtual memory size of the emulated machine.
#define MEMORY_SIZE (2 << 20) // 2MB

/// The number of general purpose registers in the virtual machine.
#define NUM_GPRS 31

/// Alias for a chunk of data passed to and from the virtual registers or memory.
typedef uint64_t BitData;

/// Alias for an instruction as a binary word.
typedef uint32_t BitInst;

#endif //COMMON_CONST_H

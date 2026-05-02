#ifndef WINOS_BOOT_CONTRACT_H
#define WINOS_BOOT_CONTRACT_H

#include "boot_params.h"

/*
 * Boot-to-kernel ABI contract:
 * - Kernel entry symbol: kernel_entry64
 * - First argument register in x86_64 long mode: RDI -> BOOT_PARAMS*
 * - Stack: 16-byte aligned before C call
 */

typedef void (*WINOS_KERNEL_ENTRY)(BOOT_PARAMS* params);

#endif

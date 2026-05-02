#include "../../include/boot_params.h"

void arch_pmm_init(const win_boot_params_t *boot) {
    (void)boot;
    // TODO: initialize buddy allocator from UEFI memory map
}

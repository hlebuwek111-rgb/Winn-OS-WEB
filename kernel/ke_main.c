#include "include/ke_init.h"
#include "include/ke_types.h"

void arch_gdt_init(void);
void arch_idt_init(void);
void arch_pmm_init(const win_boot_params_t *boot);
void arch_vmm_init(void);
void arch_sched_init(void);

volatile const char *g_boot_banner = "Winn OS kernel initialized";

void ke_main(const win_boot_params_t *boot) {
    (void)boot;
    arch_gdt_init();
    arch_idt_init();
    arch_pmm_init(boot);
    arch_vmm_init();
    arch_sched_init();

    for (;;) {
        __asm__ volatile ("hlt");
    }
}

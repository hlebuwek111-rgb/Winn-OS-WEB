#include <stdint.h>

#include "../include/boot/boot_params.h"

static volatile uint16_t* const COM1 = (volatile uint16_t*)0x3F8;

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_init(void) {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

static void serial_putc(char c) {
    while ((inb(0x3F8 + 5) & 0x20) == 0) {}
    outb(0x3F8, (uint8_t)c);
}

static void serial_puts(const char* s) {
    while (*s) serial_putc(*s++);
}

void kernel_main(BOOT_PARAMS* params) {
    (void)COM1;
    serial_init();
    serial_puts("[WINOS] kernel_main reached\r\n");
    serial_puts("[WINOS] milestone: serial online\r\n");

    if (params) {
        serial_puts("[WINOS] boot params received\r\n");
    } else {
        serial_puts("[WINOS] boot params missing\r\n");
    }

    serial_puts("[WINOS] milestone: boot chain increment #1 complete\r\n");

    for (;;) {
        __asm__ volatile("hlt");
    }
}

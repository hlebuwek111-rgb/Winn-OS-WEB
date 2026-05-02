#ifndef WINOS_BOOT_PARAMS_H
#define WINOS_BOOT_PARAMS_H

#include <stdint.h>

#define BOOT_PARAMS_VERSION 1u
#define BOOT_MAX_MEM_DESC   256u

typedef struct BOOT_MEMORY_DESCRIPTOR {
    uint32_t type;
    uint32_t reserved;
    uint64_t physical_start;
    uint64_t virtual_start;
    uint64_t number_of_pages;
    uint64_t attribute;
} BOOT_MEMORY_DESCRIPTOR;

typedef struct BOOT_MEMORY_MAP {
    uint64_t descriptor_size;
    uint64_t descriptor_version;
    uint64_t descriptor_count;
    BOOT_MEMORY_DESCRIPTOR descriptors[BOOT_MAX_MEM_DESC];
} BOOT_MEMORY_MAP;

typedef struct BOOT_FRAMEBUFFER_INFO {
    uint64_t base;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
    uint32_t pixel_format;
} BOOT_FRAMEBUFFER_INFO;

typedef struct BOOT_ACPI_INFO {
    uint64_t rsdp_physical_address;
} BOOT_ACPI_INFO;

typedef struct BOOT_SMBIOS_INFO {
    uint64_t smbios_entry_point;
} BOOT_SMBIOS_INFO;

typedef struct BOOT_PARAMS {
    uint32_t version;
    uint32_t flags;
    BOOT_MEMORY_MAP memory_map;
    BOOT_FRAMEBUFFER_INFO framebuffer;
    BOOT_ACPI_INFO acpi;
    BOOT_SMBIOS_INFO smbios;
    uint64_t kernel_image_base;
    uint64_t kernel_entry;
} BOOT_PARAMS;

#endif

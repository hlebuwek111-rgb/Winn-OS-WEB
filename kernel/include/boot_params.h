#ifndef BOOT_PARAMS_H
#define BOOT_PARAMS_H

#include "ke_types.h"

typedef struct {
    u64 physical_base;
    u64 virtual_base;
    u64 page_count;
    u32 type;
    u32 reserved;
} win_mem_desc_t;

typedef struct {
    u64 width;
    u64 height;
    u64 pixels_per_scanline;
    u32 *framebuffer;
} win_fb_info_t;

typedef struct {
    u64 rsdp;
    u64 smbios;
    u64 mem_desc_count;
    win_mem_desc_t *mem_map;
    win_fb_info_t fb;
} win_boot_params_t;

#endif

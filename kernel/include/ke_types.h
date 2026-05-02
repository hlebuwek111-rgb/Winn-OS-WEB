#ifndef KE_TYPES_H
#define KE_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;

typedef enum {
    KE_OK = 0,
    KE_ERR_GENERIC = -1,
    KE_ERR_UNSUPPORTED = -2,
} ke_status_t;

#endif

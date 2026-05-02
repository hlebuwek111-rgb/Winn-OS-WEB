#ifndef WFS_H
#define WFS_H

#include "ke_types.h"

#define WFS_MAGIC 0x57465331534F4E57ULL /* WFS1SONW */
#define WFS_CLUSTER_SIZE 4096
#define WFS_MFT_RECORD_SIZE 1024

typedef struct {
    u64 magic;
    u64 partition_size_bytes;
    u32 cluster_size;
    u32 mft_record_size;
    u64 mft_start_lcn;
    u64 journal_start_lcn;
    u64 bitmap_start_lcn;
    u64 transaction_id;
} wfs_superblock_t;

typedef struct {
    u32 type;
    u16 length;
    u16 flags;
} wfs_attr_hdr_t;

typedef struct {
    u32 magic; /* 'FILE' */
    u16 sequence;
    u16 link_count;
    u16 attr_offset;
    u16 flags;
    u64 frn;
} wfs_mft_header_t;

#endif

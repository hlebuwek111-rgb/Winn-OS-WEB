#ifndef WFS_ONDISK_H
#define WFS_ONDISK_H

#include <stdint.h>

#define WFS_SUPERBLOCK_MAGIC 0x53465757u /* "WWFS" little-endian */
#define WFS_SUPERBLOCK_VERSION 1u
#define WFS_MIN_CLUSTER_SIZE 512u
#define WFS_MAX_CLUSTER_SIZE (2u * 1024u * 1024u)

#define WFS_PACKED __attribute__((packed))

enum wfs_system_file_record {
    WFS_SYS_MFT = 0,
    WFS_SYS_MFT_MIRROR = 1,
    WFS_SYS_LOG = 2,
    WFS_SYS_BITMAP = 3,
    WFS_SYS_ROOT_DIR = 4,
    WFS_SYS_BAD_CLUSTERS = 5,
    WFS_SYS_UPCASE = 6,
    WFS_SYS_MAX
};

enum wfs_attr_type {
    WFS_ATTR_STANDARD_INFORMATION = 0x10,
    WFS_ATTR_ATTRIBUTE_LIST = 0x20,
    WFS_ATTR_FILE_NAME = 0x30,
    WFS_ATTR_OBJECT_ID = 0x40,
    WFS_ATTR_SECURITY_DESCRIPTOR = 0x50,
    WFS_ATTR_VOLUME_NAME = 0x60,
    WFS_ATTR_VOLUME_INFORMATION = 0x70,
    WFS_ATTR_DATA = 0x80,
    WFS_ATTR_INDEX_ROOT = 0x90,
    WFS_ATTR_INDEX_ALLOCATION = 0xA0,
    WFS_ATTR_BITMAP = 0xB0,
    WFS_ATTR_REPARSE_POINT = 0xC0,
    WFS_ATTR_END = 0xFFFFFFFFu
};

struct wfs_extent_desc {
    uint64_t lcn;
    uint64_t cluster_count;
} WFS_PACKED;

struct wfs_superblock {
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    uint32_t bytes_per_sector;
    uint32_t sectors_per_cluster;
    uint64_t total_sectors;
    uint64_t mft_lcn;
    uint64_t mftmirr_lcn;
    uint64_t journal_lcn;
    uint64_t bitmap_lcn;
    uint32_t mft_record_size;
    uint32_t index_record_size;
    uint8_t volume_uuid[16];
    uint8_t reserved[400];
    uint32_t header_crc32;
} WFS_PACKED;

struct wfs_mft_record_header {
    uint32_t signature; /* "FILE" */
    uint16_t fixup_offset;
    uint16_t fixup_count;
    uint64_t lsn;
    uint16_t sequence_number;
    uint16_t hard_link_count;
    uint16_t first_attr_offset;
    uint16_t flags;
    uint32_t bytes_in_use;
    uint32_t bytes_allocated;
    uint64_t base_record_ref;
    uint16_t next_attr_id;
    uint16_t align;
    uint32_t record_number;
} WFS_PACKED;

struct wfs_attr_header {
    uint32_t type;
    uint32_t length;
    uint8_t non_resident;
    uint8_t name_length;
    uint16_t name_offset;
    uint16_t flags;
    uint16_t attr_id;
} WFS_PACKED;

struct wfs_attr_resident {
    struct wfs_attr_header common;
    uint32_t value_length;
    uint16_t value_offset;
    uint8_t indexed_flag;
    uint8_t reserved;
} WFS_PACKED;

struct wfs_attr_nonresident {
    struct wfs_attr_header common;
    uint64_t lowest_vcn;
    uint64_t highest_vcn;
    uint16_t runlist_offset;
    uint16_t compression_unit;
    uint32_t padding;
    uint64_t allocated_size;
    uint64_t data_size;
    uint64_t initialized_size;
} WFS_PACKED;

#endif

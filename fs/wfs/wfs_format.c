#include "../../kernel/include/wfs.h"

const wfs_superblock_t g_wfs_default_superblock = {
    .magic = WFS_MAGIC,
    .partition_size_bytes = 0,
    .cluster_size = WFS_CLUSTER_SIZE,
    .mft_record_size = WFS_MFT_RECORD_SIZE,
    .mft_start_lcn = 0,
    .journal_start_lcn = 0,
    .bitmap_start_lcn = 0,
    .transaction_id = 1,
};

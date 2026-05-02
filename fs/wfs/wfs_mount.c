#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "include/wfs_ondisk.h"

static bool wfs_is_power_of_two(uint64_t value)
{
    return value != 0 && (value & (value - 1)) == 0;
}

static bool wfs_lcn_in_bounds(const struct wfs_superblock *sb, uint64_t lcn)
{
    const uint64_t cluster_size = (uint64_t)sb->bytes_per_sector * sb->sectors_per_cluster;
    const uint64_t total_bytes = (uint64_t)sb->total_sectors * sb->bytes_per_sector;

    if (cluster_size == 0) {
        return false;
    }

    return lcn <= (total_bytes / cluster_size);
}

bool wfs_validate_superblock(const struct wfs_superblock *sb)
{
    uint64_t cluster_size;

    if (sb == NULL) {
        return false;
    }

    if (sb->magic != WFS_SUPERBLOCK_MAGIC) {
        return false;
    }

    if (sb->version_major != WFS_SUPERBLOCK_VERSION) {
        return false;
    }

    if (!wfs_is_power_of_two(sb->bytes_per_sector) ||
        !wfs_is_power_of_two(sb->sectors_per_cluster)) {
        return false;
    }

    cluster_size = (uint64_t)sb->bytes_per_sector * sb->sectors_per_cluster;
    if (cluster_size < WFS_MIN_CLUSTER_SIZE || cluster_size > WFS_MAX_CLUSTER_SIZE) {
        return false;
    }

    if (!wfs_lcn_in_bounds(sb, sb->mft_lcn) ||
        !wfs_lcn_in_bounds(sb, sb->journal_lcn) ||
        !wfs_lcn_in_bounds(sb, sb->bitmap_lcn)) {
        return false;
    }

    if (sb->mft_record_size < sizeof(struct wfs_mft_record_header) ||
        !wfs_is_power_of_two(sb->mft_record_size)) {
        return false;
    }

    return true;
}

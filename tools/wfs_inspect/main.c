#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../fs/wfs/include/wfs_ondisk.h"

static void print_uuid(const uint8_t uuid[16])
{
    printf("%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-"
           "%02x%02x%02x%02x%02x%02x",
           uuid[0], uuid[1], uuid[2], uuid[3],
           uuid[4], uuid[5], uuid[6], uuid[7],
           uuid[8], uuid[9], uuid[10], uuid[11],
           uuid[12], uuid[13], uuid[14], uuid[15]);
}

int main(int argc, char **argv)
{
    const char *path;
    FILE *fp;
    struct wfs_superblock sb;
    size_t read_size;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <wfs-image>\n", argv[0]);
        return 1;
    }

    path = argv[1];
    fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "failed to open %s: %s\n", path, strerror(errno));
        return 1;
    }

    read_size = fread(&sb, 1, sizeof(sb), fp);
    fclose(fp);
    if (read_size != sizeof(sb)) {
        fprintf(stderr, "short read: expected %zu bytes, got %zu\n", sizeof(sb), read_size);
        return 1;
    }

    printf("WFS Superblock\n");
    printf("  magic:              0x%08" PRIx32 "\n", sb.magic);
    printf("  version:            %" PRIu16 ".%" PRIu16 "\n", sb.version_major, sb.version_minor);
    printf("  bytes/sector:       %" PRIu32 "\n", sb.bytes_per_sector);
    printf("  sectors/cluster:    %" PRIu32 "\n", sb.sectors_per_cluster);
    printf("  total sectors:      %" PRIu64 "\n", sb.total_sectors);
    printf("  MFT LCN:            %" PRIu64 "\n", sb.mft_lcn);
    printf("  MFT mirror LCN:     %" PRIu64 "\n", sb.mftmirr_lcn);
    printf("  journal LCN:        %" PRIu64 "\n", sb.journal_lcn);
    printf("  bitmap LCN:         %" PRIu64 "\n", sb.bitmap_lcn);
    printf("  MFT record size:    %" PRIu32 "\n", sb.mft_record_size);
    printf("  index record size:  %" PRIu32 "\n", sb.index_record_size);
    printf("  volume UUID:        ");
    print_uuid(sb.volume_uuid);
    printf("\n");

    return 0;
}

#include "../../kernel/include/wfs.h"

ke_status_t wfs_mount(const wfs_superblock_t *sb) {
    if (!sb || sb->magic != WFS_MAGIC) {
        return KE_ERR_GENERIC;
    }
    return KE_OK;
}

ke_status_t wfs_read_file(u64 frn, void *buffer, u64 len) {
    (void)frn; (void)buffer; (void)len;
    return KE_ERR_UNSUPPORTED;
}

#include <efi.h>
#include <efilib.h>

#include "../../include/boot/boot_contract.h"

#define KERNEL_PATH L"\\System32\\winoskrnl.exe"

typedef WINOS_KERNEL_ENTRY KERNEL_ENTRY;

static EFI_STATUS load_kernel_image(
    EFI_HANDLE image,
    EFI_SYSTEM_TABLE* st,
    EFI_PHYSICAL_ADDRESS* image_base,
    KERNEL_ENTRY* entry_point) {
    EFI_LOADED_IMAGE* loaded_image = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs = NULL;
    EFI_FILE_PROTOCOL* root = NULL;
    EFI_FILE_PROTOCOL* file = NULL;
    EFI_STATUS status;

    status = uefi_call_wrapper(st->BootServices->HandleProtocol, 3,
                               image,
                               &LoadedImageProtocol,
                               (void**)&loaded_image);
    if (EFI_ERROR(status)) return status;

    status = uefi_call_wrapper(st->BootServices->HandleProtocol, 3,
                               loaded_image->DeviceHandle,
                               &FileSystemProtocol,
                               (void**)&fs);
    if (EFI_ERROR(status)) return status;

    status = uefi_call_wrapper(fs->OpenVolume, 2, fs, &root);
    if (EFI_ERROR(status)) return status;

    status = uefi_call_wrapper(root->Open, 5, root, &file, KERNEL_PATH, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) return status;

    EFI_FILE_INFO* info = NULL;
    UINTN info_size = SIZE_OF_EFI_FILE_INFO + 256;
    status = uefi_call_wrapper(st->BootServices->AllocatePool, 3, EfiLoaderData, info_size, (void**)&info);
    if (EFI_ERROR(status)) return status;

    status = uefi_call_wrapper(file->GetInfo, 4, file, &GenericFileInfo, &info_size, info);
    if (EFI_ERROR(status)) return status;

    UINTN kernel_size = (UINTN)info->FileSize;
    status = uefi_call_wrapper(st->BootServices->AllocatePages, 4,
                               AllocateAnyPages,
                               EfiLoaderData,
                               EFI_SIZE_TO_PAGES(kernel_size),
                               image_base);
    if (EFI_ERROR(status)) return status;

    status = uefi_call_wrapper(file->Read, 3, file, &kernel_size, (void*)(UINTN)(*image_base));
    if (EFI_ERROR(status)) return status;

    *entry_point = (KERNEL_ENTRY)(UINTN)(*image_base);
    return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE* st) {
    InitializeLib(image, st);

    BOOT_PARAMS params;
    ZeroMem(&params, sizeof(params));
    params.version = BOOT_PARAMS_VERSION;

    EFI_MEMORY_DESCRIPTOR* map_buf = NULL;
    UINTN map_size = 0, map_key = 0, desc_size = 0;
    UINT32 desc_version = 0;

    EFI_STATUS status = uefi_call_wrapper(st->BootServices->GetMemoryMap, 5,
                                          &map_size,
                                          map_buf,
                                          &map_key,
                                          &desc_size,
                                          &desc_version);
    if (status != EFI_BUFFER_TOO_SMALL) return status;

    map_size += desc_size * 8;
    status = uefi_call_wrapper(st->BootServices->AllocatePool, 3,
                               EfiLoaderData,
                               map_size,
                               (void**)&map_buf);
    if (EFI_ERROR(status)) return status;

    EFI_PHYSICAL_ADDRESS kernel_base = 0;
    KERNEL_ENTRY kernel_entry = NULL;
    status = load_kernel_image(image, st, &kernel_base, &kernel_entry);
    if (EFI_ERROR(status)) return status;

    status = uefi_call_wrapper(st->BootServices->GetMemoryMap, 5,
                               &map_size,
                               map_buf,
                               &map_key,
                               &desc_size,
                               &desc_version);
    if (EFI_ERROR(status)) return status;

    params.memory_map.descriptor_size = desc_size;
    params.memory_map.descriptor_version = desc_version;
    params.memory_map.descriptor_count = map_size / desc_size;
    if (params.memory_map.descriptor_count > BOOT_MAX_MEM_DESC) {
        params.memory_map.descriptor_count = BOOT_MAX_MEM_DESC;
    }

    for (UINTN i = 0; i < params.memory_map.descriptor_count; ++i) {
        EFI_MEMORY_DESCRIPTOR* src = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)map_buf + (i * desc_size));
        BOOT_MEMORY_DESCRIPTOR* dst = &params.memory_map.descriptors[i];
        dst->type = src->Type;
        dst->physical_start = src->PhysicalStart;
        dst->virtual_start = src->VirtualStart;
        dst->number_of_pages = src->NumberOfPages;
        dst->attribute = src->Attribute;
    }

    params.kernel_image_base = kernel_base;
    params.kernel_entry = (uint64_t)(UINTN)kernel_entry;

    status = uefi_call_wrapper(st->BootServices->ExitBootServices, 2, image, map_key);
    if (EFI_ERROR(status)) return status;

    kernel_entry(&params);
    return EFI_SUCCESS;
}

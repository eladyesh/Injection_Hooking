// example driver
#include <ntddk.h>

typedef NTSTATUS (*pfnNtCreateFile)(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
    );

// Pointer to the original NtCreateFile function
pfnNtCreateFile pOrigNtCreateFile;

// Hook function for NtCreateFile
NTSTATUS MyNtCreateFile(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength
    )
{
    // Do whatever you want with the function parameters here
    DbgPrint("NtCreateFile hooked\n");

    // Call the original NtCreateFile function
    return ((pfnNtCreateFile)(SSDT[index]))(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
}

NTSTATUS DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{
    UNREFERENCED_PARAMETER(RegistryPath);

    // Get the address of KeServiceDescriptorTable
    PVOID* SSDT = (PVOID*)MmGetSystemRoutineAddress((PUNICODE_STRING)&"KeServiceDescriptorTable");
    if (SSDT == NULL) {
        DbgPrint("Failed to obtain KeServiceDescriptorTable\n");
        return STATUS_UNSUCCESSFUL;
    }

    // Find the index of NtCreateFile in the SSDT
    ULONG index = 0;
    for (index = 0; index < 0x1000; index++) {
        if (MmIsAddressValid((PVOID)(SSDT[index]))) {
            PCHAR serviceName = (PCHAR)((PULONG)(*((PULONG)(SSDT[index])) + 1));
            if (strcmp(serviceName, "NtCreateFile") == 0) {
                break;
            }
        }
    }
    if (index >= 0x1000) {
        DbgPrint("Failed to find NtCreateFile in SSDT\n");
        return STATUS_UNSUCCESSFUL;
    }

    // Save the address of the original NtCreateFile function
    pOrigNtCreateFile = (pfnNtCreateFile)(SSDT[index]);

    // Replace the SSDT entry for NtCreateFile with the address of our hook function
    DWORD oldProtect;
    VirtualProtect(&pOrigNtCreateFile, sizeof(PVOID), PAGE_EXECUTE_READWRITE, &oldProtect);
    *pOrigNtCreateFile = (PVOID)MyNtCreateFile;
    VirtualProtect(&pOrigNtCreateFile, sizeof(PVOID), oldProtect, NULL);

    // Return success
    DbgPrint("Driver loaded\n");
    return STATUS_SUCCESS;
}

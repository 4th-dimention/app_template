
function char *
W32_FrameCStringFromString(String8 string)
{
    char *buffer = 0;
    buffer = M_ArenaPushZero(&os->frame_arena, string.size+1);
    MemoryCopy(buffer, string.str, string.size);
    return buffer;
}

function void
W32_SaveToFile(String8 path, void *data, U64 data_len)
{
    HANDLE file = {0};
    {
        DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = CREATE_ALWAYS;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFile(W32_FrameCStringFromString(path),
                              desired_access,
                              share_mode,
                              &security_attributes,
                              creation_disposition,
                              flags_and_attributes,
                              template_file)) != INVALID_HANDLE_VALUE)
        {
            
            void *data_to_write = data;
            DWORD data_to_write_size = (DWORD)data_len;
            DWORD bytes_written = 0;
            
            WriteFile(file, data_to_write, data_to_write_size, &bytes_written, 0);
            
            CloseHandle(file);
        }
        else
        {
            W32_OutputError("File I/O Error", "Could not save to \"%s\"", path);
        }
    }
}

function void
W32_AppendToFile(String8 path, void *data, U64 data_len)
{
    HANDLE file = {0};
    {
        DWORD desired_access = FILE_APPEND_DATA;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = OPEN_ALWAYS;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFileA(W32_FrameCStringFromString(path),
                               desired_access,
                               share_mode,
                               &security_attributes,
                               creation_disposition,
                               flags_and_attributes,
                               template_file)) != INVALID_HANDLE_VALUE)
        {
            
            void *data_to_write = data;
            DWORD data_to_write_size = (DWORD)data_len;
            DWORD bytes_written = 0;
            
            SetFilePointer(file, 0, 0, FILE_END);
            WriteFile(file, data_to_write, data_to_write_size, &bytes_written, 0);
            
            CloseHandle(file);
        }
        else
        {
            W32_OutputError("File I/O Error", "Could not save to \"%s\"", path);
        }
    }
}

function void
W32_LoadEntireFile(M_Arena *arena, String8 path, void **data, U64 *data_len)
{
    *data = 0;
    *data_len = 0;
    
    HANDLE file = {0};
    
    {
        DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = OPEN_EXISTING;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFile(W32_FrameCStringFromString(path), desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
        {
            
            DWORD read_bytes = GetFileSize(file, 0);
            if(read_bytes)
            {
                void *read_data = M_ArenaPush(arena, read_bytes+1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, read_data, read_bytes, &bytes_read, &overlapped);
                
                ((U8 *)read_data)[read_bytes] = 0;
                
                *data = read_data;
                *data_len = (U64)bytes_read;
            }
            CloseHandle(file);
        }
    }
}

function char *
W32_LoadEntireFileAndNullTerminate(M_Arena *arena, String8 path)
{
    char *result = 0;
    
    HANDLE file = {0};
    {
        DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes = {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = OPEN_EXISTING;
        DWORD flags_and_attributes = 0;
        HANDLE template_file = 0;
        
        if((file = CreateFile(W32_FrameCStringFromString(path), desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
        {
            
            DWORD read_bytes = GetFileSize(file, 0);
            if(read_bytes)
            {
                result = M_ArenaPush(arena, read_bytes+1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};
                
                ReadFile(file, result, read_bytes, &bytes_read, &overlapped);
                
                result[read_bytes] = 0;
            }
            CloseHandle(file);
        }
        else
        {
            W32_OutputError("File I/O Error", "Could not read from \"%s\"", path);
        }
    }
    
    return result;
}

function void
W32_FreeFileMemory(void *data)
{
    W32_HeapFree(data);
}

function void
W32_DeleteFile(String8 path)
{
    DeleteFileA(W32_FrameCStringFromString(path));
}

function B32
W32_MakeDirectory(String8 path)
{
    B32 result = 1;
    if(!CreateDirectoryA(W32_FrameCStringFromString(path), 0))
    {
        result = 0;
    }
    return result;
}

function B32
W32_DoesFileExist(String8 path)
{
    B32 found = GetFileAttributesA(W32_FrameCStringFromString(path)) != INVALID_FILE_ATTRIBUTES;
    return found;
}

function B32
W32_DoesDirectoryExist(String8 path)
{
    DWORD file_attributes = GetFileAttributesA(W32_FrameCStringFromString(path));
    B32 found = (file_attributes != INVALID_FILE_ATTRIBUTES &&
                 !!(file_attributes & FILE_ATTRIBUTE_DIRECTORY));
    return found;
}

function B32
W32_CopyFile(String8 dest, String8 source)
{
    B32 success = 0;
    success = CopyFile(W32_FrameCStringFromString(source), W32_FrameCStringFromString(dest), 0);
    return success;
}

function OS_DirectoryList
W32_DirectoryListLoad(M_Arena *arena, String8 path, S32 flags)
{
    OS_DirectoryList list = {0};
    
    return list;
}

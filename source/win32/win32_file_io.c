
function B32
OS_SaveToFile(String8 path, void *data, U64 data_len)
{
    B32 result = 0;
    HANDLE file = {0};
    
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
    
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    
    if((file = CreateFile((char*)path_copy.str,
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
        result = 1;
    }
    else
    {
        OS_OutputError("File I/O Error", "Could not save to \"%s\"", path);
    }
    
    OS_ReleaseScratch(scratch);
    
    return(result);
}

function void
OS_AppendToFile(String8 path, void *data, U64 data_len)
{
    HANDLE file = {0};
    
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
    
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    
    if((file = CreateFileA((char*)path_copy.str,
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
        OS_OutputError("File I/O Error", "Could not save to \"%s\"", path);
    }
    
    OS_ReleaseScratch(scratch);
}

function void
OS_LoadEntireFile(M_Arena *arena, String8 path, void **data, U64 *data_len)
{
    *data = 0;
    *data_len = 0;
    
    HANDLE file = {0};
    
    
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
    
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    
    if((file = CreateFile((char*)path_copy.str, desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
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
    
    OS_ReleaseScratch(scratch);
    
}

function char *
OS_LoadEntireFileAndNullTerminate(M_Arena *arena, String8 path)
{
    char *result = 0;
    
    HANDLE file = {0};
    
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
    
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    
    if((file = CreateFile((char*)path_copy.str, desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attributes, template_file)) != INVALID_HANDLE_VALUE)
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
        OS_OutputError("File I/O Error", "Could not read from \"%s\"", path.str);
    }
    
    OS_ReleaseScratch(scratch);
    
    return result;
}

function void
W32_FreeFileMemory(void *data)
{
    W32_HeapFree(data);
}

function void
OS_DeleteFile(String8 path)
{
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    DeleteFileA((char*)path_copy.str);
    OS_ReleaseScratch(scratch);
}

function B32
OS_MakeDirectory(String8 path)
{
    B32 result = 1;
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    if(!CreateDirectoryA((char*)path_copy.str, 0))
    {
        result = 0;
    }
    OS_ReleaseScratch(scratch);
    return result;
}

function B32
OS_DoesFileExist(String8 path)
{
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    B32 found = (GetFileAttributesA((char*)path_copy.str) != INVALID_FILE_ATTRIBUTES);
    OS_ReleaseScratch(scratch);
    return found;
}

function B32
OS_DoesDirectoryExist(String8 path)
{
    M_Arena *scratch = OS_GetScratch();
    String8 path_copy = PushStringCopy(scratch, path);
    DWORD file_attributes = GetFileAttributesA((char*)(path_copy.str));
    B32 found = (file_attributes != INVALID_FILE_ATTRIBUTES &&
                 !!(file_attributes & FILE_ATTRIBUTE_DIRECTORY));
    
    OS_ReleaseScratch(scratch);
    
    return found;
}

function OS_DirectoryList
OS_DirectoryListLoad(M_Arena *arena, String8 path, S32 flags)
{
    OS_DirectoryList list = {0};
    
    return list;
}


function U8*
_W32_InitFilter(M_Arena *arena, String8 *fixed_ext){
    U8 *filter = 0;
    if (fixed_ext != 0){
        String8List list = {0};
        StringListPush(arena, &list, fixed_ext[0]);
        StringListPush(arena, &list, str8_lit("\0"));
        StringListPush(arena, &list, str8_lit("*."));
        StringListPush(arena, &list, fixed_ext[1]);
        StringListPush(arena, &list, str8_lit("\0\0"));
        String8 filter_str = StringListJoin(arena, &list, 0);
        filter = filter_str.str;
    }
    return(filter);
}

function String8
OS_DialogueSavePath(M_Arena *arena, String8 *fixed_ext)
{
    M_Arena *scratch = OS_GetScratch1(arena);
    
    OPENFILENAMEA openfilename = {sizeof(openfilename)};
    openfilename.lpstrFilter = (LPSTR)_W32_InitFilter(scratch, fixed_ext);
    openfilename.nMaxFile = KB(32);
    openfilename.lpstrFile = (LPSTR)PushArray(arena, U8, openfilename.nMaxFile);
    openfilename.lpstrFile[0] = 0;
    openfilename.lpstrTitle = "Save";
    openfilename.Flags = OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    
    String8 result = {0};
    if (GetSaveFileNameA(&openfilename)){
        result.str = openfilename.lpstrFile;
        result.size = 0;
        for (;openfilename.lpstrFile[result.size] != 0; result.size += 1);
    }
    
    OS_ReleaseScratch(scratch);
    return(result);
}

function String8
OS_DialogueLoadPath(M_Arena *arena, String8 *fixed_ext)
{
    M_Arena *scratch = OS_GetScratch1(arena);
    
    OPENFILENAMEA openfilename = {sizeof(openfilename)};
    openfilename.lpstrFilter = (LPSTR)_W32_InitFilter(scratch, fixed_ext);
    openfilename.nMaxFile = KB(32);
    openfilename.lpstrFile = (LPSTR)PushArray(arena, U8, openfilename.nMaxFile);
    openfilename.lpstrFile[0] = 0;
    openfilename.lpstrTitle = "Open";
    openfilename.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
    
    String8 result = {0};
    if (GetOpenFileNameA(&openfilename)){
        result.str = openfilename.lpstrFile;
        result.size = 0;
        for (;openfilename.lpstrFile[result.size] != 0; result.size += 1);
    }
    
    OS_ReleaseScratch(scratch);
    return(result);
}

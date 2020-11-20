
typedef struct W32_AppCode W32_AppCode;
struct W32_AppCode
{
    ApplicationPermanentLoadCallback  *PermanentLoad;
    ApplicationHotLoadCallback        *HotLoad;
    ApplicationHotUnloadCallback      *HotUnload;
    ApplicationUpdateCallback         *Update;
    HMODULE dll;
    FILETIME last_dll_write_time;
};

internal B32
W32_AppCodeLoad(W32_AppCode *app_code)
{
    B32 result = 1;
    
    CopyFile(global_app_dll_path, global_temp_app_dll_path, FALSE);
    app_code->dll = LoadLibraryA(global_temp_app_dll_path);
    
    app_code->last_dll_write_time = W32_GetLastWriteTime(global_app_dll_path);
    
    if(!app_code->dll)
    {
        result = 0;
        goto end;
    }
    
    app_code->PermanentLoad    = (void *)GetProcAddress(app_code->dll, "PermanentLoad");
    app_code->HotLoad          = (void *)GetProcAddress(app_code->dll, "HotLoad");
    app_code->HotUnload        = (void *)GetProcAddress(app_code->dll, "HotUnload");
    app_code->Update           = (void *)GetProcAddress(app_code->dll, "Update");
    
    if(!app_code->PermanentLoad || !app_code->HotLoad || !app_code->HotUnload || !app_code->Update)
    {
        app_code->PermanentLoad = ApplicationPermanentLoadStub;
        app_code->HotLoad = ApplicationHotLoadStub;
        app_code->HotUnload = ApplicationHotUnloadStub;
        app_code->Update = ApplicationUpdateStub;
        result = 0;
        goto end;
    }
    
    end:;
    return result;
}

internal void
W32_AppCodeUnload(W32_AppCode *app_code)
{
    if(app_code->dll)
    {
        FreeLibrary(app_code->dll);
    }
    app_code->dll = 0;
    app_code->PermanentLoad = ApplicationPermanentLoadStub;
    app_code->HotLoad = ApplicationHotLoadStub;
    app_code->HotUnload = ApplicationHotUnloadStub;
    app_code->Update = ApplicationUpdateStub;
}

internal void
W32_AppCodeUpdate(W32_AppCode *app_code)
{
    FILETIME last_write_time = W32_GetLastWriteTime(global_app_dll_path);
    if(CompareFileTime(&last_write_time, &app_code->last_dll_write_time))
    {
        app_code->HotUnload();
        W32_AppCodeUnload(app_code);
        W32_AppCodeLoad(app_code);
        app_code->HotLoad(&global_os);
    }
}

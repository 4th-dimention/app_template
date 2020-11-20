
//~ NOTE(rjf): Keys

typedef enum Key
{
#define Key(name, str) Key_##name,
#include "os_key_list.inc"
#undef Key
    Key_Max
}
Key;

typedef U32 KeyModifiers;
enum
{
    KeyModifier_Ctrl  = (1<<0),
    KeyModifier_Shift = (1<<1),
    KeyModifier_Alt   = (1<<2),
};

//~ NOTE(rjf): Mouse

typedef enum MouseButton
{
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
}
MouseButton;

//~ NOTE(rjf): Gamepads

typedef enum GamepadButton
{
#define GamepadButton(name, str) GamepadButton_##name,
#include "os_gamepad_button_list.inc"
#undef GamepadButton
    GamepadButton_Max
}
GamepadButton;

//~ NOTE(rjf): Platform Directory Listing

#define OS_DirectoryList_IncludeDirectories (1<<0)
#define OS_DirectoryList_IncludeExtensions  (1<<1)
#define OS_DirectoryList_SearchRecursively  (1<<2)
#define OS_DirectoryItem_IsDirectory        (1<<0)

typedef struct OS_DirectoryItem OS_DirectoryItem;
struct OS_DirectoryItem
{
    String8 string;
    U64 flags;
};

typedef struct OS_DirectoryItemChunk OS_DirectoryItemChunk;
struct OS_DirectoryItemChunk
{
    OS_DirectoryItem items[32];
    OS_DirectoryItemChunk *next;
    U64 item_count;
};

typedef struct OS_DirectoryList OS_DirectoryList;
struct OS_DirectoryList
{
    U64 flags;
    U64 item_count;
    OS_DirectoryItemChunk *first_chunk;
};

//~ NOTE(rjf): Events

typedef enum OS_EventType
{
    OS_EventType_Null,
    
    // NOTE(rjf): Keyboard
    OS_EventType_KeyStart,
    OS_EventType_CharacterInput,
    OS_EventType_KeyPress,
    OS_EventType_KeyRelease,
    OS_EventType_KeyEnd,
    
    // NOTE(rjf): Mouse
    OS_EventType_MouseStart,
    OS_EventType_MousePress,
    OS_EventType_MouseRelease,
    OS_EventType_MouseMove,
    OS_EventType_MouseScroll,
    OS_EventType_MouseEnd,
    
    // NOTE(rjf): Gamepads
    OS_EventType_GamepadStart,
    OS_EventType_GamepadConnect,
    OS_EventType_GamepadDisconnect,
    OS_EventType_GamepadButtonPress,
    OS_EventType_GamepadButtonRelease,
    OS_EventType_GamepadJoystickMove,
    OS_EventType_GamepadTrigger,
    OS_EventType_GamepadEnd,
    
    OS_EventType_Max,
}
OS_EventType;

typedef struct OS_Event OS_Event;
struct OS_Event
{
    OS_EventType type;
    Key key;
    GamepadButton gamepad_button;
    MouseButton mouse_button;
    KeyModifiers modifiers;
    S32 gamepad_index;
    U64 character;
    v2 position;
    v2 delta;
    v2 scroll;
};

//~ NOTE(allen): Thread Context

typedef struct OS_ArenaInlineRestore OS_ArenaInlineRestore;
struct OS_ArenaInlineRestore{
    OS_ArenaInlineRestore *next;
};

typedef struct OS_ArenaNode OS_ArenaNode;
struct OS_ArenaNode
{
    OS_ArenaNode *next;
    OS_ArenaNode *prev;
    M_Arena arena;
    OS_ArenaInlineRestore *restore;
    U64 ref_count;
};

typedef struct OS_File_Line OS_File_Line;
struct OS_File_Line
{
    char *file_name;
    U64 line_number;
};

typedef struct OS_ThreadContext OS_ThreadContext;
struct OS_ThreadContext
{
    OS_ArenaNode *free;
    OS_ArenaNode *first_used;
    OS_ArenaNode *last_used;
    char *file_name;
    U64 line_number;
};

//~ NOTE(rjf): Platform Data

typedef struct OS_State OS_State;
struct OS_State
{
    // NOTE(rjf): Application Metadata
    String8 executable_folder_absolute_path;
    String8 executable_absolute_path;
    String8 working_directory_path;
    
    // NOTE(rjf): Memory
    M_Arena permanent_arena;
    M_Arena frame_arena;
    
    // NOTE(rjf): Options
    volatile B32 quit;
    B32 vsync;
    B32 fullscreen;
    iv2 window_size;
    F32 current_time;
    F32 target_frames_per_second;
    B32 wait_for_events_to_update;
    B32 pump_events;
    
    // NOTE(rjf): Event Queue and Input Data
    v2 mouse_position;
    U64 event_count;
    OS_Event events[4096];
    
    // NOTE(rjf): Audio Output Data
    F32 *sample_out;
    U32 sample_count_to_output;
    U32 samples_per_second;
    
    // NOTE(rjf): Functions
    void *(*Reserve)(U64 size);
    void (*Release)(void *memory);
    void (*Commit)(void *memory, U64 size);
    void (*Decommit)(void *memory, U64 size);
    void (*OutputError)(char *error_type, char *error_format, ...);
    void (*SaveToFile)(String8 path, void *data, U64 data_len);
    void (*AppendToFile)(String8 path, void *data, U64 data_len);
    void (*LoadEntireFile)(M_Arena *arena, String8 path, void **data, U64 *data_len);
    char *(*LoadEntireFileAndNullTerminate)(M_Arena *arena, String8 path);
    void (*DeleteFile)(String8 path);
    B32 (*MakeDirectory)(String8 path);
    B32 (*DoesFileExist)(String8 path);
    B32 (*DoesDirectoryExist)(String8 path);
    B32 (*CopyFile)(String8 dest, String8 source);
    OS_DirectoryList (*ListDirectory)(M_Arena *arena, String8 path, S32 flags);
    F32 (*GetTime)(void);
    U64 (*GetCycles)(void);
    void (*ResetCursor)(void);
    void (*SetCursorToHorizontalResize)(void);
    void (*SetCursorToVerticalResize)(void);
    void (*SetCursorToIBar)(void);
    void (*RefreshScreen)(void);
    void *(*LoadOpenGLProcedure)(char *name);
    OS_ThreadContext *(*GetThreadContext)(void);
};

global OS_State *os = 0;

#ifdef _MSC_VER
#define APP_ENTRY_POINT __declspec(dllexport)
#else
#define APP_ENTRY_POINT
#endif

/* Loaded as "PermanentLoad" */
#define APP_PERMANENT_LOAD APP_ENTRY_POINT void PermanentLoad(OS_State *os_)
typedef void ApplicationPermanentLoadCallback(OS_State *);
internal void ApplicationPermanentLoadStub(OS_State *_) {}

/* Loaded as "HotLoad" */
#define APP_HOT_LOAD APP_ENTRY_POINT void HotLoad(OS_State *os_)
typedef void ApplicationHotLoadCallback(OS_State *);
internal void ApplicationHotLoadStub(OS_State *_) {}

/* Loaded as "HotUnload" */
#define APP_HOT_UNLOAD APP_ENTRY_POINT void HotUnload(void)
typedef void ApplicationHotUnloadCallback(void);
internal void ApplicationHotUnloadStub(void) {}

/* Loaded as "Update" */
#define APP_UPDATE APP_ENTRY_POINT void Update(void)
typedef void ApplicationUpdateCallback(void);
internal void ApplicationUpdateStub(void) {}

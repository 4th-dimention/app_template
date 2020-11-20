////////////////////////////////
// NOTE(allen): Directory Listing Types

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

////////////////////////////////
// NOTE(allen): Event Codes

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

typedef enum MouseButton
{
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
}
MouseButton;

typedef enum GamepadButton
{
#define GamepadButton(name, str) GamepadButton_##name,
#include "os_gamepad_button_list.inc"
#undef GamepadButton
    GamepadButton_Max
}
GamepadButton;

////////////////////////////////
// NOTE(allen): Events

typedef enum OS_EventKind
{
    OS_EventKind_Null,
    
    // NOTE(rjf): Window
    OS_EventKind_WindowClose,
    
    // NOTE(rjf): Keyboard
    OS_EventKind_KeyStart,
    OS_EventKind_CharacterInput,
    OS_EventKind_KeyPress,
    OS_EventKind_KeyRelease,
    OS_EventKind_KeyEnd,
    
    // NOTE(rjf): Mouse
    OS_EventKind_MouseStart,
    OS_EventKind_MousePress,
    OS_EventKind_MouseRelease,
    OS_EventKind_MouseMove,
    OS_EventKind_MouseScroll,
    OS_EventKind_MouseEnd,
    
    // NOTE(rjf): Gamepads
    OS_EventKind_GamepadStart,
    OS_EventKind_GamepadConnect,
    OS_EventKind_GamepadDisconnect,
    OS_EventKind_GamepadButtonPress,
    OS_EventKind_GamepadButtonRelease,
    OS_EventKind_GamepadJoystickMove,
    OS_EventKind_GamepadTrigger,
    OS_EventKind_GamepadEnd,
    
    OS_EventKind_Max,
}
OS_EventKind;

typedef struct OS_Event OS_Event;
struct OS_Event
{
    OS_EventKind kind;
    Key key;
    GamepadButton gamepad_button;
    MouseButton mouse_button;
    KeyModifiers modifiers;
    S32 gamepad_index;
    U32 character;
    V2F32 position;
    V2F32 delta;
    V2F32 scroll;
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
    V2S32 window_size;
    F32 current_time;
    F32 target_frames_per_second;
    B32 wait_for_events_to_update;
    B32 pump_events;
    
    // NOTE(rjf): Event Queue and Input Data
    V2F32 mouse_position;
    U64 event_count;
    OS_Event events[4096];
    
    // NOTE(rjf): Audio Output Data
    F32 *sample_out;
    U32 sample_count_to_output;
    U32 samples_per_second;
};

global OS_State *os = 0;

////////////////////////////////
// NOTE(rjf): OS functions with specialized implementations

void  OS_Quit(void);
void* OS_Reserve(U64 size);
void  OS_Release(void *memory);
void  OS_Commit(void *memory, U64 size);
void  OS_Decommit(void *memory, U64 size);
void  OS_OutputError(char *error_type, char *error_format, ...);
B32   OS_SaveToFile(String8 path, void *data, U64 data_len);
void  OS_AppendToFile(String8 path, void *data, U64 data_len);
void  OS_LoadEntireFile(M_Arena *arena, String8 path, void **data, U64 *data_len);
char* OS_LoadEntireFileAndNullTerminate(M_Arena *arena, String8 path);
void  OS_DeleteFile(String8 path);
B32   OS_MakeDirectory(String8 path);
B32   OS_DoesFileExist(String8 path);
B32   OS_DoesDirectoryExist(String8 path);
OS_DirectoryList OS_ListDirectory(M_Arena *arena, String8 path, S32 flags);
F32   OS_GetTime(void);
U64   OS_GetCycles(void);
void  OS_ResetCursor(void);
void  OS_SetCursorToHorizontalResize(void);
void  OS_SetCursorToVerticalResize(void);
void  OS_SetCursorToIBar(void);
void  OS_RefreshScreen(void);
void* OS_LoadOpenGLProcedure(char *name);
OS_ThreadContext* OS_GetThreadContext(void);
String8 OS_DialogueSavePath(M_Arena *arena, String8 *fixed_extension);
String8 OS_DialogueLoadPath(M_Arena *arena, String8 *fixed_extension);

////////////////////////////////
// NOTE(allen): APP entry points

void APP_Init(void);
void APP_Update(void);

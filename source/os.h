////////////////////////////////
// NOTE(allen): Constants

global U64 os_repaint_interval_usec = Million(1)/60;
global String8 os_window_title = str8_comp("Application");
global U64 os_default_window_width  = 1280;
global U64 os_default_window_height =  720;

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

typedef struct OS_FileLine OS_FileLine;
struct OS_FileLine
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
void  OS_NextFrameImmediate(B32 immediate);
void  OS_NextFrameFullScreen(B32 full_screen);
RectF32 OS_WindowGetRect(void);

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
U64   OS_GetNowInMicroseconds(void);
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
// NOTE(allen): OS functions with general implementations

function String8 KeyName(Key index);
function String8 GamepadButtonName(GamepadButton index);
function B32 OS_EventIsMouse(OS_Event * event);
function B32 OS_CompareEvents(OS_Event a, OS_Event b);
function OS_Event OS_KeyPressEvent(Key key, KeyModifiers modifiers);
function OS_Event OS_KeyReleaseEvent(Key key, KeyModifiers modifiers);
function OS_Event OS_CharacterInputEvent(U32 character);
function OS_Event OS_MouseMoveEvent(V2F32 position, V2F32 delta);
function OS_Event OS_MousePressEvent(MouseButton button, V2F32 position);
function OS_Event OS_MouseReleaseEvent(MouseButton mouse_button, V2F32 position);
function OS_Event OS_MouseScrollEvent(V2F32 delta, KeyModifiers modifiers);
function OS_Event OS_WindowClose(void);
function B32 OS_GetNextEvent(OS_Event * * event);
function void OS_EatEvent(OS_Event * event);
function void OS_PushEvent(OS_Event event);
function M_Arena * OS_GetScratch(void);
function M_Arena * OS_GetScratch1(M_Arena * a1);
function M_Arena * OS_GetScratch2(M_Arena * a1, M_Arena * a2);
function void OS_ReleaseScratch(M_Arena * arena);
function OS_FileLine OS_ThreadRememberFileLine(void);

////////////////////////////////
// NOTE(allen): APP entry points

void APP_Init(void);
void APP_Update(void);

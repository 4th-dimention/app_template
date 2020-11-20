////////////////////////////////
// NOTE(allen): Events

function String8
KeyName(Key index)
{
    local char *strings[Key_Max] =
    {
#define Key(name, str) str,
#include "os_key_list.inc"
#undef Key
    };
    char *string = "(Invalid Key)";
    if(index >= 0 && index < ArrayCount(strings))
    {
        string = strings[index];
    }
    String8 result;
    result.str = string;
    result.size = CalculateCStringLength(result.str);
    return result;
}

function String8
GamepadButtonName(GamepadButton index)
{
    local char *strings[GamepadButton_Max] =
    {
#define GamepadButton(name, str) str,
#include "os_gamepad_button_list.inc"
#undef GamepadButton
    };
    char *string = "(Invalid Gamepad Button)";
    if(index >= 0 && index < ArrayCount(strings))
    {
        string = strings[index];
    }
    String8 result;
    result.str = string;
    result.size = CalculateCStringLength(result.str);
    return result;
}

function B32
OS_EventIsMouse(OS_Event *event)
{
    return event->kind > OS_EventKind_MouseStart && event->kind < OS_EventKind_MouseEnd;
}

function B32
OS_CompareEvents(OS_Event a, OS_Event b)
{
    B32 result = 0;
    if(a.kind == b.kind &&
       a.key == b.key &&
       a.mouse_button == b.mouse_button &&
       a.gamepad_button == b.gamepad_button &&
       a.character == b.character &&
       a.modifiers == b.modifiers)
    {
        result = 1;
    }
    return result;
}

function OS_Event
OS_KeyPressEvent(Key key, KeyModifiers modifiers)
{
    OS_Event event =
    {
        .kind = OS_EventKind_KeyPress,
    };
    event.key = key;
    event.modifiers = modifiers;
    return event;
}

function OS_Event
OS_KeyReleaseEvent(Key key, KeyModifiers modifiers)
{
    OS_Event event =
    {
        .kind = OS_EventKind_KeyRelease,
    };
    event.key = key;
    event.modifiers = modifiers;
    return event;
}

function OS_Event
OS_CharacterInputEvent(U32 character)
{
    OS_Event event =
    {
        .kind = OS_EventKind_CharacterInput,
    };
    event.character = character;
    return event;
}

function OS_Event
OS_MouseMoveEvent(V2F32 position, V2F32 delta)
{
    OS_Event event =
    {
        .kind = OS_EventKind_MouseMove,
    };
    event.position = position;
    event.delta = delta;
    return event;
}

function OS_Event
OS_MousePressEvent(MouseButton button, V2F32 position)
{
    OS_Event event =
    {
        .kind = OS_EventKind_MousePress,
    };
    event.mouse_button = button;
    event.position = position;
    return event;
}

function OS_Event
OS_MouseReleaseEvent(MouseButton mouse_button, V2F32 position)
{
    OS_Event event =
    {
        .kind = OS_EventKind_MouseRelease,
    };
    event.mouse_button = mouse_button;
    event.position = position;
    return event;
}

function OS_Event
OS_MouseScrollEvent(V2F32 delta, KeyModifiers modifiers)
{
    OS_Event event =
    {
        .kind = OS_EventKind_MouseScroll,
    };
    event.scroll = delta;
    event.modifiers = modifiers;
    return event;
}

function OS_Event
OS_WindowClose(void)
{
    OS_Event event = 
    {
        .kind = OS_EventKind_WindowClose,
    };
    return(event);
}

function B32
OS_GetNextEvent(OS_Event **event)
{
    B32 result = 0;
    Assert(os != 0);
    U32 start_index = 0;
    OS_Event *new_event = 0;
    if(*event)
    {
        start_index = (*event - os->events) + 1;
    }
    for(U32 i = start_index; i < os->event_count; ++i)
    {
        if(os->events[i].kind != OS_EventKind_Null)
        {
            new_event = os->events+i;
            break;
        }
    }
    *event = new_event;
    result = (new_event != 0);
    return result;
}

function void
OS_EatEvent(OS_Event *event)
{
    event->kind = OS_EventKind_Null;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
function void
OS_BeginFrame(void)
{
    os->pump_events = 0;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
function void
OS_EndFrame(void)
{
    //os->current_time += 1.f / os->target_frames_per_second;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
function void
OS_PushEvent(OS_Event event)
{
    Assert(os != 0);
    if(os->event_count < ArrayCount(os->events))
    {
        os->events[os->event_count++] = event;
    }
}

////////////////////////////////
// NOTE(allen): Thread Context

function OS_ArenaNode*
_OS_GetFreeScratch(OS_ThreadContext *tctx)
{
    OS_ArenaNode *usable_node = tctx->free;
    Assert(usable_node != 0);
    SLLStackPop(tctx->free);
    DLLPushBack(tctx->first_used, tctx->last_used, usable_node);
    return(usable_node);
}

function M_Arena*
_OS_MarkArenaRestore(OS_ArenaNode *node)
{
    M_Arena *result = &node->arena;
    OS_ArenaInlineRestore *restore = PushArray(result, OS_ArenaInlineRestore, 1);
    SLLStackPush(node->restore, restore);
    node->ref_count += 1;
    return(result);
}

function M_Arena*
OS_GetScratch(void)
{
    OS_ThreadContext *tctx = OS_GetThreadContext();
    OS_ArenaNode *usable_node = tctx->first_used;
    if (usable_node == 0)
    {
        usable_node = _OS_GetFreeScratch(tctx);
    }
    return(_OS_MarkArenaRestore(usable_node));
}

function M_Arena*
OS_GetScratch1(M_Arena *a1)
{
    OS_ThreadContext *tctx = OS_GetThreadContext();
    OS_ArenaNode *usable_node = 0;
    for (OS_ArenaNode *node = tctx->first_used;
         node != 0;
         node = node->next)
    {
        if (&node->arena != a1)
        {
            usable_node = node;
            break;
        }
    }
    if (usable_node == 0)
    {
        usable_node = _OS_GetFreeScratch(tctx);
    }
    return(_OS_MarkArenaRestore(usable_node));
}

function M_Arena*
OS_GetScratch2(M_Arena *a1, M_Arena *a2)
{
    OS_ThreadContext *tctx = OS_GetThreadContext();
    OS_ArenaNode *usable_node = 0;
    for (OS_ArenaNode *node = tctx->first_used;
         node != 0;
         node = node->next)
    {
        if (&node->arena != a1 && &node->arena != a2)
        {
            usable_node = node;
            break;
        }
    }
    if (usable_node == 0)
    {
        usable_node = _OS_GetFreeScratch(tctx);
    }
    return(_OS_MarkArenaRestore(usable_node));
}

function void
OS_ReleaseScratch(M_Arena *arena)
{
    OS_ArenaNode *node = CastFromMember(OS_ArenaNode, arena, arena);
    Assert(node->ref_count > 0);
    node->ref_count -= 1;
    if (node->ref_count == 0)
    {
        OS_ThreadContext *tctx = OS_GetThreadContext();
        DLLRemove(tctx->first_used, tctx->last_used, node);
        M_ArenaClear(arena);
        SLLStackPush(tctx->free, node);
    }
    else
    {
        void *ptr = node->restore;
        node->restore = node->restore->next;
        M_ArenaSetPosBackByPtr(arena, ptr);
    }
}

function void
_OS_ThreadSaveFileLine(char *file_name, U64 line_number)
{
    OS_ThreadContext *tctx = OS_GetThreadContext();
    tctx->file_name = file_name;
    tctx->line_number = line_number;
}

#define OS_ThreadSaveFileLine() _OS_ThreadSaveFileLine(__FILE__, __LINE__)

function OS_File_Line
OS_ThreadRememberFileLine(void)
{
    OS_ThreadContext *tctx = OS_GetThreadContext();
    OS_File_Line result = {tctx->file_name, tctx->line_number};
    return(result);
}



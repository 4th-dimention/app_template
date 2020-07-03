////////////////////////////////
// NOTE(allen): Events

internal String8
KeyName(Key index)
{
    local_persist char *strings[Key_Max] =
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

internal String8
GamepadButtonName(GamepadButton index)
{
    local_persist char *strings[GamepadButton_Max] =
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

internal b32
OS_EventIsMouse(OS_Event *event)
{
    return event->type > OS_EventType_MouseStart && event->type < OS_EventType_MouseEnd;
}

internal b32
OS_CompareEvents(OS_Event a, OS_Event b)
{
    b32 result = 0;
    if(a.type == b.type &&
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

internal OS_Event
OS_KeyPressEvent(Key key, KeyModifiers modifiers)
{
    OS_Event event =
    {
        .type = OS_EventType_KeyPress,
    };
    event.key = key;
    event.modifiers = modifiers;
    return event;
}

internal OS_Event
OS_KeyReleaseEvent(Key key, KeyModifiers modifiers)
{
    OS_Event event =
    {
        .type = OS_EventType_KeyRelease,
    };
    event.key = key;
    event.modifiers = modifiers;
    return event;
}

internal OS_Event
OS_CharacterInputEvent(u64 character)
{
    OS_Event event =
    {
        .type = OS_EventType_CharacterInput,
    };
    event.character = character;
    return event;
}

internal OS_Event
OS_MouseMoveEvent(v2 position, v2 delta)
{
    OS_Event event =
    {
        .type = OS_EventType_MouseMove,
    };
    event.position = position;
    event.delta = delta;
    return event;
}

internal OS_Event
OS_MousePressEvent(MouseButton button, v2 position)
{
    OS_Event event =
    {
        .type = OS_EventType_MousePress,
    };
    event.mouse_button = button;
    event.position = position;
    return event;
}

internal OS_Event
OS_MouseReleaseEvent(MouseButton mouse_button, v2 position)
{
    OS_Event event =
    {
        .type = OS_EventType_MouseRelease,
    };
    event.mouse_button = mouse_button;
    event.position = position;
    return event;
}

internal OS_Event
OS_MouseScrollEvent(v2 delta, KeyModifiers modifiers)
{
    OS_Event event =
    {
        .type = OS_EventType_MouseScroll,
    };
    event.scroll = delta;
    event.modifiers = modifiers;
    return event;
}

internal b32
OS_GetNextEvent(OS_Event **event)
{
    b32 result = 0;
    Assert(os != 0);
    u32 start_index = 0;
    OS_Event *new_event = 0;
    if(*event)
    {
        start_index = (*event - os->events) + 1;
    }
    for(u32 i = start_index; i < os->event_count; ++i)
    {
        if(os->events[i].type != OS_EventType_Null)
        {
            new_event = os->events+i;
            break;
        }
    }
    *event = new_event;
    result = new_event != 0;
    return result;
}

internal void
OS_EatEvent(OS_Event *event)
{
    event->type = OS_EventType_Null;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
OS_BeginFrame(void)
{
    os->pump_events = 0;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
OS_EndFrame(void)
{
    os->current_time += 1.f / os->target_frames_per_second;
}

// NOTE(rjf): Only called by platform layers. Do not call in app.
internal void
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

internal OS_ArenaNode*
_OS_GetFreeScratch(OS_ThreadContext *tctx)
{
    OS_ArenaNode *usable_node = tctx->free;
    Assert(usable_node != 0);
    SLLStackPop(tctx->free);
    DLLPushBack(tctx->first_used, tctx->last_used, usable_node);
    return(usable_node);
}

internal M_Arena*
_OS_MarkArenaRestore(OS_ArenaNode *node)
{
    M_Arena *result = &node->arena;
    OS_ArenaInlineRestore *restore = PushArray(result, OS_ArenaInlineRestore, 1);
    SLLStackPush(node->restore, restore);
    node->ref_count += 1;
    return(result);
}

internal M_Arena*
OS_GetScratch(void)
{
    OS_ThreadContext *tctx = os->GetThreadContext();
    OS_ArenaNode *usable_node = tctx->first_used;
    if (usable_node == 0)
    {
        usable_node = _OS_GetFreeScratch(tctx);
    }
    return(_OS_MarkArenaRestore(usable_node));
}

internal M_Arena*
OS_GetScratch1(M_Arena *a1)
{
    OS_ThreadContext *tctx = os->GetThreadContext();
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

internal M_Arena*
OS_GetScratch2(M_Arena *a1, M_Arena *a2)
{
    OS_ThreadContext *tctx = os->GetThreadContext();
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

internal void
OS_ReleaseScratch(M_Arena *arena)
{
    OS_ArenaNode *node = CastFromMember(OS_ArenaNode, arena, arena);
    Assert(node->ref_count > 0);
    node->ref_count -= 1;
    if (node->ref_count == 0)
    {
        OS_ThreadContext *tctx = os->GetThreadContext();
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

internal void
_OS_ThreadSaveFileLine(char *file_name, u64 line_number)
{
    OS_ThreadContext *tctx = os->GetThreadContext();
    tctx->file_name = file_name;
    tctx->line_number = line_number;
}

#define OS_ThreadSaveFileLine() _OS_ThreadSaveFileLine(__FILE__, __LINE__)

internal OS_File_Line
OS_ThreadRememberFileLine(void)
{
    OS_ThreadContext *tctx = os->GetThreadContext();
    OS_File_Line result = {tctx->file_name, tctx->line_number};
    return(result);
}



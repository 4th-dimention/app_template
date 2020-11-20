// NOTE(allen): memory

#define M_ARENA_MAX          GB(4)
#define M_ARENA_COMMIT_SIZE  KB(4)

function M_Arena
M_ArenaInitializeWithAlign(U64 auto_align)
{
    M_Arena arena = {0};
    arena.max = M_ARENA_MAX;
    arena.base = os->Reserve(arena.max);
    arena.alloc_position = 0;
    arena.commit_position = 0;
    arena.auto_align = auto_align;
    return arena;
}

function M_Arena
M_ArenaInitialize(void)
{
    return(M_ArenaInitializeWithAlign(8));
}

function void *
M_ArenaPush(M_Arena *arena, U64 size)
{
    void *memory = 0;
    if(arena->alloc_position + size > arena->commit_position)
    {
        U64 commit_size = size;
        commit_size += M_ARENA_COMMIT_SIZE-1;
        commit_size -= commit_size % M_ARENA_COMMIT_SIZE;
        os->Commit((U8 *)arena->base + arena->commit_position, commit_size);
        arena->commit_position += commit_size;
    }
    memory = (U8 *)arena->base + arena->alloc_position;
    U64 p = arena->alloc_position + size;
    arena->alloc_position = (p + arena->auto_align - 1)&(~(arena->auto_align - 1));
    return memory;
}

function void *
M_ArenaPushZero(M_Arena *arena, U64 size)
{
    void *memory = M_ArenaPush(arena, size);
    MemorySet(memory, 0, size);
    return memory;
}

function void
M_ArenaSetPosBack(M_Arena *arena, U64 pos)
{
    if (pos <= arena->alloc_position)
    {
        arena->alloc_position = pos;
    }
}

function void
M_ArenaSetPosBackByPtr(M_Arena *arena, void *ptr)
{
    U8 *uptr = (U8*)ptr;
    U64 pos = (uptr - (U8*)arena->base);
    if ((U8*)arena->base <= uptr)
    {
        M_ArenaSetPosBack(arena, pos);
    }
}

function void
M_ArenaPop(M_Arena *arena, U64 size)
{
    size = ClampTop(size, arena->alloc_position);
    arena->alloc_position -= size;
}

function void
M_ArenaClear(M_Arena *arena)
{
    M_ArenaPop(arena, arena->alloc_position);
}

function void
M_ArenaRelease(M_Arena *arena)
{
    os->Release(arena->base);
}

function M_Temp
M_BeginTemp(M_Arena *arena)
{
    M_Temp temp = {arena, arena->alloc_position};
    return(temp);
}

function void
M_EndTemp(M_Temp temp)
{
    M_ArenaSetPosBack(temp.arena, temp.pos);
}


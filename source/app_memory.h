// NOTE(allen): memory

typedef struct M_Arena M_Arena;
struct M_Arena
{
    void *base;
    u64 max;
    u64 alloc_position;
    u64 commit_position;
    u64 auto_align;
};

typedef struct M_Temp M_Temp;
struct M_Temp
{
    M_Arena *arena;
    u64 pos;
};

////////////////////////////////

#define PushArray(arena,T,c) ( (T*)(M_ArenaPush((arena),sizeof(T)*(c))) )
#define PushArrayZero(arena,T,c) ( (T*)(M_ArenaPushZero((arena),sizeof(T)*(c))) )

internal M_Arena M_ArenaInitializeWithAlign(u64 auto_align);
internal M_Arena M_ArenaInitialize(void);
internal void    M_ArenaRelease(M_Arena *arena);

internal void* M_ArenaPush(M_Arena *arena, u64 size);
internal void* M_ArenaPushZero(M_Arena *arena, u64 size);
internal void  M_ArenaSetPosBack(M_Arena *arena, u64 pos);
internal void  M_ArenaSetPosBackByPtr(M_Arena *arena, void *ptr);
internal void  M_ArenaPop(M_Arena *arena, u64 size);
internal void  M_ArenaClear(M_Arena *arena);

internal M_Temp M_BeginTemp(M_Arena *arena);
internal void   M_EndTemp(M_Temp temp);


////////////////////////////////
// NOTE(allen): Memory Types

typedef struct M_Arena M_Arena;
struct M_Arena
{
    void *base;
    U64 max;
    U64 alloc_position;
    U64 commit_position;
    U64 auto_align;
};

typedef struct M_Temp M_Temp;
struct M_Temp
{
    M_Arena *arena;
    U64 pos;
};

////////////////////////////////
// NOTE(allen): Memory Functions

#define PushArray(arena,T,c) ( (T*)(M_ArenaPush((arena),sizeof(T)*(c))) )
#define PushArrayZero(arena,T,c) ( (T*)(M_ArenaPushZero((arena),sizeof(T)*(c))) )

function M_Arena M_ArenaInitializeWithAlign(U64 auto_align);
function M_Arena M_ArenaInitialize(void);
function void    M_ArenaRelease(M_Arena *arena);

function void* M_ArenaPush(M_Arena *arena, U64 size);
function void* M_ArenaPushZero(M_Arena *arena, U64 size);
function void  M_ArenaSetPosBack(M_Arena *arena, U64 pos);
function void  M_ArenaSetPosBackByPtr(M_Arena *arena, void *ptr);
function void  M_ArenaPop(M_Arena *arena, U64 size);
function void  M_ArenaClear(M_Arena *arena);

function M_Temp M_BeginTemp(M_Arena *arena);
function void   M_EndTemp(M_Temp temp);


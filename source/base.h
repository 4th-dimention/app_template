////////////////////////////////
//~ NOTE(rjf): C Standard Library

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MemoryCopy(d,s,z) memmove(d,s,z)
#define MemoryCopyStruct(d,s) MemoryCopy(d,s,sizeof(*(d)))

#define MemorySet(s,v,z) memset(s,v,z)
#define MemoryZero(s,z)  memset(s,0,z)
#define MemoryZeroStruct(s) MemoryZero(s,sizeof(*(s)))
#define MemoryZeroArray(a)  memset(a,0,sizeof(a))

#define CalculateCStringLength      (U32)strlen
#define CStringToI32(s)            ((S32)atoi(s))
#define CStringToI16(s)            ((S16)atoi(s))
#define CStringToF32(s)            ((F32)atof(s))

////////////////////////////////
//~ NOTE(rjf): Base Types

typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef S8       B8;
typedef S16      B16;
typedef S32      B32;
typedef S64      B64;
typedef float    F32;
typedef double   F64;

////////////////////////////////
//~ NOTE(rjf): Helper Macros

#define global         static
#define function       static
#define local          static

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define KB(n)  (((U64)(n)) << 10)
#define MB(n)  (((U64)(n)) << 20)
#define GB(n)  (((U64)(n)) << 30)
#define TB(n)  (((U64)(n)) << 40)

#define Thousand(n)   ((n)*1000)
#define Million(n)    ((n)*1000000)
#define Billion(n)    ((n)*1000000000)

#define PI (3.1415926535897f)

#define Stmnt(S) do{ S }while(0)

#define AssertBreak() (*(int*)0 = 0xABCD)
#if SHIP_MODE
#define Assert(c)
#else
#define Assert(c) Stmnt( if (!(c)){ AssertBreak(); } )
#endif
#define AssertImplies(a,b) Assert(!(a) || (b))
#define InvalidPath Assert(!"Invalid Path")
#define NotImplemented Assert(!"Not Implemented")

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A,B) A##B
#define Glue(A,B) Glue_(A,B)

#define Min(A,B) ( ((A)<(B))?(A):(B) )
#define Max(A,B) ( ((A)>(B))?(A):(B) )

#define ClampTop(A,X) Min(A,X)
#define ClampBot(X,B) Max(X,B)
#define Clamp(A,X,B) ( ((X)<(A))?(A):((B)>(X))?(B):(X) )

#define Swap(T,a,b) Stmnt( T t__ = a; a = b; b = t__; )

#define IntFromPtr(ptr) ((U64)(ptr))
#define PtrFromInt(i) (void*)((U8*)0 + (i))

#define Member(T,m) (((T*)0)->m)
#define OffsetOf(T,m) IntFromPtr(&Member(T,m))
#define CastFromMember(T,m,ptr) (T*)(((U8*)ptr) - OffsetOf(T,m))

////////////////////////////////
//~ NOTE(allen): Linked Lists

#define DLLMembers(type,next,prev,sib) union{ struct{ type prev; type next; }; type sib[2]; };

#define DLLPushBack_NP(f,l,n,next,prev) ( (f)==0?\
((f)=(l)=(n),(n)->next=(n)->prev=0):\
((l)->next=(n),(n)->prev=(l),(l)=(n),(n)->next=0) )

#define DLLPushFront_NP(f,l,n,next,prev) DLLPushBack_NP(l,f,n,prev,next)

#define DLLPushBack(f,l,n) DLLPushBack_NP(f,l,n,next,prev)
#define DLLPushFront(f,l,n) DLLPushFront_NP(f,l,n,next,prev)


#define DLLInsertAfter_NP(f,l,p,n,next,prev) ( ((l)==(p))?\
DLLPushBack_NP(f,l,n,next,prev):\
((n)->prev=(p),(n)->next=(p)->next,(p)->next->prev=(n),(p)->next=(n)) )
#define DLLInsertBefore_NP(f,l,p,n,next,prev) DLLInsertAfter_NP(l,f,p,n,prev,next)

#define DLLInsertAfter(f,l,p,n) DLLInsertAfter_NP(f,l,p,n,next,prev)
#define DLLInsertBefore(f,l,p,n) DLLInsertBefore_NP(f,l,p,n,next,prev)

#define DLLRemoveFirst_NP(f,l,next,prev) ( ((f)==(l))?\
(f)=(l)=0:\
((f)=(f)->next,(f)->prev=0) )
#define DLLRemoveLast_NP(f,l,next,prev) DLLRemoveFirst_NP(l,f,prev,next)
#define DLLRemove_NP(f,l,n,next,prev) ( ((f)==(n))?\
DLLRemoveFirst_NP(f,l,next,prev):\
((f)==(l))?\
DLLRemoveLast_NP(f,l,next,prev):\
((n)->next->prev=(n)->prev,(n)->prev->next=(n)->next) )

#define DLLRemove(f,l,n) DLLRemove_NP(f,l,n,next,prev)



#define SLLQueuePush_N(f,l,n,next) ( (f)==0?\
((f)=(l)=(n),(n)->next=0):\
((l)->next=(n),(n)->next=0) )
#define SLLQueuePushFront_N(f,l,n,next) ( (f)==0?\
((f)=(l)=(n),(n)->next=0):\
((n)->next=(f),(f)=(n)) )
#define SLLQueuePop_N(f,l,next) ( (f)==(l)?\
(f)=(l)=0:\
((f)=(f)->next) )

#define SLLQueuePush(f,l,n) SLLQueuePush_N(f,l,n,next)
#define SLLQueuePushFront(f,l,n) SLLQueuePushFront_N(f,l,n,next)
#define SLLQueuePop(f,l) SLLQueuePop_N(f,l,next)



#define SLLStackPush_N(f,n,next) ( (n)->next=(f), (f)=(n) )
#define SLLStackPop_N(f,next) ( (f)=(f)->next )

#define SLLStackPush(f,n) SLLStackPush_N(f,n,next)
#define SLLStackPop(f) SLLStackPop_N(f,next)


////////////////////////////////
//~ NOTE(allen): Constants

typedef enum
{
    Dimension_X,
    Dimension_Y,
    Dimension_Z,
    Dimension_W,
} Dimension;

typedef enum
{
    Side_Min,
    Side_Max,
} Side;

typedef enum
{
    OperatingSystem_Windows,
    OperatingSystem_Linux,
    OperatingSystem_Mac,
    OperatingSystem_COUNT,
} OperatingSystem;

typedef enum
{
    Architecture_x86,
    Architecture_x64,
    Architecture_arm,
    Architecture_arm64,
    Architecture_COUNT,
} Architecture;

typedef enum
{
    Month_January,
    Month_February,
    Month_March,
    Month_April,
    Month_May,
    Month_June,
    Month_July,
    Month_August,
    Month_September,
    Month_October,
    Month_November,
    Month_December,
} Month;

typedef enum
{
    DayOfWeek_Sunday,
    DayOfWeek_Monday,
    DayOfWeek_Tuesday,
    DayOfWeek_Wednesday,
    DayOfWeek_Thursday,
    DayOfWeek_Friday,
    DayOfWeek_Saturday,
} DayOfWeek;

////////////////////////////////
//~ NOTE(allen): Vectors

typedef union V2F32 V2F32;
union V2F32
{
    struct { F32 x; F32 y; };
    float v[2];
};

typedef union V3F32 V3F32;
union V3F32
{
    struct { F32 x; F32 y; F32 z; };
    F32 v[3];
};

typedef union V4F32 V4F32;
union V4F32
{
    struct { F32 x; F32 y; F32 z; F32 w; };
    F32 v[4];
};

typedef union V2S32 V2S32;
union V2S32
{
    struct { S32 x; S32 y; };
    S32 v[2];
};

typedef union V3S32 V3S32;
union V3S32
{
    struct { S32 x; S32 y; S32 z; };
    S32 v[3];
};

typedef union V4S32 V4S32;
union V4S32
{
    struct { S32 x; S32 y; S32 z; S32 w; };
    S32 v[4];
};

////////////////////////////////
//~ NOTE(allen): Matrix

typedef struct M4x4F32 M4x4F32;
struct M4x4F32
{
    F32 v[4][4];
};

////////////////////////////////
//~ NOTE(allen): Interval

typedef union RangeF32 RangeF32;
union RangeF32
{
    struct { F32 min; F32 max; };
    F32 v[2];
};

typedef union RangeU64 RangeU64;
union RangeU64
{
    struct { U64 min; U64 max; };
    U64 v[2];
};

typedef union RectF32 RectF32;
union RectF32
{
    struct{ F32 x0; F32 y0; F32 x1; F32 y1; };
    F32 v[4];
    struct{ V2F32 p0; V2F32 p1; };
    V2F32 p[2];
};

////////////////////////////////
//~ NOTE(allen): String

typedef struct String8 String8;
struct String8
{
    union
    {
        U8 *string;
        U8 *str;
        void *data;
        void *ptr;
    };
    U64 size;
};

#define str8_lit(s) str8((U8*)(s), ArrayCount(s) - 1)
#define str8_comp(s) {(U8*)(s), ArrayCount(s) - 1}
#define str8_expand(s) (S32)((s).size), ((s).str)

typedef U32 StringMatchFlags;
enum
{
    StringMatchFlag_MatchCase       = (1<<0),
    StringMatchFlag_RightSideSloppy = (1<<1),
};

typedef struct String8_Node String8_Node;
struct String8_Node
{
    String8_Node *next;
    String8 string;
};

typedef struct String8_List String8_List;
struct String8_List
{
    String8_Node *first;
    String8_Node *last;
    U64 total_size;
    U64 node_count;
};

typedef struct String_Join String_Join;
struct String_Join
{
    String8 pre;
    String8 sep;
    String8 post;
};

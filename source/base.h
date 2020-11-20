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

#define CalculateCStringLength (u32)strlen
#define CStringToI32(s)            ((i32)atoi(s))
#define CStringToI16(s)            ((i16)atoi(s))
#define CStringToF32(s)            ((f32)atof(s))

////////////////////////////////
//~ NOTE(rjf): Base Types

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef i8       b8;
typedef i16      b16;
typedef i32      b32;
typedef i64      b64;
typedef float    f32;
typedef double   f64;

////////////////////////////////
//~ NOTE(rjf): Helper Macros

#define global         static
#define internal       static
#define local_persist  static
#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define Bytes(n)      ((u64)(n))
#define Kilobytes(n)  (((u64)(n)) << 10)
#define Megabytes(n)  (((u64)(n)) << 20)
#define Gigabytes(n)  (((u64)(n)) << 30)
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

#define IntFromPtr(ptr) ((u64)(ptr))
#define PtrFromInt(i) (void*)((u8*)0 + (i))

#define Member(T,m) (((T*)0)->m)
#define OffsetOf(T,m) IntFromPtr(&Member(T,m))
#define CastFromMember(T,m,ptr) (T*)(((u8*)ptr) - OffsetOf(T,m))

////////////////////////////////
//~ NOTE(allen): Linked Lists

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

////////////////////////////////
//~ NOTE(allen): Vectors

typedef union v2 v2;
union v2
{
    struct
    {
        f32 x;
        f32 y;
    };
    
    struct
    {
        f32 width;
        f32 height;
    };
    
    float elements[2];
    float v[2];
};

typedef union v3 v3;
union v3
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
    };
    
    struct
    {
        f32 r;
        f32 g;
        f32 b;
    };
    
    f32 elements[3];
    f32 v[3];
};

typedef union v4 v4;
union v4
{
    struct
    {
        f32 x;
        f32 y;
        union
        {
            struct
            {
                f32 z;
                
                union
                {
                    f32 w;
                    f32 radius;
                };
            };
            struct
            {
                f32 width;
                f32 height;
            };
        };
    };
    
    struct
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    
    f32 elements[4];
    f32 v[4];
};

typedef union iv2 iv2;
union iv2
{
    struct
    {
        i32 x;
        i32 y;
    };
    
    struct
    {
        i32 width;
        i32 height;
    };
    
    i32 elements[2];
    i32 v[2];
};

typedef union iv3 iv3;
union iv3
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
    };
    
    struct
    {
        i32 r;
        i32 g;
        i32 b;
    };
    
    i32 elements[3];
    i32 v[3];
};

typedef union iv4 iv4;
union iv4
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
        i32 w;
    };
    
    struct
    {
        i32 r;
        i32 g;
        i32 b;
        i32 a;
    };
    
    i32 elements[4];
    i32 v[4];
};

#define v2(...)   (v2){ __VA_ARGS__ }
#define v3(...)   (v3){ __VA_ARGS__ }
#define v4(...)   (v4){ __VA_ARGS__ }
#define iv2(...) (iv2){ __VA_ARGS__ }
#define iv3(...) (iv3){ __VA_ARGS__ }
#define iv4(...) (iv4){ __VA_ARGS__ }

////////////////////////////////
//~ NOTE(allen): Matrix

typedef struct m4 m4;
struct m4
{
    f32 elements[4][4];
};

////////////////////////////////
//~ NOTE(allen): Interval

typedef union Range Range;
union Range
{
    struct
    {
        f32 min;
        f32 max;
    };
    f32 v[2];
};

typedef union Rangei Rangei;
union Rangei
{
    struct{
        i64 min;
        i64 max;
    };
    i64 v[2];
};

typedef union Rangeu Rangeu;
union Rangeu
{
    struct{
        u64 min;
        u64 max;
    };
    u64 v[2];
};

typedef union Rect Rect;
union Rect
{
    struct{
        f32 x0;
        f32 y0;
        f32 x1;
        f32 y1;
    };
    struct{
        v2 p0;
        v2 p1;
    };
    f32 v[4];
    v2 p[2];
};

////////////////////////////////
//~ NOTE(allen): String

typedef struct String8 String8;
struct String8
{
    union
    {
        u8 *string;
        u8 *str;
        void *data;
        void *ptr;
    };
    u64 size;
};

#define S8Lit(s) S8((u8*)(s), ArrayCount(s) - 1)
#define S8LitComp(s) {(u8*)(s), ArrayCount(s) - 1}
#define StringExpand(s) (int)((s).size), ((s).str)

typedef u32 StringMatchFlags;
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
    u64 total_size;
    u64 node_count;
};

typedef struct String_Join String_Join;
struct String_Join
{
    String8 pre;
    String8 sep;
    String8 post;
};

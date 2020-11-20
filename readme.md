# Overview
Version on: July 3rd 2020 2:30pm

This layer is meant to be copy-pasted into a fresh project to start new applications. It comes with several pre-built systems common at the beginning of any application development.

Target Support List
- OS: Win32
- Renderer: OpenGL

Main Feature Support List
- Input processing from mouse and keyboard
- Render mono-color shapes and textures

See `app.c` and `app.h` for the main "hello world" example program.

The rest of this doc will try to briefly outline the concept of each section with a few samples of the sort of API's in that section to give you an idea of the sort of features you will find there. The only exhaustive list of features is in the code itself.


# Base
The base establishes common types, macros, and functions for the template.
These are in the files `base.h` and `base.c`

## Base Types
Convenient integer and float type names:
`S8, ... S64, U8, ... U64, B8, ... B64, F32, F64`

## Helper Macros
Macros that get used all over the place such as:
- `#define function static` for functions
- `#define ArrayCount(c) ...` for static array counts
- `#define GB(n) ...`
- `#define Assert(c) ...`
- `#define Min(a,b) ...`
- `#define Swap(T,a,b) ...`
- `#define IntFromPtr(ptr) ...`

## Linked Lists
Macros for conveniently managing linked lists

- `DLL` macros deal with doubly linked lists
- `SLLQueue` macros deal with singly linked lists in queue order
- `SLLStack` macros deal with singly linked lists in stack order

Most linked list macros have an equivalent version that allow you to alter the names for `next` and `prev` pointers. For example

- `#define DLLPushBack(f,l,n) ...` pushes a node `n` onto the list formed by `f` `l` the types of the arguments
must match and contain a `next` and `prev` pointer.
- `#define DLLPushBack_NP(f,l,n,next,prev) ...` still requires the pointers to exist, now you can supply the names

## Constants
A few enums for universal names of various concepts such as.

```C
typedef enum
{
    Dimension_X,
    Dimension_Y,
    Dimension_Z,
    Dimension_W,
} Dimension;
```

## Scalar
A handful of scalar math functions, many simply wrapping `<math.h>`

- `AbsoluteValue`
- `SquareRoot`
- `Sin`
- `S32Ceil` floating point ceiling, evaluates to `S32`
- `F32Ceil` floating point ceiling, evaluates to `F32`
- `Lerp(a,t,b)`

## Vector
Small fixed-size array types with helpful accessors and math functions.

`V2F32, V3F32, V4F32` are the floating point vectors

`V2S32, V3S32, V4S32` are the integer vectors

These can be constrcted via constructors:

`V2F32 p = v2F32(x, y)`

The floating point vectors have math functions such as:
- `V2Add`
- `V2Sub`
- `V2Mul`
- `V2Dot`
- `V2Hadamard`
- `V2Length`
- `V2Normalize`

## Matrix
A 4x4 floating point matrix with all sorts of miscellanious functions.

- `M4MultiplyM4`
- `M4TranslateV3`
- `M4ScaleV3`
- `M4Perspective`
- `M4Inverse`

## Color
RGB <-> HSV conversions

- `RGBToHSV`
- `HSVToRGB`

## Interval
Types for one dimensional intervals and axis aligned rectangles

`RangeF32` floating point interval

`RangeU64` unsigned integer interval

Ranges have `min` and `max` and by convention encode the partially open interval `[min,max)`
although sometimes for floats it is also useful to use `(min,max)` or `[min,max]`.

There are various functions on ranges
- `RangeSize`
- `RangeUnion`
- `RangeGrow`
- `RangeOverlaps`

`Rect` is a floating point rectangle encoded as an range on two axes.

Rectangles contain `x0` `y0` `x1` `y1` plus several other union-ed accessors like `p0` `p1`

- `MakeRect(x0,y0,x1,y1)`
- `MakeRectVec(p0,p1)`
- `MakeRectRanges(x,y)`

Anything a range can do, a rectangle can also do by applying the logic on each axis lifted or folded:

- `RectUnion`
- `RectGrow`
- `RectOverlaps`

## String
The lengthed string type, and helper types and functions.

```C
struct String8
{
    U8 *str;
    U64 size;
};
```

There are helpers on single characters:
- `CharIsAlpha`
- `CharIsSymbol`
- `CharToLower`

Constructors for `String8`:
- `str8(str,size)`
- `str8_lit("")`
- `String8FromCString(str)`
- `StringSubstring(string,range)`

Predicates for `String8`:
- `StringMatch`

Format string based `String8` constructors (with an arena):
- `PushStringF(arena, fmt, ...)`

There are string lists, which are simple linked lists of strings with helper functions for constructing and then linearizing them:
- `StringListPush`
- `StringListPushF`
- `StringListJoin`

## Additional String Stuff
A hard to categorize assemblence of various string functions - possible to ignore but has some neat stuff too.


# OS
The OS layer establishes the main types and functions for dealing with user input events, low level memory allocation, graphics, files, time, threads, etc.

The list of primary functions can be found in `os.h` in the `OS_State` struct, which is available everywhere through a global `OS_State *os` pointer.  In `os.c` there are additional wrappers and helpers for these functions.

## Events
The event handling is done through a "waterfall" architecture. Each frame a list of events is made available by the platform layer implementation, and the application can query for and remove events by type in any order.

Functions for this system include:
- `KeyName`
- `OS_GetNextEvent`
- `OS_EatEvent`

The intended use goes:
```C
OS_Event *event = 0;
for (;OS_GetNextEvent(&event);)
{
    if (event->type == ...)
    {
        ...
        OS_EatEvent(event);
    }
}
```

## Thread Context
The template does not supply any multi-threading systems, but it does supply a thread context pointer. The thread context pointer provides a scratch memory pool, and easy passing of debug information through otherwise non-debug related functions.

The functions for getting scratch memory return scratch memory as an arena pointer. The intended use is usually to close a scratch arena pointer at the end of any scope that opened it. For arenas that persist longer, a new arena should be constructed and it's lifetime managed manually instead.

- `OS_GetScratch`
- `OS_ReleaseScratch`
- `OS_ThreadSaveFileLine` remembers the `__FILE__,__LINE__` where it is invoked and saves it to the thread context.
- `OS_ThreadRememberFileLine` recalls the most recently saved `__FILE__,__LINE__`


# Memory
In `app_memory.h` and `app_memory.c`, the only built in memory management primitive is `M_Arena`, which uses contiguous linear allocations, with excessively large pre-reserved memory regions and commits to grow as needed.

They can be created:
- `M_AreaInitialize`

Allocations can be made:
- `M_ArenaPush`
- `PushArray`

They can be freed, but strictly in reverse order:
- `M_ArenaPop`
- `M_BeginTemp`
- `M_EndTemp`

Their initial memory reserve and commits can be fully released:
- `M_ArenaRelease`


# Render
The render system is a basic OpenGL 3.3 implementation of mono-chrome polygon shapes and mono-chrome texturing. It supplies primitive batching, a single shader, font rasterization and metrics through `stb_truetype.h`, and a couple of starter draw primitives.

Begin and end each frame:
- `R_Begin`
- `R_End`

Initialize fonts, set custom bitmaps into the font texture and metrics data, get string metrics:
- `R_InitFont`
- `R_FontSetSlot`
- `R_StringDimWithFont`

Select a font into the render context and render with it:
- `R_SelectFont`
- `R_String`

Draw basic shapes:
- `R_Rect`
- `R_RectOutline`


# App
The main app file needs to include the `.h` and `.c` files, and define four main implementation functions which are exposed through macros:

`APP_PERMANENT_LOAD` is called once to initialize the application
This should include a call to `R_Init` to initialize the rendering system.


`APP_HOT_LOAD` is called whenever the application layer `.dll` is reloaded

`APP_HOT_UNLOAD` is called whenever the application layer `.dll` is about to be unloaded

`APP_UPDATE` is called once per frame of the application
This is where you may call `R_Begin` `R_End` and any draw calls, and process events from the OS layer.


# Info
`readme.md` by Allen Webster (allenwebster@4coder.net)

App Template by Ryan Fleury with extensions by Allen Webster

This documentation is provided as is without warranty, express or implied, of suitability for a particular purpose.

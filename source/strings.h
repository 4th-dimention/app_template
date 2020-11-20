
//~ NOTE(rjf): String Manipulation

#define CharIsSpace(c) ((c) <= 32)
#define CharIsDigit CharIsNumeric

function b32
CharIsAlpha(char c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}

function b32
CharIsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

function b32
CharIsSymbol(char c)
{
    return (c == '~' ||
            c == '!' ||
            c == '%' ||
            c == '^' ||
            c == '&' ||
            c == '*' ||
            c == '(' ||
            c == ')' ||
            c == '[' ||
            c == ']' ||
            c == '{' ||
            c == '}' ||
            c == '-' ||
            c == '+' ||
            c == '=' ||
            c == ';' ||
            c == ':' ||
            c == '<' ||
            c == '>' ||
            c == '/' ||
            c == '?' ||
            c == '.' ||
            c == ',');
}

function char
CharToLower(char c)
{
    if(c >= 'A' && c <= 'Z')
    {
        return c + 32;
    }
    return c;
}

function char
CharToUpper(char c)
{
    if(c >= 'a' && c <= 'z')
    {
        return c - 32;
    }
    return c;
}

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

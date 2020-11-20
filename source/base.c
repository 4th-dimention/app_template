////////////////////////////////
// NOTE(allen): Scalar

#define FMod fmodf
#define AbsoluteValue fabsf
#define SquareRoot sqrtf
#define Sin sinf
#define Cos cosf
#define Tan tanf

function S32
S32Ceil(F32 x)
{
    if (x > 0)
    {
        return(((S32) x) + 1);
    }
    else
    {
        return(((S32) x));
    }
}

function S32
S32Floor(F32 x)
{
    if (x > 0)
    {
        return(((S32) x));
    }
    else
    {
        return(((S32) x) - 1);
    }
}

function F32
F32Ceil(F32 x)
{
    if (x > 0)
    {
        return((F32) ((S32) x) + 1);
    }
    else
    {
        return((F32) ((S32) x));
    }
}

function F32
F32Floor(F32 x)
{
    if (x > 0)
    {
        return((F32) ((S32) x));
    }
    else
    {
        return((F32) ((S32) x) - 1);
    }
}

function F32
Lerp(F32 a, F32 t, F32 b)
{
    return(a + (b - a)*t);
}

////////////////////////////////
// NOTE(allen): vector

#define V2Expand(v) ((v).x), ((v).y)
#define V3Expand(v) ((v).x), ((v).y), ((v).z)
#define V4Expand(v) ((v).x), ((v).y), ((v).z), ((v).w)

function V2F32
v2F32(F32 x, F32 y){
    V2F32 r = {x,y};
    return(r);
}

function V3F32
v3F32(F32 x, F32 y, F32 z){
    V3F32 r = {x,y,z};
    return(r);
}

function V4F32
v4F32(F32 x, F32 y, F32 z, F32 w){
    V4F32 r = {x,y,z,w};
    return(r);
}

function V2F32
V2Add(V2F32 a, V2F32 b)
{
    V2F32 c = { a.x + b.x, a.y + b.y };
    return c;
}
function V3F32
V3Add(V3F32 a, V3F32 b)
{
    V3F32 c = { a.x + b.x, a.y + b.y, a.z + b.z };
    return c;
}
function V4F32
V4Add(V4F32 a, V4F32 b)
{
    V4F32 c = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    return c;
}

function V2F32
V2Sub(V2F32 a, V2F32 b)
{
    V2F32 c = { a.x - b.x, a.y - b.y };
    return c;
}
function V3F32
V3Sub(V3F32 a, V3F32 b)
{
    V3F32 c = { a.x - b.x, a.y - b.y, a.z - b.z };
    return c;
}
function V4F32
V4Sub(V4F32 a, V4F32 b)
{
    V4F32 c = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    return c;
}

function V2F32
V2Mul(V2F32 v, F32 f)
{
    v.x *= f;
    v.y *= f;
    return v;
}
function V3F32
V3Mul(V3F32 v, F32 f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;
    return v;
}
function V4F32
V4Mul(V4F32 v, F32 f)
{
    v.x *= f;
    v.y *= f;
    v.z *= f;
    v.w *= f;
    return v;
}

function F32
V2Dot(V2F32 a, V2F32 b)
{
    return a.x*b.x + a.y*b.y;
}
function F32
V3Dot(V3F32 a, V3F32 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
function F32
V4Dot(V4F32 a, V4F32 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

function V2F32
V2Hadamard(V2F32 a, V2F32 b)
{
    V2F32 v = {a.x*b.x, a.y*b.y};
    return(v);
}
function V3F32
V3Hadamard(V3F32 a, V3F32 b)
{
    V3F32 v = {a.x*b.x, a.y*b.y, a.z*b.z};
    return(v);
}
function V4F32
V4Hadamard(V4F32 a, V4F32 b)
{
    V4F32 v = {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};
    return(v);
}

function F32
V2Length(V2F32 a)
{
    return SquareRoot(V2Dot(a,a));
}
function F32
V3Length(V3F32 a)
{
    return SquareRoot(V3Dot(a,a));
}
function F32
V4Length(V4F32 a)
{
    return SquareRoot(V4Dot(a,a));
}

function V2F32
V2Normalize(V2F32 v)
{
    F32 inv_length = 1.f/V2Length(v);
    V2F32 result = { v.x*inv_length, v.y*inv_length, };
    return result;
}
function V3F32
V3Normalize(V3F32 v)
{
    F32 inv_length = 1.f/V3Length(v);
    V3F32 result = { v.x*inv_length, v.y*inv_length, v.z*inv_length, };
    return result;
}
function V4F32
V4Normalize(V4F32 v)
{
    F32 inv_length = 1.f/V4Length(v);
    V4F32 result = { v.x*inv_length, v.y*inv_length, v.z*inv_length, v.w*inv_length, };
    return result;
}

function V3F32
V3Cross(V3F32 a, V3F32 b)
{
    V3F32 result =
    {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x,
    };
    return result;
}

////////////////////////////////
// NOTE(allen): matrix

function M4x4F32
M4InitD(F32 diagonal)
{
    M4x4F32 m =
    {
        {
            { diagonal                },
            { 0.f, diagonal           },
            { 0.f, 0.f, diagonal      },
            { 0.f, 0.f, 0.f, diagonal },
        }
    };
    return m;
}

function M4x4F32
M4MultiplyM4(M4x4F32 a, M4x4F32 b)
{
    M4x4F32 c = {0};
    
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
        {
            c.v[i][j] = (a.v[0][j]*b.v[i][0] +
                         a.v[1][j]*b.v[i][1] +
                         a.v[2][j]*b.v[i][2] +
                         a.v[3][j]*b.v[i][3]);
        }
    }
    
    return c;
}

function M4x4F32
M4MultiplyF32(M4x4F32 a, F32 b)
{
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
        {
            a.v[i][j] *= b;
        }
    }
    
    return a;
}

function V4F32
V4MultiplyM4(V4F32 v, M4x4F32 m)
{
    V4F32 result = {0};
    
    for(int i = 0; i < 4; ++i)
    {
        result.v[i] = (v.v[0]*m.v[0][i] +
                       v.v[1]*m.v[1][i] +
                       v.v[2]*m.v[2][i] +
                       v.v[3]*m.v[3][i]);
    }
    
    return result;
}

function M4x4F32
M4TranslateV3(V3F32 translation)
{
    M4x4F32 result = M4InitD(1.f);
    result.v[3][0] = translation.x;
    result.v[3][1] = translation.y;
    result.v[3][2] = translation.z;
    return result;
}

function M4x4F32
M4ScaleV3(V3F32 scale)
{
    M4x4F32 result = M4InitD(1.f);
    result.v[0][0] = scale.x;
    result.v[1][1] = scale.y;
    result.v[2][2] = scale.z;
    return result;
}

function M4x4F32
M4Perspective(F32 fov, F32 aspect_ratio, F32 near_z, F32 far_z)
{
    M4x4F32 result = {0};
    F32 tan_theta_over_2 = Tan(fov * (PI / 360.f));
    result.v[0][0] = 1.f / tan_theta_over_2;
    result.v[1][1] = aspect_ratio / tan_theta_over_2;
    result.v[2][3] = -1.f;
    result.v[2][2] = (near_z + far_z) / (near_z - far_z);
    result.v[3][2] = (2.f * near_z * far_z) / (near_z - far_z);
    result.v[3][3] = 0.f;
    return result;
}

function M4x4F32
M4LookAt(V3F32 eye, V3F32 center, V3F32 up)
{
    M4x4F32 result;
    
    V3F32 f = V3Normalize(V3Sub(center, eye));
    V3F32 s = V3Normalize(V3Cross(f, up));
    V3F32 u = V3Cross(s, f);
    
    result.v[0][0] = s.x;
    result.v[0][1] = u.x;
    result.v[0][2] = -f.x;
    result.v[0][3] = 0.0f;
    
    result.v[1][0] = s.y;
    result.v[1][1] = u.y;
    result.v[1][2] = -f.y;
    result.v[1][3] = 0.0f;
    
    result.v[2][0] = s.z;
    result.v[2][1] = u.z;
    result.v[2][2] = -f.z;
    result.v[2][3] = 0.0f;
    
    result.v[3][0] = -V3Dot(s, eye);
    result.v[3][1] = -V3Dot(u, eye);
    result.v[3][2] = V3Dot(f, eye);
    result.v[3][3] = 1.0f;
    
    return result;
}

function M4x4F32
M4Inverse(M4x4F32 m)
{
    F32 coef00 = m.v[2][2] * m.v[3][3] - m.v[3][2] * m.v[2][3];
    F32 coef02 = m.v[1][2] * m.v[3][3] - m.v[3][2] * m.v[1][3];
    F32 coef03 = m.v[1][2] * m.v[2][3] - m.v[2][2] * m.v[1][3];
    F32 coef04 = m.v[2][1] * m.v[3][3] - m.v[3][1] * m.v[2][3];
    F32 coef06 = m.v[1][1] * m.v[3][3] - m.v[3][1] * m.v[1][3];
    F32 coef07 = m.v[1][1] * m.v[2][3] - m.v[2][1] * m.v[1][3];
    F32 coef08 = m.v[2][1] * m.v[3][2] - m.v[3][1] * m.v[2][2];
    F32 coef10 = m.v[1][1] * m.v[3][2] - m.v[3][1] * m.v[1][2];
    F32 coef11 = m.v[1][1] * m.v[2][2] - m.v[2][1] * m.v[1][2];
    F32 coef12 = m.v[2][0] * m.v[3][3] - m.v[3][0] * m.v[2][3];
    F32 coef14 = m.v[1][0] * m.v[3][3] - m.v[3][0] * m.v[1][3];
    F32 coef15 = m.v[1][0] * m.v[2][3] - m.v[2][0] * m.v[1][3];
    F32 coef16 = m.v[2][0] * m.v[3][2] - m.v[3][0] * m.v[2][2];
    F32 coef18 = m.v[1][0] * m.v[3][2] - m.v[3][0] * m.v[1][2];
    F32 coef19 = m.v[1][0] * m.v[2][2] - m.v[2][0] * m.v[1][2];
    F32 coef20 = m.v[2][0] * m.v[3][1] - m.v[3][0] * m.v[2][1];
    F32 coef22 = m.v[1][0] * m.v[3][1] - m.v[3][0] * m.v[1][1];
    F32 coef23 = m.v[1][0] * m.v[2][1] - m.v[2][0] * m.v[1][1];
    
    V4F32 fac0 = { coef00, coef00, coef02, coef03 };
    V4F32 fac1 = { coef04, coef04, coef06, coef07 };
    V4F32 fac2 = { coef08, coef08, coef10, coef11 };
    V4F32 fac3 = { coef12, coef12, coef14, coef15 };
    V4F32 fac4 = { coef16, coef16, coef18, coef19 };
    V4F32 fac5 = { coef20, coef20, coef22, coef23 };
    
    V4F32 vec0 = { m.v[1][0], m.v[0][0], m.v[0][0], m.v[0][0] };
    V4F32 vec1 = { m.v[1][1], m.v[0][1], m.v[0][1], m.v[0][1] };
    V4F32 vec2 = { m.v[1][2], m.v[0][2], m.v[0][2], m.v[0][2] };
    V4F32 vec3 = { m.v[1][3], m.v[0][3], m.v[0][3], m.v[0][3] };
    
    V4F32 inv0 = V4Add(V4Sub(V4Hadamard(vec1, fac0), V4Hadamard(vec2, fac1)), V4Hadamard(vec3, fac2));
    V4F32 inv1 = V4Add(V4Sub(V4Hadamard(vec0, fac0), V4Hadamard(vec2, fac3)), V4Hadamard(vec3, fac4));
    V4F32 inV2F32 = V4Add(V4Sub(V4Hadamard(vec0, fac1), V4Hadamard(vec1, fac3)), V4Hadamard(vec3, fac5));
    V4F32 inV3F32 = V4Add(V4Sub(V4Hadamard(vec0, fac2), V4Hadamard(vec1, fac4)), V4Hadamard(vec2, fac5));
    
    V4F32 sign_a = { +1, -1, +1, -1 };
    V4F32 sign_b = { -1, +1, -1, +1 };
    
    M4x4F32 inverse;
    for(U32 i = 0; i < 4; ++i)
    {
        inverse.v[0][i] = inv0.v[i] * sign_a.v[i];
        inverse.v[1][i] = inv1.v[i] * sign_b.v[i];
        inverse.v[2][i] = inV2F32.v[i] * sign_a.v[i];
        inverse.v[3][i] = inV3F32.v[i] * sign_b.v[i];
    }
    
    V4F32 row0 = { inverse.v[0][0], inverse.v[1][0], inverse.v[2][0], inverse.v[3][0] };
    V4F32 m0 = { m.v[0][0], m.v[0][1], m.v[0][2], m.v[0][3] };
    V4F32 dot0 = V4Hadamard(m0, row0);
    F32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);
    
    F32 one_over_det = 1 / dot1;
    
    return M4MultiplyF32(inverse, one_over_det);
}

function M4x4F32
M4RemoveRotation(M4x4F32 mat)
{
    V3F32 scale =
    {
        V3Length(v3F32(mat.v[0][0], mat.v[0][1], mat.v[0][2])),
        V3Length(v3F32(mat.v[1][0], mat.v[1][1], mat.v[1][2])),
        V3Length(v3F32(mat.v[2][0], mat.v[2][1], mat.v[2][2])),
    };
    
    mat.v[0][0] = scale.x;
    mat.v[1][0] = 0.f;
    mat.v[2][0] = 0.f;
    
    mat.v[0][1] = 0.f;
    mat.v[1][1] = scale.y;
    mat.v[2][1] = 0.f;
    
    mat.v[0][2] = 0.f;
    mat.v[1][2] = 0.f;
    mat.v[2][2] = scale.z;
    
    return mat;
}

////////////////////////////////
// NOTE(allen): color

function V3F32
RGBToHSV(V3F32 rgb)
{
    F32 c_max = MaximumInV3(rgb);
    F32 c_min = MinimumInV3(rgb);
    F32 delta = c_max - c_min;
    B32 c_max_is_r = (rgb.x > rgb.y && rgb.x > rgb.z);
    B32 c_max_is_g = (rgb.y > rgb.x && rgb.y > rgb.z);
    B32 c_max_is_b = (rgb.z > rgb.x && rgb.z > rgb.y);
    F32 h = (c_max_is_r ? (rgb.y - rgb.z) / delta + 0 :
             c_max_is_g ? (rgb.z - rgb.x) / delta + 2 :
             c_max_is_b ? (rgb.x - rgb.y) / delta + 4 : 0);
    F32 s = c_max == 0 ? 0 : (delta / c_max);
    F32 v = c_max;
    V3F32 hsv = {h / 6.f, s, v};
    return hsv;
}

function V3F32
HSVToRGB(V3F32 hsv)
{
    F32 h = FMod(hsv.x * 360.f, 360.f);
    F32 s = hsv.y;
    F32 v = hsv.z;
    
    F32 c = v * s;
    F32 x = c * (1 - AbsoluteValueF(FMod((h / 60.f), 2) - 1));
    F32 m = v - c;
    
    F32 r;
    F32 g;
    F32 b;
    
    if((h >= 0.f && h < 60.f) || (h >= 360.f && h < 420.f))
    {
        r = c;
        g = x;
        b = 0;
    }
    else if(h >= 60.f && h < 120.f)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if(h >= 120.f && h < 180.f)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if(h >= 180.f && h < 240.f)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if(h >= 240.f && h < 300.f)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if((h >= 300.f && h <= 360.f) ||
            (h >= -60.f && h <= 0.f))
    {
        r = c;
        g = 0;
        b = x;
    }
    
    V3F32 rgb = {r + m, g + m, b + m};
    return rgb;
}

////////////////////////////////
// NOTE(allen): interval

function Range
MakeRange(F32 a, F32 b)
{
    Range range = {a, b};
    if (a > b)
    {
        Swap(F32, range.min, range.max);
    }
    return(range);
}
function Rangei
MakeRangei(S64 a, S64 b)
{
    Rangei range = {a, b};
    if (a > b)
    {
        Swap(S64, range.min, range.max);
    }
    return(range);
}
function Rangeu
MakeRangeu(U64 a, U64 b)
{
    Rangeu range = {a, b};
    if (a > b)
    {
        Swap(U64, range.min, range.max);
    }
    return(range);
}

#define RangeSize(range) ((range).max - (range.min))

function Range
RangeUnion(Range a, Range b)
{
    Range result;
    result.min = Min(a.min, b.min);
    result.max = Max(a.max, b.max);
    return(result);
}

function Range
RangeIntersection(Range a, Range b)
{
    Range result;
    result.min = Max(a.min, b.min);
    result.max = Min(a.max, b.max);
    result.min = ClampTop(result.min, result.max);
    return(result);
}

function Range
RangeGrow(Range range, F32 x)
{
    range.min -= x;
    range.max += x;
    return(range);
}

function Range
RangeShrink(Range range, F32 x)
{
    range.min += x;
    range.max -= x;
    range.min = ClampTop(range.min, range.max);
    return(range);
}

function B32
RangeContains(Range range, F32 x)
{
    return(range.min <= x && x < range.max);
}

function B32
RangeOverlaps(Range a, Range b)
{
    return(a.min < b.max && b.min < a.max);
}

function Rect
MakeRect(F32 x0, F32 y0, F32 x1, F32 y1)
{
    Rect rect = {x0, y0, x1, y1};
    return(rect);
}
function Rect
MakeRectVec(V2F32 p0, V2F32 p1)
{
    Rect rect = {p0.x, p0.y, p1.x, p1.y};
    return(rect);
}
function Rect
MakeRectRanges(Range x, Range y)
{
    Rect rect = {x.min, y.min, x.max, y.max};
    return(rect);
}

function Rect
RectUnion(Rect a, Rect b)
{
    Rect result;
    result.x0 = Min(a.x0, b.x0);
    result.y0 = Min(a.y0, b.y0);
    result.x1 = Max(a.x1, b.x1);
    result.y1 = Max(a.y1, b.y1);
    return(result);
}

function Rect
RectIntersect(Rect a, Rect b)
{
    Rect result;
    result.x0 = Max(a.x0, b.x0);
    result.y0 = Max(a.y0, b.y0);
    result.x1 = Min(a.x1, b.x1);
    result.y1 = Min(a.y1, b.y1);
    result.x0 = ClampTop(result.x0, result.x1);
    result.y0 = ClampTop(result.y0, result.y1);
    return(result);
}

function Rect
RectGrow(Rect rect, F32 v)
{
    rect.x0 -= v;
    rect.y0 -= v;
    rect.x1 += v;
    rect.y1 += v;
    return(rect);
}

function Rect
RectShrink(Rect rect, F32 v)
{
    rect.x0 += v;
    rect.y0 += v;
    rect.x1 -= v;
    rect.y1 -= v;
    rect.x0 = ClampTop(rect.x0, rect.x1);
    rect.y0 = ClampTop(rect.y0, rect.y1);
    return(rect);
}

function Range
RectGetRange(Rect rect, Dimension dim)
{
    Range range = {rect.p0.v[dim], rect.p1.v[dim]};
    return(range);
}

function B32
RectContains(Rect rect, V2F32 p)
{
    return(rect.x0 <= p.x && p.x < rect.x0 && rect.y0 <= p.y && p.y < rect.y0);
}

function B32
RectOverlaps(Rect a, Rect b)
{
    return(a.x0 < b.x0 && b.x0 < a.x0 && a.y0 < b.y0 && b.y0 < a.y0);
}

function V2F32
RectGetDim(Rect rect)
{
    V2F32 p = {rect.x1 - rect.x0, rect.y1 - rect.y0};
    return(p);
}
#define RectSize(r) RectGetDim(r)

function V2F32
RectGetCenter(Rect rect)
{
    V2F32 p = {0.5f*(rect.x1 + rect.x0), 0.5f*(rect.y1 + rect.y0)};
    return(p);
}

////////////////////////////////
//~ NOTE(allen): String

function B32
CharIsSpace(char c)
{
    return((c) <= 32);
}

function B32
CharIsAlpha(char c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}

function B32
CharIsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

function B32
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

function String8
str8(U8 *str, U64 size)
{
    String8 result = {str, size};
    return(result);
}

function String8
String8FromCString(char *cstring)
{
    String8 string = {0};
    string.str = (U8 *)cstring;
    string.size = CalculateCStringLength(cstring);
    return string;
}

function B32
StringMatchGeneric(String8 a, String8 b, StringMatchFlags flags)
{
    B32 result = 0;
    
    if(a.size == b.size || (flags & StringMatchFlag_RightSideSloppy))
    {
        B32 insensitive = (flags & StringMatchFlag_MatchCase);
        U64 size = Min(a.size, b.size);
        result = 1;
        for(U64 i = 0; i < size; ++i)
        {
            U8 at = a.str[i];
            U8 bt = b.str[i];
            if(insensitive)
            {
                at = CharToUpper(at);
                bt = CharToUpper(bt);
            }
            if(at != bt)
            {
                result = 0;
                break;
            }
        }
    }
    
    return result;
}

function B32
StringMatch(String8 a, String8 b)
{
    return StringMatchGeneric(a, b, StringMatchFlag_MatchCase);
}

function B32
StringMatchCaseInsensitive(String8 a, String8 b)
{
    return StringMatchGeneric(a, b, 0);
}

function String8
StringSubstring(String8 string, Rangeu range)
{
    range.max = ClampTop(range.max, string.size);
    range.min = ClampTop(range.min, string.size);
    string.str += range.min;
    string.size = RangeSize(range);
    return(string);
}

function String8
StringPrefix(String8 string, U64 size)
{
    string.size = ClampTop(size, string.size);
    return(string);
}

function String8
StringPostfix(String8 string, U64 size)
{
    size = ClampTop(size, string.size);
    string.str = (string.str + string.size) - size;
    string.size = size;
    return(string);
}

function String8
PushStringFV(M_Arena *arena, char *format, va_list args)
{
    va_list args2;
    va_copy(args2, args);
    U32 needed_bytes = vsnprintf(0, 0, format, args) + 1;
    String8 result = {0};
    result.str = PushArray(arena, U8, needed_bytes);
    result.size = vsnprintf((char*)result.str, needed_bytes, format, args2);
    result.str[result.size] = 0;
    return(result);
}

function String8
PushStringF(M_Arena *arena, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    String8 result = PushStringFV(arena, fmt, args);
    va_end(args);
    return(result);
}

////////////////////////////////
//~ NOTE(allen): String List

function String8_Node*
StringListPush(M_Arena *arena, String8_List *list, String8 string)
{
    String8_Node *node = PushArray(arena, String8_Node, 1);
    SLLQueuePush(list->first, list->last, node);
    list->node_count += 1;
    list->total_size += string.size;
    node->string = string;
    return(node);
}

function String8_Node*
StringListPushFront(M_Arena *arena, String8_List *list, String8 string)
{
    String8_Node *node = PushArray(arena, String8_Node, 1);
    SLLQueuePushFront(list->first, list->last, node);
    list->node_count += 1;
    list->total_size += string.size;
    node->string = string;
    return(node);
}

function String8_Node*
StringListPushF(M_Arena *arena, String8_List *list, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    String8 string = PushStringFV(arena, fmt, args);
    String8_Node *result = StringListPush(arena, list, string);
    va_end(args);
    return(result);
}

function String8_Node*
StringListPushFrontF(M_Arena *arena, String8_List *list, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    String8 string = PushStringFV(arena, fmt, args);
    String8_Node *result = StringListPushFront(arena, list, string);
    va_end(args);
    return(result);
}

function String8
StringListJoin(M_Arena *arena, String8_List *list, String_Join *optional_join)
{
    String_Join join = {0};
    if (optional_join == 0)
    {
        MemoryCopyStruct(&join, optional_join);
    }
    
    String8 result;
    result.size = join.pre.size + join.post.size + (list->node_count - 1)*join.sep.size + list->total_size;
    U8 *ptr = result.str = PushArray(arena, U8, result.size);
    
    MemoryCopy(ptr, join.pre.str, join.pre.size);
    ptr += join.pre.size;
    for (String8_Node *node = list->first;
         node != 0;
         node = node->next)
    {
        MemoryCopy(ptr, node->string.str, node->string.size);
        ptr += node->string.size;
        if (node->next != 0)
        {
            MemoryCopy(ptr, join.sep.str, join.sep.size);
            ptr += join.sep.size;
        }
    }
    MemoryCopy(ptr, join.post.str, join.post.size);
    ptr += join.post.size;
    
    return(result);
}

////////////////////////////////
//~ NOTE(allen): Additional String Stuff

function S64
GetFirstIntegerFromString(String8 string)
{
    S32 result = 0;
    B32 found_first_digit = 0;
    U32 integer_write_pos = 0;
    U8 integer[64] = {0};
    U32 read_pos = 0;
    for(;; ++read_pos)
    {
        if(string.str[read_pos] == 0)
        {
            break;
        }
        if(found_first_digit)
        {
            if(integer_write_pos == sizeof(integer))
            {
                integer[sizeof(integer) - 1] = 0;
                break;
            }
            if(CharIsDigit(string.str[read_pos]) || string.str[read_pos] == '-')
            {
                integer[integer_write_pos++] = string.str[read_pos];
            }
            else
            {
                integer[integer_write_pos++] = 0;
                break;
            }
        }
        else
        {
            if(CharIsDigit(string.str[read_pos]) || string.str[read_pos] == '-')
            {
                integer[integer_write_pos++] = string.str[read_pos];
                found_first_digit = 1;
            }
        }
    }
    result = CStringToI32(integer);
    return result;
}

function F32
GetFirstF32FromCString(char *str)
{
    F32 result = 0;
    B32 found_first_digit = 0;
    U32 float_write_pos = 0;
    char float_str[64] = {0};
    U32 read_pos = 0;
    for(;; ++read_pos)
    {
        if(str[read_pos] == 0)
        {
            break;
        }
        if(found_first_digit)
        {
            if(float_write_pos == sizeof(float_str))
            {
                float_str[sizeof(float_str) - 1] = 0;
                break;
            }
            if(CharIsDigit(str[read_pos]) || str[read_pos] == '.' || str[read_pos] == '-')
            {
                float_str[float_write_pos++] = str[read_pos];
            }
            else
            {
                float_str[float_write_pos++] = 0;
                break;
            }
        }
        else
        {
            if(CharIsDigit(str[read_pos]) || str[read_pos] == '.' || str[read_pos] == '-')
            {
                float_str[float_write_pos++] = str[read_pos];
                found_first_digit = 1;
            }
        }
    }
    result = CStringToF32(float_str);
    return result;
}

function void
CopySubstringToStringUntilCharN(char *str1, U32 str1_max,
                                const char *str2, char str2_term)
{
    U32 write_pos = 0;
    while(1)
    {
        if(str2[write_pos] == str2_term || write_pos == str1_max - 1)
        {
            str1[write_pos++] = 0;
            break;
        }
        else
        {
            str1[write_pos] = str2[write_pos];
            ++write_pos;
        }
    }
}

function void
CopyStringToFixedSizeBuffer(char *dest, U32 dest_max,
                            const char *src)
{
    U32 read_pos = 0;
    U32 write_pos = 0;
    for(;;)
    {
        if(src[read_pos] == 0 || write_pos >= dest_max)
        {
            break;
        }
        dest[write_pos++] = src[read_pos++];
    }
    if(write_pos >= dest_max)
    {
        dest[dest_max - 1] = 0;
    }
    else
    {
        dest[write_pos++] = 0;
    }
}

function U32
CStringIndexAfterSubstring(char *str, char *substr)
{
    U32 result = 0;
    for(U32 i = 0; str[i]; ++i)
    {
        if(str[i] == substr[0])
        {
            if(CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
            {
                result = i + CalculateCStringLength(substr);
            }
        }
    }
    return result;
}

function U32
CStringFirstIndexAfterSubstring(char *str, char *substr)
{
    U32 result = 0;
    for(U32 i = 0; str[i]; ++i)
    {
        if(str[i] == substr[0])
        {
            if(CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
            {
                result = i + CalculateCStringLength(substr);
                break;
            }
        }
    }
    return result;
}

function void
CopyCStringToFixedSizeBuffer(char *destination, U32 destination_max, char *source)
{
    for(U32 i = 0; i < destination_max; ++i)
    {
        destination[i] = source[i];
        if(source[i] == 0)
        {
            break;
        }
    }
    destination[destination_max-1] = 0;
}

function void
CopyCStringToFixedSizeBufferN(char *destination, U32 destination_max, char *source, U32 source_max)
{
    for(U32 i = 0; i < destination_max && i < source_max; ++i)
    {
        destination[i] = source[i];
        if(source[i] == 0)
        {
            break;
        }
    }
    destination[destination_max-1] = 0;
}

function char *
ConvertCStringToLowercase(char *str)
{
    for(int i = 0; str[i]; ++i)
    {
        str[i] = CharToLower(str[i]);
    }
    return str;
}

function char *
ConvertCStringToUppercase(char *str)
{
    for(int i = 0; str[i]; ++i)
    {
        str[i] = CharToUpper(str[i]);
    }
    return str;
}

function char *
ConvertCStringToLowercaseWithUnderscores(char *str)
{
    for(int i = 0; str[i]; ++i)
    {
        if(str[i] == ' ')
        {
            str[i] = '_';
        }
        else
        {
            str[i] = CharToLower(str[i]);
        }
    }
    return str;
}

static unsigned int global_crc32_table[] =
{
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
    0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
    0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
    0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
    0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
    0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
    0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
    0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
    0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
    0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
    0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
    0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
    0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
    0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
    0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
    0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
    0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
    0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
    0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
    0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
    0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
    0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4,
};

function U32
CStringCRC32N(char *name, U32 n)
{
    U32 crc = 0;
    for(U32 i = 0; name[i] && i < n; ++i)
    {
        crc = (crc << 8) ^ global_crc32_table[((crc >> 24) ^ name[i]) & 255];
    }
    return crc;
}

function U32
CStringCRC32(char *name)
{
    return CStringCRC32N(name, (U32)(U32)(-1));
}

function void
AppendToFixedSizeCString(char *destination, U32 destination_max, char *str)
{
    U32 i = 0;
    for(; i < destination_max && destination[i]; ++i);
    if(destination_max > i)
    {
        CopyCStringToFixedSizeBuffer(destination + i, destination_max - i, str);
    }
}

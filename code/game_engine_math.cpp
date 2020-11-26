
struct color
{
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;
};

struct vec4
{
    union
    {
        struct
        {
            real32 a;
            real32 r;
            real32 g;
            real32 b;
        };

        struct
        {
            real32 x;
            real32 y;
            real32 z;
            real32 w;
        };
    };
};

internal vec4
operator*(vec4& A, real32 B)
{
    vec4 Result;
    Result = vec4{A.x*B, A.y*B, A.z*B, A.w*B};
    return Result;
}

internal vec4
operator*(real32 B, vec4& A)
{
    vec4 Result;
    Result = vec4{A.x*B, A.y*B, A.z*B, A.w*B};
    return Result;
}

internal vec4&
operator*=(vec4& A, real32 B)
{
    A = A*B;
    return A;
}

struct vec2
{
        real32 x;
        real32 y;
};

inline vec2
operator-(vec2& A)
{
    vec2 Result;
    Result = vec2{-A.x, -A.y};
    return Result;
}

internal vec2
operator+(vec2& A, vec2& B)
{
    vec2 Result;
    Result = vec2{A.x+B.x, A.y+B.y};
    return Result;
}

internal vec2&
operator+=(vec2& A, vec2& B)
{
    A = A+B;
    return A;
}

internal vec2
operator-(vec2& A, vec2& B)
{
    vec2 Result;
    Result = vec2{A.x-B.x, A.y-B.y};
    return Result;
}

internal vec2&
operator-=(vec2& A, vec2& B)
{
    A = A-B;
    return A;
}

internal vec2
operator*(vec2& A, real32 B)
{
    vec2 Result;
    Result = vec2{A.x*B, A.y*B};
    return Result;
}

internal vec2
operator*(real32 B, vec2& A)
{
    vec2 Result;
    Result = vec2{A.x*B, A.y*B};
    return Result;
}

internal vec2&
operator*=(vec2& A, real32 B)
{
    A = A*B;
    return A;
}

internal real32
Dot(vec2& A, vec2& B)
{
    real32 Result = (A.x*B.x+A.y*B.y);
    return Result;
}

internal real32
LengthSqr(vec2& A)
{
    real32 Result;
    Result = (A.x*A.x)+(A.y*A.y);
    return Result;
}

internal real32
DistanceSqr(vec2& B, vec2& A)
{
    return (A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y);
}

# Interpreter

## Base types
All basic types used in the program:
`s8, s16, s32, s64, u8, u16, u32, u64, f32, f64, b32`

## Helper Macros
Macros that used all over the place:
- `#define global static`
- `#define internal static`
- `#define global_persist static`
- `#define Max(a, b) ((a) > (b) ? (a) : (b))`
- `#define Min(a, b) ((a) < (b) ? (a) : (b))`
- `#define SizeOf(item) ((char *)(&item + 1) - (char *)(&item))`
- `#define OffsetOf(type, member) ((mem_index)(&((type *)0)->member))`

Macros used for testing:
``` C
#define dotest(stream, result)\
     lexer = init_stream(#stream);\
     Assert(expr0(&lexer) == (result));
```

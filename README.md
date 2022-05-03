# Interpreter
This is an interpreter for a subset of C programming language. 
The lexer part is done, but it can and probably will be rewritten with FSM in the future to speed up lexing phase. 
This program is still in development!

The rest of this doc will try to briefly cover the set of macros and important data structures which are used all over the place.
For complete understanding and more exhaustive list of things I suggest you to look in the code.

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

Macros for dynamic array:
- `#define darr_len(array) DynamicArray__len(array)`
- `#define darr_cap(array) DynamicArray__cap(array)`
- `#define darr_push(array, item) DynamicArray__push(array, item)`
- `#define darr_free(array) DynamicArray__free(array)`

Macros used for testing:
``` C
#define dotest(stream, result)\
     lexer = init_stream(#stream);\
     Assert(expr0(&lexer) == (result));
     
#define Match(match)\
     Assert((token = eattoken(&lexer)).kind == match)
```

## Data Structures
Token struct holds information like token-kind, token-subkind, lexeme, and union to store the value of number:
``` C
typedef struct Token{
    TokenKind kind;     // base token kind.
    TokenSubKind skind; // token sub kind.
    String lexeme;      // just for print, because we have an intern.
    u32 linenumber;
    u32 columnnumber;
    union{
        u64 U64;
        f64 F64;
        String intern;
    }u;
}Token;
```

Lexer struct holds information about source file being scanned, lexer state, line/column numbers, current token and lookahead token and other data:
``` C
typedef struct Lexer{
    String source;    // name of the file being scanned.
    String contents;  // contents of the file being scanned.
    char at[2];       // parse point, at[0] is the current position in the file, and at[1] is lookahead character.
    u32 lastline;     // current line in the source file where the parse point is.
    u32 lastcolumn;   // current column int he source file where the parse point is.
    Token token;      // current token.
    Token lookahead;  // lookahead token, we can use the same technique as at[2].
    Token *tokenbuf;  // STUDY: Do I really need it? buffer of all tokens corresponding to the current source excluding whitespaces.
    LexerState state; // we need this as a flag to recognize some Ambiguous tokens.
    s32 flag;         // additional information for the state.
    s32 error;        // error code
}Lexer;
```

Dynamic array data structure:
``` C
typedef struct DynamicArray{
    mem_index len;
    mem_index cap;
    char arr[1];
}DynamicArray;
```

## Grammar
Here is a short subset of a grammar used to write an interpreter:
``` C
// expr0 := expr1 ;
// expr1 := expr2 + expr2 
//        | expr2 - expr2
//        | expr2 & expr2
//        | expr2 | expr2
//        | expr2 ^ expr2
//        | expr2 << expr2
//        | expr2 >> expr2
//        | expr2
// expr2 := epxr3 * expr3
//        | expr3 / expr3
//        | expr3 % expr3
//        | expr3
// expr3 := -expr4
//        | +epxr4
//        | expr4
// expr4 := ( expr1 )
//        | name
//        | number

// int_num := 0|[1-9][0-9]*
//          | 0(X|x)[0-9|a-f|A-F]+
//          | 0[0-7]+
//          | 0(B|b)[0-1]+

// float_num := (0|[1-9][0-9]*)(e|.[0-9]*)([E|e](e|+|-)(0|[1-9][0-9]*))?
//            | (.[0-9]*)([E|e](e|+|-)(0|[1-9][0-9]*))?

// esc_seq := [\n|\r|\a|\b|\f|\t|\v|\\|\?|\'|\"|\ooo|\xhh]
// char_literal := '\'' (esc_seq|?|"|(^esc_seq)+) '\''

// string_literal := '"' (^")* '"'

// name := [a-z|A-Z|_] [a-z|A-Z|_|0-9]*

// name_list := name (',' name)*

// assign_op := | = | *= | /= | %= | += | -= | <<= | >>= | &= | ^= | |=
```

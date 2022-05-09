


#if !defined(PARSER_H)
#define PARSER_H

//
// EBNF Grammar:
//

// name := ([a-z]|[A-Z]|_) ([a-z]|[A-Z]|_|[0-9])*

// octal := 0[0-7]+
// binary := 0(B|b)(0|1)+
// integer := (0|[1-9][0-9]*)
// hexidecimal := 0(X|x)([0-9]|[a-f]|[A-F])+
// int_number := (-|+)(integer|octal|hexidecimal|binary)

// float_number := (-|+) (0|[1-9][0-9]*) (e|.[0-9]*) ((E|e)(-|+)?(0|[1-9][0-9]*))?
//                 | (-|+) .[0-9]* ((E|e)(-|+)?(0|[1-9][0-9]*))

// assign_op := (= | += | -= | *= | /= | %= | |= | &= | ^= | <<= | >>= )

// unary_op := (& | * | + | - | ~ | !)

// esc_seq := (\n|\t|\v|\b|\r|\f|\a|\\|\?|\'|\"|\ooo|\xhh)
// char_literal := ' (esc_seq | ^es_seq) '

// string := " (^("|\n|\r))* "

// c_multiline_comment := /* (^*|*+(^/))* */
// c_singleline_comment := // (^\n)* \n
// c_comment := c_multiline_comment | c-singleline_comment

// base_type := (void|char|short|int|long|unsigned|signed|double)
// storage_class_specifier := (static | extern)
// type_qualifier := (const | volatile)
 
// enum_iterm := name '=' expr0
// enum_items := enum_item (',' enum_iterm)*
// enum_decl := 'enum' name '{' enum_items? '}' ';'

struct t; // struct definition.
struct t{f32 x1, x2;}; // struct declaration.
struct {f32 x1, x2;}; // anonimus structure.

union x; // union definition.
union x{f64 F64; u64 U64; char *string;}; // union declaration.
union {f64 F64; u64 U64; char *string;}; // anonimus union.

struct outter_struct1
{
    f32 F32;
    struct inner_struct1
    {
        u32 x1, x2;
        char *name1;
        char *name2;
        f32 arr[10];
    };

    struct inner_struct2
    {
        u32 count_of_pointer;
        void (*func_ptr1)(u32 param1, u32 param2);
        void (*func_ptr2)(char *param1, struct outter_struct1 *param2);
    };

    union // anonimus union.
    {
        f64 F64;
        u64 U64;
        char *string;
    };

    struct outter_struct2
    {
        char *param1;
        void *(*another_func_ptr)(u32 param1, u32 param2);
        struct deep_nested_struct
        {
            union deeper_nested_union
            {
                void (*another_ptr)();

                // an array of pointers to function name(u32, f32, f32) returning int.
                int (*x[3])(u32 param1, f32 param2, f32 param3); 
            };
            
            char *param2;
            char *param3;
            void *memory;
            u32 some_bytes[]; // Zero-sized array can be only in the end.
        };
    };
    
};


typedef struct Decl{
    String source;
    u32 line_number;
    u32 column_number;
    char name[512];
    char data_type[32]; // declaration data type (int|short|char...).
    char type[512]; // full type, like (function returning int).
    char final_type[512];
}Decl;

// param_list := type name (',' type name)*
// direct_decl := name
//              | '(' decl ')'
//              | direct_decl '(' param_list? ')'
//              | direct_decl '[ const_expr? ']'

// decl := (*)* direct_decl
internal Decl init_decl_parser(Lexer *lexer);
internal void parse_decl(Lexer *lexer, Decl *decl);
internal void parse_direct_decl(Lexer *lexer, Decl *decl);

//struct_declaration_list := struct_or_union_decl
//                          | struct_declaration_list
// struct_or_union_decl := (struct|union) name? '{' struct_declaration_list '}' ';'
//                       | (struct|union) name ';'

// decl := struct_or_union_decl
//       | enum_decl

// expr0 := expr1 + expr1
//        | expr1 - expr1
//        | expr1 | expr1
//        | expr1 & expr1
//        | expr1 ^ expr1
//        | expr1 << expr1
//        | expr1 >> expr1
//        | expr1
//
// expr1 := expr2 * expr2
//        | expr2 / expr2
//        | expr2 % expr2
//        | expr2
//
// expr2 := +expr3
//        | -expr3
//        | expr3
//
// expr3 := '(' expr0 ')'
//        | name
//        | float_number
//        | int_number
internal s64 expr0(Lexer *lexer);
internal s64 expr1(Lexer *lexer);
internal s64 expr2(Lexer *lexer);
internal s64 expr3(Lexer *lexer);
internal s64 expr4(Lexer *lexer);

#endif // PARSER_H

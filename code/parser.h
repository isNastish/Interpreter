


#if !defined(PARSER_H)
#define PARSER_H

#define darr(x)

typedef enum DeclType{
    DT_FuncDecl,
    DT_VarDecl,
    DT_EnumDecl,
    DT_StructDecl,
    DT_UnionDecl,
    DT_TypedefDecl,
}DeclType;

typedef enum FuncLinkageType{
    FLT_Static,
    FLT_Extern, // by default.
}FuncLinkageType;

typedef struct Decl Decl;
typedef struct FuncDecl FuncDecl;
typedef struct VarDecl VarDecl;
typedef struct StructDecl StructDecl;
typedef struct UnionDecl UnionDecl;
typedef struct TypedefDecl TypedefDecl;
typedef struct EnumDecl EnumDecl;

typedef struct FuncDeclParam FuncDeclParam;
struct FuncDeclParam { String name; String type; };
struct FuncDecl{
    String return_type;

    darr(FuncDeclParam *params;)

    union{
        f64 F64;
        u64 U64;
        
        // etc...
    }return_value;

    FuncLinkageType linkage_type;
};

struct VarDecl{
    String type;

    union{
        f64 F64;
        u64 U64;
        String string;
    }value;
};  

struct Decl{
    String source;
    u32 line_number;
    u32 column_number;

    String name;
    DeclType type;
    union{
        FuncDecl func_decl;
        VarDecl var_decl;
        //EnumDecl enum_decl;
        //StructDecl struct_decl;
        //UnionDecl union_decl;
        //TypedefDecl typedef_decl;   
    };
};

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

// linkage_type := (static|extern)

// name_list := name (',' name)*

// type_list := type (',' type)*

// type_specifier := (char|short|int|long|float|double|signed|unsigned)
// type_qualifier := (const|volatile)
// type := type_qualifier? type_specifier
 
// enum_iterm := name '=' expr0
// enum_items := enum_item (',' enum_iterm)*
// enum_decl := 'enum' name '{' enum_items? '}' ';'

struct t; // struct definition.
struct t{f32 x1, x2;}; // struct declaration.
struct {f32 x1, x2;}; // anonimus structure.

union x; // union definition.
union x{f64 F64; u64 U64; char *string;}; // union declaration.
union {f64 F64; u64 U64; char *string;}; // anonimus union.

//struct_declaration_list := struct_or_union_decl
//                          | struct_declaration_list
// struct_or_union_decl := (struct|union) name? '{' struct_declaration_list '}' ';'
//                       | (struct|union) name ';'


//
// TODO: Grammar for functions is not complete.
//

// func_param := type name 
// func_param_list := func_param (',' func_param)*
// func_name := name
// func_return_type := (int_number | float_number)
// func_decl := func_return_type func_name '(' func_param_list? ')' '{' stmt_block '}'

// var_direct_decl := name
//                  | '(' var_decl1 ')
//                  | var_direc_decl '[' const_expr ']'
// var_decl1 := (*)* var_direct_decl
// var_decl := type var_decl1 ('=' expr0)? ';'
internal void parse_decl(Lexer *lexer, Decl *decl);
internal void parse_direct_decl(Lexer *lexer, Decl *decl);

// decl := func_decl
//       | var_decl
//       | struct_decl
//       | union_decl
//       | enum_decl
//       | typedef_decl

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

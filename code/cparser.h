

/*
 * Aleksey Yevtushenko
 * February 16, 2022
 */

#if !defined(CPARSER_H)
#define CPARSER_H

// NOTE: Just a tag which says that it's a dynamic array.
#define arr(x) x 

//
// grammar: 
//

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

internal s64 expr0(Lexer *lexer);
internal s64 expr1(Lexer *lexer);
internal s64 expr2(Lexer *lexer);
internal s64 expr3(Lexer *lexer);
internal s64 expr4(Lexer *lexer);


//
// grammar:
//

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

// switch_default := 'default' ':' stmt*
// switch_case := 'case' '('? const_expr ')'? ':' stmt*
// switch_block := '{' switch_case* switch_default? '}'
// switch_stmt := 'switch' '(' expr ')' switch_block

// expr_stmt   := expr? ';'
// for_stmt    := 'for' '(' expr? ';' expr? ';' expr?) stmt_block
// while_stmt  := 'while' '(' expr ')' stmt_block
// do_while    := 'do' stmt_block 'while' '(' expr ')' ';'

// if_stmt     := 'if' '(' expr ')' (stmt_block? | ';')
//                ('else' 'if' '(' expr ')' (stmt_block? | ';'))* 
//                ('else' (stmt_block? | ';'))?

// break_stmt := 'break' ';'

// continue_stmt := 'continue' ';'

// return_stmt := 'return' ('(' expr ')' | expr?) ';'

// stmt := switch_stmt
//       | expr_stmt
//       | comp_stmt
//       | if_stmt
//       | while_stmt
//       | for_stmt
//       | do_stmt
//       | goto_stmt
//       | continue_stmt
//       | return_stmt
//       | '{' stmt* '}'


// enum_item := name ('=' const_expr)?
// enum_items := enum_item (',' enum_item)* ','?
// enum_decl := name '{' enum_items? '}' ';'

// func_param := type name
// func_param_list := func_param (',' func_param)*
// func_decl := type name '(' func_param_list? ')' stmt_block

// typedef_decl := type name ';'

// aggregate_decl := TODO

// var_decl := TODO

// decl := 'enum' enum_decl
//       | 'struct' aggregate_decl
//       | 'union' aggregate_decl
//       | var_decl
//       | 'typedef' typedef_decl
//       | func_decl


// ternary_expr := or_expr '?' ternary_expr ':' ternary_expr ';'

// expr := ternary_expr

typedef struct Expr Expr;
typedef struct Decl Decl;
typedef struct Stmt Stmt;

typedef enum DeclKind{
    Decl_Unknown,
}DeclKind;

typedef struct Decl{
    DeclKind kind;
    String name; // decl name.
    
}Decl;

typedef enum ExprKind{
    Expr_Unknown,
    
    Expr_Int, 
    Expr_Float, 
    Expr_String,
    Expr_Identifier, 
    Expr_Compound, 
    Expr_Binary, 
    Expr_Unary,
    Expr_Ternary,
    
}ExprKind;

struct Expr{
    ExprKind kind;
    union{
        s64 S64;
        f64 F64;
        String string;
        String name; // NOTE: ?
        
        
        // Binary operator.
        struct{
            Expr *left_part;
            Expr *right_part;
        };
        
        // Ternary operator.
        struct{
            Expr *cond;
            Expr *then_part;
            Expr *else_part;
        };
    }u;
};

typedef enum StmtKind{
    Stmt_Unknown, 
    
    Stmt_Expr, 
    Stmt_For, 
    Stmt_While, 
    Stmt_Do, 
    Stmt_If, 
    Stmt_Switch, 
    Stmt_Return,
    
}StmtKind;

typedef struct StmtBlock{
    arr(Stmt **stmts);
}StmtBlock;

typedef struct ElseIf{
    Expr *cond; // condition.
    StmtBlock *block; 
}ElseIf;

typedef struct Case{
    arr(Expr **exprs);
    StmtBlock block;
}Case;

typedef struct Stmt{
    StmtKind kind;
    Expr *epxr; // NOTE: if_expr, while_expr, do_while_expr, for_expr, return_expr.
    union{
        // If.
        struct{
            Expr *if_expr;
            StmtBlock *block;
            arr(ElseIf *elseifs); // NOTE: can be doubly linked list.
            StmtBlock *else_block; 
        };
        
        // while.
        struct{
            Expr *while_expr;
            StmtBlock *while_block;
        };
        
        // for.
        struct{
            Expr *for_expr;
            Expr *init_part;
            Expr *cond_part;
            Expr *then_part;
            StmtBlock for_block;
        };
        
        // do.
        struct{
            StmtBlock *do_block;
            Expr *do_while_expr;
        };
        
        // return.
        struct{
            // TypeSpec return_type;
            Expr *return_expr;
        };
        
        // switch.
        struct{
            Expr *switch_expr;
            arr(Case **cases);
            Expr *default_expr;
        };
    };
}Stmt;

#endif // CPARSER_H



/*
 * Aleksey Yevtushenko
 * February 16, 2022
 */

#if !defined(CPARSER_H)
#define CPARSER_H

// NOTE: Just a tag which says that it's a dynamic array.
#define darr(x) x 

internal s64 expr0(Lexer *lexer);
internal s64 expr1(Lexer *lexer);
internal s64 expr2(Lexer *lexer);
internal s64 expr3(Lexer *lexer);
internal s64 expr4(Lexer *lexer);

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
    darr(Expr **exprs);
    StmtBlock block;
}Case;

typedef struct Stmt{
    StmtKind kind;
    union{
        // If.
        struct{
            Expr *if_expr;
            StmtBlock *block;
            darr(ElseIf *elseifs); // NOTE: can be doubly linked list.
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
            darr(Case **cases);
            Expr *default_expr;
        };
    };
}Stmt;

#endif // CPARSER_H

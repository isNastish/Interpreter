

/*
 * Aleksey Yevtushenko
 * February 16, 2022
 */

internal s64 expr0(Lexer *lexer);
internal s64 expr1(Lexer *lexer);
internal s64 expr2(Lexer *lexer);
internal s64 expr3(Lexer *lexer);
internal s64 expr4(Lexer *lexer);

// expr0 := epxr1 ;
internal s64 expr0(Lexer *lexer){
    s64 result = 0;
    
    nexttoken(lexer);
    result = expr1(lexer);
    if(!is_token(lexer->token, TK_Semi)){
        // TODO: error handling.
    }
    
    return(result);
}

// // expr1 := expr2 + expr2 
//        | expr2 - expr2
//        | expr2 & expr2
//        | expr2 | expr2
//        | expr2 ^ expr2
//        | expr2 << expr2
//        | expr2 >> expr2
//        | expr2
internal s64 expr1(Lexer *lexer){
    s64 result = expr2(lexer);
    
    while(is_token(lexer->token, TK_Plus) ||
          is_token(lexer->token, TK_Minus) ||
          is_token(lexer->token, TK_And) ||
          is_token(lexer->token, TK_Or) ||
          is_token(lexer->token, TK_Xor) ||
          is_token(lexer->token, TK_ShiftL) ||
          is_token(lexer->token, TK_ShiftR)){
        s32 op = (lexer->token.kind);
        nexttoken(lexer);
        
        switch(op){
            case(TK_Plus):{ result += expr2(lexer); }break;
            case(TK_Minus):{ result -= expr2(lexer); }break;
            case(TK_And):{ result &= expr2(lexer); }break;
            case(TK_Or):{ result |= expr2(lexer); }break;
            case(TK_Xor):{ result ^= expr2(lexer); }break;
            case(TK_ShiftL):{ result <<= expr2(lexer); }break;
            case(TK_ShiftR):{ result >>= expr2(lexer); }break;
        }
    }
    
    return(result);
}

// expr2 := epxr3 * expr3
//        | expr3 / expr3
//        | expr3 % expr3
//        | expr3
internal s64 expr2(Lexer *lexer){
    s64 result = expr3(lexer);
    
    while(is_token(lexer->token, TK_Mul) ||
          is_token(lexer->token, TK_Div) ||
          is_token(lexer->token, TK_Mod)){
        s32 op = lexer->token.kind;
        nexttoken(lexer);
        
        switch(op){
            case(TK_Mul):{ result *= expr3(lexer); }break;
            case(TK_Div):{
                s64 tmp = expr3(lexer);
                if(tmp == 0){
                    // TOOD: error handling (even God cannot divide by zero).
                }
                else{
                    result /= tmp;
                }
            }break;
            case(TK_Mod):{ result %= expr3(lexer); }break;
        }
    }
    
    return(result);
}

// expr3 := -expr4
//        | +epxr4
//        | expr4
internal s64 expr3(Lexer *lexer){
    s64 result = 0;
    
    if(is_token(lexer->token, TK_Minus)){
        nexttoken(lexer);
        result = -expr4(lexer);
    }
    else if(is_token(lexer->token, TK_Plus)){
        nexttoken(lexer);
        result = expr4(lexer);
    }
    else{
        result = expr4(lexer);
    }
    
    return(result);
}

// expr4 := ( expr1 )
//        | name
//        | number
internal s64 expr4(Lexer *lexer){
    s64 result = 0;
    
    if(is_token(lexer->token, TK_OpenParen)){
        nexttoken(lexer);
        result = expr1(lexer);
        if(is_token(lexer->token, TK_CloseParen)){
            nexttoken(lexer);
        }
        else{
            // TODO: error handling (missing close parentheses).
        }
    }
    else if(is_token(lexer->token, TK_IntNum)){
        result = lexer->token.u.U64;
        nexttoken(lexer);
    }
    else if(is_token(lexer->token, TK_FloatNum)){
        result = lexer->token.u.F64;
        nexttoken(lexer);
    }
    else if(is_token(lexer->token, TK_Name)){
        // TODO: implement later.
    }
    else{
        // TODO: error handling (unexpected token kind).
    }
    
    return(result);
}

internal b32 is_type_specifier(Lexer *lexer){
    b32 result = 0;
    
    //
    // TODO: deal with TK_Void (from the parser side).
    //
    
    if(is_token(lexer->token, TK_Char)){result = 1;}
    else if(is_token(lexer->token, TK_Short)){result = 1;}
    else if(is_token(lexer->token, TK_Int)){result = 1;}
    else if(is_token(lexer->token, TK_Long)){result = 1;}
    else if(is_token(lexer->token, TK_Unsigned)){result = 1;}
    else if(is_token(lexer->token, TK_Signed)){result = 1;}
    else if(is_token(lexer->token, TK_Float)){result = 1;}
    else if(is_token(lexer->token, TK_Double)){result = 1;}
    
    return(result);
}

// decl := (*)* direct_decl
internal Decl init_decl_parser(Lexer *lexer){
    Decl decl = {0};
    
    decl.source = lexer->source;
    decl.line_number = lexer->lastline;
    decl.column_number = lexer->lastcolumn;

    nexttoken(lexer);
    if(is_type_specifier(lexer)){
        strncpy(decl.data_type,
                lexer->token.u.intern.data,
                lexer->token.u.intern.count);
    }
    
    parse_decl(lexer, &decl);

    // NOTE: forming final description.
    strcpy(decl.final_type, decl.name);
    strcat(decl.final_type, ": ");
    if(*decl.type){
        strcat(decl.final_type, decl.type);
    }
    strcat(decl.final_type, decl.data_type);

    return(decl);
}

internal void parse_decl(Lexer *lexer, Decl *decl){
    s32 count_stars = 0;
    for(nexttoken(lexer);
        is_token(lexer->token, TK_Mul);
        nexttoken(lexer)){
        ++count_stars;
    }
    parse_direct_decl(lexer, decl);
    
    while(count_stars){
        strcat(decl->type, " pointer to");
        --count_stars;
    }
}

internal void parse_direct_decl(Lexer *lexer, Decl *decl){
    if(is_token(lexer->token, TK_OpenParen)){
        parse_decl(lexer, decl);
        if(is_token(lexer->token, TK_CloseParen)){
            nexttoken(lexer); 
        }
        else{
            // TODO: error handling.
        }
    }
    else if(is_token(lexer->token, TK_Name)){
        strncpy(decl->name,
                lexer->token.u.intern.data,
                lexer->token.u.intern.count);
        nexttoken(lexer);
    }
    else{
        // TODO: error handling (expected name or dcl).
    }

    TokenKind kind;
    while(is_token(lexer->token, TK_OpenParen) ||
          is_token(lexer->token, TK_OpenBracket)){
        kind = lexer->token.kind;
        if(kind == TK_OpenParen){
            nexttoken(lexer);
            if(is_token(lexer->token, TK_CloseParen)){
                strcat(decl->type, " function returning");
            }
            else{
                // TODO: error handling.
            }
        }
        else{ // '['
            nexttoken(lexer);
            if(is_token(lexer->token, TK_CloseBracket)){
                strcat(decl->type, " array[] of");
            }
            else{
                // TODO: error handling.
            }
        }
        nexttoken(lexer);
    }
}


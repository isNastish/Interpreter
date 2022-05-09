

// Aleksey Yevtushenko
// February 13, 2022

#include "common.h"
#include "io.c"
#include "table.c"
#include "lexer.c"
#include "parser.c"
#include "test.c"

int main(int args_count, char **args_values){
    intern_reserved(&intern_table);
    
    CONSOLE_INFO;
    
    TEST(test_keywords);
    TEST(test_operators);
    TEST(test_integers);
    TEST(test_floats);
    TEST(test_characters);
    TEST(test_strings);
    TEST(test_comments);
    TEST(test_suffixes);
    TEST(test_preproc);
    TEST(test_parse_expr);
    TEST(test_parse_declarations);
    
    return(0);
}

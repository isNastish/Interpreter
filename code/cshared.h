

/*
 * Aleksey Yevtushenko
 * February 10, 2022
 */

#if !defined(CSHARED_H)
#define CSHARED_H

// Checks whether the character is ' ', '\t', '\v', '\f', '\r', '\n'.
internal b32 is_white(char c);

//Checks whether the character is '\r', '\n'.
internal b32 is_newline(char c);

// Checks whether the character is ' ', '\t', '\v', '\f'.
internal b32 is_spacing(char c);

// Checks whether the character is alphabet symbol.
internal b32 is_alpha(char c);

// Checks whether the character is digit.
internal b32 is_digit(char c);

// Checks whether the character is alphanumeric.
internal b32 is_alnum(char c);

// Checks whether the character is valid hex symbol. [0-9|a-f|A-F].
internal b32 is_hex(char c);

// Checks whether the character is valid oct symbol. [0-7].
internal b32 is_oct(char c);

// Checks whether the character is valid binary symbol. '0', '1'.
internal b32 is_bin(char c);

// Checks whether the c is in uppercase.
internal b32 is_upper(char c);

// Checks whether the c is in lowercase.
internal b32 is_lower(char c);

// Checks whether the string contains given character.
internal b32 oneof(char c, char *s);

// Converts lowercase character to uppercase.
internal b32 to_upper(char c);

// Convers uppercase character to lowercase.
internal b32 to_lower(char c);

// Converts each lowercase character in string to uppercase.
internal void to_upper_s(char *s);

// Convers each uppercase character in string to lowercase.
internal void to_lower_s(char *s);

// Returns pointer to the first occurence of s2 in s1, or 0 if not present.
internal char *is_substr(char *s1, char *s2);

// Returns length of the string s.
internal u32 str_len(char *s);

// Compares two string, the same as strcmp.
internal s32 str_cmp(char *s1, char *s2);

// Computes hex digit based on it's symbol. Assumes that c is valid hex symbol.
internal u32 hexval(char c);

// Returns true if c1 == '/', and c2 == '*', false otherwise.
internal b32 Ccomment_start(char c1, char c2);

// Returns true if c1 == '*', and c2 == '/', false otherwise.
internal b32 Ccomment_end(char c1, char c2);

// Returns true if c1 == '/' and c2 == '/', false otherwise.
internal b32 Cppcomment(char c1, char c2);

/* Returns true if c1 is whitespace, or we have the start of C, or Cpp comment,
   false otherwise. */
internal b32 white_or_comment(char c1, char c2);

// Returns true if "\n\r" | "\r\n".
internal b32 windows_newline(char c1, char c2);

internal b32 is_white(char c){
    b32 result = ((c == ' ') ||
                  (c == '\t') ||
                  (c == '\v') ||
                  (c == '\f') ||
                  (c == '\r') ||
                  (c == '\n'));
    return(result);
}

internal b32 is_newline(char c){
    b32 result = ((c == '\r') ||
                  (c == '\n'));
    return(result);
}

internal b32 is_spacing(char c){
    b32 result = ((c == ' ') ||
                  (c == '\t') ||
                  (c == '\v') ||
                  (c == '\f'));
    return(result);
}

internal b32 is_alpha(char c){
    b32 result = (((c >= 'a') && (c <= 'z')) ||
                  ((c >= 'A') && (c <= 'Z')));
    return(result);
}

internal b32 is_digit(char c){
    b32 result = ((c >= '0') && (c <= '9'));
    return(result);
}

internal b32 is_alnum(char c){
    b32 result = (((c >= '0') && (c <= '9')) ||
                  ((c >= 'a') && (c <= 'z')) ||
                  ((c >= 'A') && (c <= 'Z')));
    return(result);
}

internal b32 is_hex(char c){
    b32 result = (((c >= '0') && (c <= '9')) ||
                  ((c >= 'a') && (c <= 'f')) ||
                  ((c >= 'A') && (c <= 'F')));
    return(result);
}

internal b32 is_oct(char c){
    b32 result = ((c >= '0') && (c <= '7'));
    return(result);
}

internal b32 is_bin(char c){
    b32 result = ((c == '0') || (c == '1'));
    return(result);
}

internal b32 is_upper(char c){
    b32 result = ((c >= 'A') && (c <= 'Z'));
    return(result);
}

internal b32 is_lower(char c){
    b32 result = ((c >= 'a') && (c <= 'z'));
    return(result);
}

internal b32 oneof(char c, char *s){
    b32 result = 0;
#if 0
    while(*s && (*s != c)) s += 1;
    if(c == *s) result = 1;
#else
    result = ((c == s[0]) ||
              (c == s[1]));
#endif
    return(result);
}

internal b32 to_upper(char c){
    b32 result = c;
    if(is_lower(c)){
        result -= ('a' - 'A');
    }
    return((char)result);
}

internal b32 to_lower(char c){
    b32 result = c;
    if(is_upper(c)){
        result += ('a' - 'A');
    }
    return((char)result);
}

internal void to_upper_s(char *s){
    while(*s){
        if(is_lower(*s)){
            *s = to_upper(*s);
        }
        s += 1;
    }
}

internal void to_lower_s(char *s){
    while(*s){
        if(is_upper(*s)){
            *s = to_lower(*s);
        }
        s += 1;
    }
}

internal char *is_substr(char *s1, char *s2){
    char *result = s1;
    char *str1, *str2;
    if(!*s2) return(result);
    while(*result){
        str1 = result;
        str2 = s2;
        while(*str1 && !(*str1 - *str2)){
            str1 += 1;
            str2 += 1;
        }
        if(!*str2){
            return(result);
        }
        result += 1;
    }
    return(0);
}

internal u32 str_len(char *s){
    char *p = s;
    while(*s){
        s += 1;
    }
    return(s - p);
}

internal s32 str_cmp(char *s1, char *s2){
    s32 result = 0;
    for(; *s1 == *s2;){
        if(*s1 == '\0'){
            return(result);
        }
        s1 += 1;
        s2 += 1;
    }
    result = (*s1 - *s2);
    return(result);
}

internal u32 hexval(char c){
    u32 result = (((c >= '0') && (c <= '9')) ?
                  (c - '0') :
                  (to_lower(c) - 'a' + 10));
    return(result);
}

internal b32 Ccomment_start(char c1, char c2){
    b32 result = ((c1 == '/') &&
                  (c2 == '*'));
    return(result);
}

internal b32 Ccomment_end(char c1, char c2){
    b32 result = ((c1 == '*') &&
                  (c2 == '/'));
    return(result);
}

internal b32 Cppcomment(char c1, char c2){
    b32 result = ((c1 == '/') &&
                  (c2 == '/'));
    return(result);
}

internal b32 white_or_comment(char c1, char c2){
    b32 result = (is_white(c1) ||
                  Ccomment_start(c1, c2) ||
                  Cppcomment(c1, c2));
    return(result);
}

// TODO: Implement.
internal s32 strn_cmp(char *s1, char *s2, u32 count){
}

internal char *read_file_into_mem_and_null_terminate(char *file_name){
    char *result = 0;
    FILE *file = fopen(file_name, "rb");
    if(file){
        mem_index file_size = 0;
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        result = (char *)malloc(file_size + 1); // '\0'
        fread(result, file_size, 1, file);
        result[file_size] = 0;

        fclose(file);
    }
    return(result);
}

internal  String read_file_into_mem(char *file_name){
    String result = {0};
    FILE *file = fopen(file_name, "rb");
    if(file){
        fseek(file, 0, SEEK_END);
        result.count = ftell(file);
        fseek(file, 0, SEEK_SET);

        result.data = (char *)malloc(result.count);
        fread(result.data, result.count, 1, file);

        fclose(file);
    }
    return(result);
}

internal  b32 windows_newline(char c1, char c2){
    b32 result = (((c1 == '\r') && (c2 == '\n')) ||
                  ((c1 == '\n') && (c2 == '\r')));
    return(result);
}

#endif // CSHARED_H

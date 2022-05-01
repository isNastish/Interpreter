

/*
 * Aleksey Yevtushenko
 * February 13, 2022
 */

#if !defined(INTERPRET_H)
#define INTERPRET_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#define internal static
#define global static
#define local_persist static

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef s32 b32;
typedef float f32;
typedef double f64;

typedef size_t mem_index;

#define Assert(expr) assert((expr))
#define U64_MAX _UI64_MAX
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define SizeOf(item) ((char *)(&item + 1) - (char *)(&item)) // FixMe: Doesn't work with types.
#define OffsetOf(type, member) ((mem_index)(&((type *)0)->member)) // '->' has higher precedence than '&'

typedef struct Buffer{
    char *data;
    u32 count;
}Buffer;
typedef Buffer String;


// Returns pointer to the chunk before advance.
static char *advance(Buffer *buf, u32 count){
    char *result = 0;
    if(buf->count >= count){
        result = buf->data;
        buf->data += count;
        buf->count -= count;
    }
    else{
        buf->data += buf->count;
        buf->count = 0;
    }
    return(result);
}

// STUDY: Can we implement the Stack using DynamicArray data structure?
typedef struct DynamicArray{
    mem_index len;
    mem_index cap;
    char arr[1];
}DynamicArray;

#define DynamicArray__header(array) ((DynamicArray *)((char *)(array) - OffsetOf(DynamicArray, arr)))
#define DynamicArray__doesfit(array, count) ((DynamicArray__len(array)+(count)) <= DynamicArray__cap(array))
#define DynamicArray__fit(array, count) (DynamicArray__doesfit(array, count) ? 0 : \
                                         ((array)=DynamicArray__grow(array, (DynamicArray__len(array)+(count)), SizeOf(*(array)))))

#define DynamicArray__push(array, item) (DynamicArray__fit(array, 1),   \
                                         (array)[DynamicArray__len(array)] = (item), \
                                         DynamicArray__header(array)->len += 1)

#define DynamicArray__len(array) ((array) ? (DynamicArray__header(array)->len) : 0)
#define DynamicArray__cap(array) ((array) ? (DynamicArray__header(array)->cap) : 0)
#define DynamicArray__free(array) ((array) ? (free(DynamicArray__header(array)), array = 0) : 0)
#define DynamicArray__pop(arra) // TODO: Implement.

static void *DynamicArray__grow(void *array, mem_index requested_size, mem_index item_size){
    mem_index updated_cap = Max(requested_size, (2 * DynamicArray__cap(array) + 1));
    mem_index size = (updated_cap * item_size + OffsetOf(DynamicArray, arr));
    DynamicArray *darr = 0;
    if(array) // realloc memory.
        darr = (DynamicArray *)realloc(DynamicArray__header(array), size);
    else{ // array was empty, so allocate the memory.
        darr = (DynamicArray *)malloc(size);
        darr->len = 0;
    }
    darr->cap = updated_cap;
    return((void *)darr->arr);
}

#define darr_len(array) DynamicArray__len(array)
#define darr_cap(array) DynamicArray__cap(array)
#define darr_push(array, item) DynamicArray__push(array, item)
// #define darr_pop(array) DynamicArray__pop(array)
#define darr_free(array) DynamicArray__free(array)

// Doubly linked list, probably for stack implementation.
typedef struct Dlist{
    s32 key;
    struct Dlist *next;
    struct Dlist *prev;
}Dlist;

static void dlist_init(Dlist *sentinel);
static void dlist_insert(Dlist *sentinel, Dlist *element);
static void dlist_insert(Dlist *sentinel, Dlist *element);
static void dlist_remove(Dlist *sentinel, Dlist *element);
static Dlist *dlist_search(Dlist *sentinel, s32 key);
static void dlist_dealloc(Dlist **sentinel);

// NOTE: Doubly linked list is not used anywhere for now.

// Init doubly linked list.
static void dlist_init(Dlist *sentinel){
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
}

// Insert the element.
static void dlist_insert(Dlist *sentinel, Dlist *element){
    element->next = sentinel->next;
    sentinel->next->prev = element;
    sentinel->next = element;
    element->prev = sentinel;
}

// Insert at the end of the list.
static void dlist_insert_last(Dlist *sentinel, Dlist *element){
    element->next = sentinel;
    element->prev = sentinel->prev;
    sentinel->prev = element;
    sentinel->prev->next = element;
}

// Remove the element.
static void dlist_remove(Dlist *sentinel, Dlist *element){
    // NOTE: If element->next == 0, it has already being removed.
    if(element->next){
        element->prev->next = element->next;
        element->next->prev = element->prev;
        /* NOTE: we cannot delete one block in a single chunk of memory
           allocated for the whole list.
           We can only zero-init this element->prev and element->next. Not sure about element itself! */
        element->prev = element->next = 0;
    }
}

// Search for an element by given key.
static Dlist *dlist_search(Dlist *sentinel, s32 key){
    Dlist *p = sentinel->next;
    while((p != sentinel) && (p->key != key))
        p = p->next;
    return(p);
}

/* Free the whole list, and zero the sentinel object.
   Should use an Arena, but for now let's use free() call.
   And note that we allocated the list as one single chunk of memory,
   so that's why deallocation is that simple. */
static void dlist_dealloc(Dlist **sentinel){
    if(*sentinel){
        free(*sentinel);
        *sentinel = 0;
    }
}

#if defined(_DEBUG)
static void dlist_print(Dlist *sentinel){
    Dlist *p;
    for(p = sentinel->next;
        p != sentinel;
        p = p->next)
        printf("el address: %p, key:[%d]\n",
               p,
               p->key);
}
#else
static void dlist_print(Dlist *sentinel){}
#endif

#include "cshared.h"
#include "clexer.h"
#include "cparser.h"

/* FixMe: Replace intern_table with global symbol HASH TABLE for lexer.
   Or even better make the symbol table a part of lexer data structure. */
static String string_intern_range(String **intern_table, char *range_start, char *range_end){
    u32 length = (range_end - range_start);
    for(s32 i = 0; (i < darr_len(*intern_table)); i += 1){
        if(!strncmp((*intern_table+i)->data, range_start, length) &&
           ((*intern_table+i)->count == length))
            return(*(*intern_table+i));
    }
    String result = {0};
    result.data = (char *)malloc(length); // FixMe: Rewrite with Arena.
    result.count = length;
    memcpy(result.data, range_start, length); // FixMe: Rewrite with my own strn_cpy() function.
    darr_push(*intern_table, result);
    return(result);
}

// NOTE: for null-terminated strings.
static String string_intern(String **intern_table, char *string){
    String result = {0};
    result = string_intern_range(intern_table, string, (string + str_len(string)));
    return(result);
}

#if defined(_DEBUG)
#include "test.c"
#endif

#endif // INTERPRET_H

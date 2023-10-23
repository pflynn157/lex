//
// This software is licensed under BSD0 (public domain).
// Therefore, this software belongs to humanity.
// See COPYING for more info.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lex.h"

lex *lex_init_string(char *input)
{
    lex *l = malloc(sizeof(lex));
    l->input = malloc(sizeof(char) * strlen(input));
    strcpy(l->input, input);
    
    l->index = 0;
    l->buf_index = 0;
    l->token_stack_len = 0;
    
    for (int i = 0; i<1024; i++)
        l->buffer[i] = 0;
    
    return l;
}

void lex_set_last_buffer(lex *l)
{
    l->last_buf_len = l->buf_index;
    for (int i = 0; i<l->buf_index + 1; i++) {
        l->last_buf[i] = l->buffer[i];
    }
}

void lex_clear_buffer(lex *l)
{
    for (int i = 0; i<l->buf_index + 1; i++) {
        l->buffer[i] = 0;
    }
    
    l->buf_index = 0;
}

int lex_is_symbol(char c)
{
    switch (c) {
        case '(': case ')':
        case '{': case '}':
        case '=':
        case ';':
        case '-': return 1;
        
        default: {}
    }
    return 0;
}

token lex_get_symbol(lex *l, char c)
{
    switch (c) {
        case '(': return t_lparen;
        case ')': return t_rparen;
        case '{': return t_lcbrace;
        case '}': return t_rcbrace;
        case '=': return t_assign;
        case ';': return t_semicolon;
        
        case '-': {
            char c2 = l->input[l->index];
            if (c2 == '>') {
                ++l->index;
                return t_arrow;
            }
            return t_minus;
        } break;
        
        default: {}
    }
    
    return t_none;
}

int lex_is_int(lex *l)
{
    for (int i = 0; i<l->buf_index; i++) {
        if (isdigit(l->buffer[i]) == 0) return 0;
    }

    return 1;
}

int lex_get_int(lex *l)
{
    return atoi(l->last_buf);
}

char *lex_get_id(lex *l)
{
    char *str = malloc(sizeof(char) * (l->last_buf_len + 1));
    int i;
    
    for (i = 0; i<l->last_buf_len; i++) {
        str[i] = l->last_buf[i];
    }
    str[i] = 0;
    
    return str;
}

token lex_get_next(lex *l)
{
    if (l->token_stack_len > 0) {
        token sym = l->token_stack[l->token_stack_len];
        --l->token_stack_len;
        if (sym != t_none)
            return sym;
    }

    if (l->index >= strlen(l->input)) {
        return t_eof;
    }
    
    while (l->index < strlen(l->input)) {
        char c = l->input[l->index];
        ++l->index;
        
        if (c == ' ' || c == '\n' || lex_is_symbol(c)) {
            if (lex_is_symbol(c)) {
                token sym = lex_get_symbol(l, c);
                if (sym == t_none) continue;
                
                if (strlen(l->buffer) > 0) {
                    ++l->token_stack_len;
                    l->token_stack[l->token_stack_len] = sym;
                } else {
                    return sym;
                }
            }
            
            if (strlen(l->buffer) == 0) {
                continue;
            }
            
            if (strcmp(l->buffer, "void") == 0) {
                lex_clear_buffer(l);
                return t_void;
            } else if (strcmp(l->buffer, "int") == 0) {
                lex_clear_buffer(l);
                return t_int;
            } else if (lex_is_int(l)) {
                lex_set_last_buffer(l);
                lex_clear_buffer(l);
                return t_int_literal;
            } else {
                lex_set_last_buffer(l);
                lex_clear_buffer(l);
                return t_id;
            }
        } else {
            l->buffer[l->buf_index] = c;
            ++l->buf_index;
        }
    }
    
    return t_none;
}

void lex_debug(token t, lex *l)
{
    switch (t) {
        case t_eof: puts("EOF"); break;
        
        case t_void: puts("VOID"); break;
        case t_int: puts("INT"); break;
        
        case t_lparen: puts("("); break;
        case t_rparen: puts(")"); break;
        case t_lcbrace: puts("{"); break;
        case t_rcbrace: puts("}"); break;
        case t_assign: puts(":="); break;
        case t_semicolon: puts(";"); break;
        case t_arrow: puts("->"); break;
        case t_minus: puts("-"); break;
        
        case t_id: printf("ID: %s\n", lex_get_id(l)); break;
        case t_int_literal: printf("INT_LITERAL: %d\n", lex_get_int(l)); break;
        
        default: puts("???");
    }
}

void lex_close(lex *l)
{
    free(l);
}


//
// This software is licensed under BSD0 (public domain).
// Therefore, this software belongs to humanity.
// See COPYING for more info.
//
#pragma once

typedef enum {
    t_eof,
    t_none,
    
    t_void,
    t_int,
    
    t_lparen,
    t_rparen,
    t_lcbrace,
    t_rcbrace,
    t_assign,
    t_semicolon,
    t_arrow,
    t_minus,
    
    t_id,
    t_int_literal
} token;

typedef struct {
    char *input;
    char buffer[1024];
    int index;
    int buf_index;
    
    char last_buf[1024];
    int last_buf_len;
    
    token token_stack[10];
    int token_stack_len;
} lex;

lex *lex_init_string(char *input);
void lex_set_last_buffer(lex *l);
void lex_clear_buffer(lex *l);
int lex_is_symbol(char c);
token lex_get_symbol(lex *l, char c);
int lex_is_int(lex *l);
int lex_get_int(lex *l);
char *lex_get_id(lex *l);
token lex_get_next(lex *l);
void lex_debug(token t, lex *l);
void lex_close(lex *l);


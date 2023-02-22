#include <stdio.h>

#include "lex.h"

int main() {
    char *input = "void main()\n { int x = 20; x -> y; 2 - 3; }\n";
    lex *l = lex_init_string(input);
    
    printf("INPUT: %s\n==================\n", l->input);
    token t = lex_get_next(l);
    while (t != t_eof) {
        lex_debug(t, l);
        t = lex_get_next(l);
    }
    
    lex_close(l);
    return 0;
}


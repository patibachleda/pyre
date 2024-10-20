#include "include/token.h"
#include <stdlib.h>
#include <stdio.h>

token_T* init_token(int type, char* value, int line) {
     token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
     printf("creating token type %d: %p\n", type, token);
     printf("creating value %s: %p\n", value, value);
     if (token) {
          token->type = type;
          token->value = value;
          token->line = line;
     }

     return token;
}
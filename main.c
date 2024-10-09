#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"


char* get_file_contents(const char* filepath)
{
     char* buffer = 0;
     long length;

     FILE* f = fopen(filepath, "rb");

     if (f)
     {
          fseek(f, 0, SEEK_END);
          length = ftell(f);
          fseek(f, 0, SEEK_SET);

          buffer = calloc(length, length);

          if (buffer)
               fread(buffer, 1, length, f);

          fclose(f);
          return buffer;
     }

     printf("Error reading file %s\n", filepath);
     exit(2);
}

int main(int argc, char* argv[]) {
     if (argc >= 2) {
          for (int i = 1; i < argc; i++) {
               int len = strlen(argv[i]);
               char* last_four = &argv[i][len - 5];
               if (strcmp(last_four, ".pyre") == 0) {
                    lexer_T* lexer = init_lexer(
                         get_file_contents(argv[i])
                    );

                    //token_T* token = (void*)0;

                    //while ((token = lexer_get_next_token(lexer)) != (void*)0) {
                    //     printf("token %d %s \n", token->type, token->value);
                    //}

                    parser_T* parser = init_parser(lexer);
                    ast_T** list = parser_parse_statements(parser);

                    for (int i = 0; i < 10; i++) {
                         printf("ast %s %s", list[i]->type, list[i]->data);
                    }

                    free(parser);
                    free(list);

               }
               else {
                    printf("Usage: pyre <filename>\n");
                    return 1;
               }
          }
     }
     else {
          printf("Welcome to the PYRE language v. 0.0.1! ");
     }

     return 0;
}
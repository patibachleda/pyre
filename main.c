#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/token.h"


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

//printlist(ast_T** list) {
//     printf("%d\n", list[0]->type);
//     printf("%s\n", list[0]->data.ast_main.body[0]->data.ast_variable_definition.value);
//     printf("%s\n", list[0]->data.ast_main.body[1]->data.ast_variable_definition.value);
//}

int main(int argc, char* argv[]) {
     if (argc >= 2) {
          for (int i = 1; i < argc; i++) {
               int len = (int) strlen(argv[i]);
               char* last_four = &argv[i][len - 5];
               if (strcmp(last_four, ".pyre") == 0) {
                    char* file = get_file_contents(argv[i]);
                    lexer_T* lexer = init_lexer(file);

                    //token_T* token = (void*)0;

                    //while ((token = lexer_get_next_token(lexer)) != (void*)0) {
                    //     printf("token %d %s \n", token->type, token->value);
                    //}

                    parser_T* parser = init_parser(lexer);
                    ast_T** list = parser_parse_statements(parser);

                    int list_size = sizeof(list) / sizeof(list[0]);

                    for (int i = 0; i < list_size; i++) {
                         free(list[i]);
                    }

                    free(file);
                    free(lexer);
                    free(parser);
                    free(list);

                    //int num = 1;
                    //ast_T* curr = list[0];

                    //while (list != NULL) {
                    //     free(curr);
                    //     curr = list[num];
                    //     num += 1;
                    //}

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
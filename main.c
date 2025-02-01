#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/token.h"
#include "include/visitor.h"


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
               int len = (int) strlen(argv[i]);
               char* last_four = &argv[i][len - 5];
               if (strcmp(last_four, ".pyre") == 0) {
                    char* file = get_file_contents(argv[i]);
                    int list_size = 0;

                    lexer_T* lexer = init_lexer(file);
                    parser_T* parser = init_parser(lexer);
                    ast_T** list = parser_parse_statements(parser, &list_size);
                    visitor_visit(list[0]);
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
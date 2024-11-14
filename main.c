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

void traverse_list(ast_T** nodeList, int numNodes) {
     for (int i = 0; i < numNodes; i++) {
          ast_T* node = nodeList[i];
          // Process the node based on its type
          switch (node->type) {
          case AST_MAIN:
                printf("Processing AST_MAIN\n");
               traverse_list(node->token.ast_main.body, node->numNodes); // Recursively process the body
               //printf("freeing token type %d: %p\n", node->type, node->token.ast_main.body);
               free(node->token.ast_main.body);
               free(node);
               break;
          case AST_PROCESS_DEFINITION:
               printf("Processing AST_PROCESS_DEFINITION\n");
               printf("Processing AST_FUNC_DEFINITION\n");
               traverse_list(node->token.ast_process_definition.func->token.ast_func_definition.body, node->token.ast_process_definition.func->numNodes);
               traverse_list(node->token.ast_process_definition.helpers, node->numNodes);
               
               free(node->token.ast_process_definition.func->token.ast_func_definition.body);
               free(node->token.ast_process_definition.func);
               free(node->token.ast_process_definition.helpers);
               free(node->token.ast_process_definition.name);

               free(node);
               break;
          case AST_PROCESS_CALL:
               printf("Processing AST_PROCESS_CALL\n");
               free(node->token.ast_process_call.name);
               free(node);
               break;
          case AST_HELPER_DEFINITION:
                printf("Processing AST_HELPER_DEFINITION: %s\n", node->token.ast_helper_definition.name);
               traverse_list(node->token.ast_helper_definition.body, node->numNodes);
               free(node->token.ast_helper_definition.name);
               free(node->token.ast_helper_definition.body);
               free(node);
               break;
          case AST_VARIABLE_DEFINITION:
                printf("Processing AST_VARIABLE_DEFINITION: %s\n", node->token.ast_variable_definition.name);
               //printf("freeing token type %s: %p\n", node->token.ast_variable_definition.type, node->token.ast_variable_definition.type);
               free(node->token.ast_variable_definition.type);
               //printf("freeing token type %s: %p\n", node->token.ast_variable_definition.name, node->token.ast_variable_definition.name);
               free(node->token.ast_variable_definition.name);
               //printf("freeing token type %d: %p\n", node->token.ast_variable_definition.value->type, node->token.ast_variable_definition.value);
               free(node->token.ast_variable_definition.value);
               free(node);
               break;
          case AST_EMIT:
               printf("Processing AST_EMIT\n");
               ast_T* stmt[] = { node->token.ast_emit.stmt };
               traverse_list(stmt, 1); // Recursively process the body
               ////printf("freeing token type %d: %p\n", node->type, node->token.ast_main.body);
               //free(node->token.ast_main.body);
               free(node);
               break;
          case AST_INT:
               printf("Processing AST_INT: %d\n", node->token.ast_int);
               //traverse_list(node->token.ast_main.body, node->numNodes); // Recursively process the body
               ////printf("freeing token type %d: %p\n", node->type, node->token.ast_main.body);
               //free(node->token.ast_main.body);
               free(node);
               break;
          default:
               // printf("Unknown node type\n");
               break;
          }
     }
}

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
                    //     printf("token %d %s %d \n", token->type, token->value, token->line);
                    //}

                    parser_T* parser = init_parser(lexer);
                    int list_size = 0;
                    ast_T** list = parser_parse_statements(parser, &list_size);
                    
                    traverse_list(list, list_size);
                
                    free(file);
                    free(lexer);
                    free(parser);
                    free(list);

                    printf("complete");

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
#include "include/ast.h"

ast_T* init_ast(int type) {
     ast_T* ast_node = calloc(1, sizeof(ast_T));

     if (ast_node) {
          ast_node->type = type;
     }

     ast_node->local_scope = (void*)0;

     return ast_node;
}
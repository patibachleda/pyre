#include "include/ast.h"

ast_T* init_ast(int type) {
     ast_T* ast_node = calloc(1, sizeof(struct AST_T*));
     ast_node->type = type;

     ast_node->scope = (void*)0;

     return ast_node;
}
#include "include/ast.h"

ast_T* init_ast(int type) {
     ast_T* ast_node = calloc(1, sizeof(struct AST_STRUCT));
     ast_node->type = type;

     ast_node->scope = (void*)0;

     //   AST_MAIN,
     ast_node->main = (void*)0;

     //   AST_PROCESS_DEFINITION,
     ast_node->process_defintion_name = (void*)0;
     ast_node->process_defintion_args = (void*)0;
     ast_node->process_definition_args_size = 0;
     ast_node->process_defintion_body = (void*)0;


     //   AST_PROCESS_CALL,
     ast_node->process_call_name = (void*)0;
     ast_node->process_call_args = (void*)0;
     ast_node->process_call_args_size = 0;

     //   AST_HELPER_DEFINITION,
     ast_node->helper_definition_name = (void*)0;
     ast_node->helper_definition_args = (void*)0;
     ast_node->helper_definition_args_size = 0;
     ast_node->helper_definition_body = (void*)0;


     //   AST_HELPER_CALL,
     ast_node->helper_call_name = (void*)0;
     ast_node->helper_call_args = (void*)0;
     ast_node->helper_call_args_size = 0;

     //   AST_STATEMENT,
     ast_node->statement_name = (void*)0;
     ast_node->statement_value = (void*)0;

     //   AST_EMIT,
     ast_node->emit_name = (void*)0;
     ast_node->emit_value = (void*)0;

     //   AST_VARIABLE_DEFINITION,
     ast_node->variable_definition_name = (void*)0;
     ast_node->variable_definition_value = (void*)0;

     //   AST_VARIABLE,
     ast_node->variable_name_value = (void*)0;

     //   AST_CONDITIONAL,
     ast_node->conditional_name = (void*)0;
     ast_node->conditional_value = (void*)0;

     //   AST_COMPARISON_OPERATOR,
     ast_node->comparison_operator_value = (void*)0;

     //   AST_BOOLEAN_LITERAL,
     ast_node->boolean_literal_value = (void*)0;

     //   AST_TYPE,
     ast_node->type_value = (void*)0;

     //   AST_STRING,
     ast_node->string_value = (void*)0;;

     /*   AST_LIST_OF_NODES */
     ast_node->list_of_nodes = (void*)0;
     ast_node->list_of_nodes_size = 0;

}
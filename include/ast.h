#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct AST_STRUCT {
     enum {
          AST_MAIN,
          AST_PROCESS_DEFINITION,
          AST_FUNCTION_DEFINITION,
          AST_PROCESS_CALL,
          AST_HELPER_DEFINITION,
          AST_HELPER_CALL,
          AST_STATEMENT,
          AST_EMIT,
          AST_VARIABLE_DEFINITION,
          AST_VARIABLE,
          AST_CONDITIONAL,
          AST_BOOLEAN_EXPRESSION,
          AST_BOOLEAN_FACTOR,
          AST_COMPARISON,
          AST_COMPARISON_OPERATOR,
          AST_BOOLEAN_LITERAL,
          AST_EXPRESSION,
          AST_TERM,
          AST_FACTOR,
          AST_ARG_LIST,
          AST_NAMED_ARG,
          AST_TYPE,
          AST_STRING,
          AST_LIST_OF_NODES,
          AST_NOOP
     } type;

     struct SCOPE_STRUCT* scope;

     //   AST_MAIN,
     struct AST_STRUCT* main;

     //   AST_PROCESS_DEFINITION,
     char* process_defintion_name;
     struct AST_STRUCT* process_defintion_args;
     size_t process_definition_args_size;
     struct AST_STRUCT* process_defintion_body;

     //   AST_PROCESS_CALL,
     char* process_call_name;
     struct AST_STRUCT* process_call_args;
     size_t process_call_args_size;


     //   AST_HELPER_DEFINITION,
     char* helper_definition_name;
     struct AST_STRUCT* helper_definition_args;
     size_t helper_definition_args_size;
     struct AST_STRUCT* helper_definition_body;

     //   AST_HELPER_CALL,
     char* helper_call_name;
     struct AST_STRUCT* helper_call_args;
     size_t helper_call_args_size;

     //   AST_STATEMENT,
     char* statement_name;
     struct AST_STRUCT* statement_value;

     //   AST_EMIT,
     char* emit_name;
     struct AST_STRUCT* emit_value;

     //   AST_VARIABLE_DEFINITION,
     char* variable_definition_name;
     struct AST_STRUCT* variable_definition_value;

     //   AST_VARIABLE,
     char* variable_name_value;

     //   AST_CONDITIONAL,
     char* conditional_name;
     struct AST_STRUCT* conditional_value;

     //   AST_BOOLEAN_EXPRESSION,
     char* boolean_expression_name;
     struct AST_STRUCT* boolean_expression_value;

     //   AST_BOOLEAN_FACTOR,
     char* boolean_factor_name;
     struct AST_STRUCT* boolean_factor_value;

     //   AST_COMPARISON,
     char* comparison_name;
     struct AST_STRUCT* comparison_value;

     //   AST_COMPARISON_OPERATOR,
     char* comparison_operator_value;

     //   AST_BOOLEAN_LITERAL,
     char* boolean_literal_value;

     //   AST_EXPRESSION,
     char* expression_name;
     struct AST_STRUCT* expression_value;

     //   AST_TERM,
     char* term_name;
     struct AST_STRUCT* term_value;

     //   AST_FACTOR,
     char* factor_name;
     struct AST_STRUCT* factor_value;

     //   AST_ARG_LIST,
     char* arg_list_name;
     struct AST_STRUCT* arg_list_value;

     //   AST_NAMED_ARG,
     char* named_arg_name;
     struct AST_STRUCT* named_arg_value;

     //   AST_TYPE,
     char* type_value;

     //   AST_STRING,
     char* string_value;

     /*   AST_LIST_OF_NODES */
     struct AST_STRUCT** list_of_nodes;
     size_t list_of_nodes_size;

}ast_T;

ast_T* init_ast(int type);
#endif

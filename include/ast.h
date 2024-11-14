#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct AST_NODE ast_T;

struct AST_NODE {
     enum {
          AST_MAIN,
          AST_PROCESS_DEFINITION,
          AST_FUNC_DEFINITION,
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
          AST_INT,
          AST_LIST_OF_NODES,
          AST_NOOP
     } type;

     union Token {
          struct AST_MAIN { ast_T** body; } ast_main;
          struct AST_PROCESS_DEFINITION { char* name; ast_T* func;  ast_T** helpers; } ast_process_definition;
          struct AST_FUNC_DEFINITION { ast_T** body; } ast_func_definition;
          struct AST_HELPER_DEFINITION { char* name; ast_T** body; } ast_helper_definition;
          struct AST_VARIABLE_DEFINITION { char* type; char* name; ast_T* value; } ast_variable_definition;
          struct AST_VARIABLE { char* name; } ast_variable;
          struct AST_BINARY { char* operator; ast_T* left; ast_T* right; } ast_binary;

          struct AST_PROCESS_CALL { char* name; ast_T** arguments;  } ast_process_call;
          struct AST_HELPER_CALL { char* name; ast_T** arguments; } ast_helper_call;
          struct AST_EMIT { ast_T* stmt; } ast_emit;

          //struct AST_CONDITIONAL { struct AST_BOOLEAN };
          //struct AST_COMPARISON {};
          //struct AST_COMPARISON_OPERATOR {};
          
          //struct AST_STATEMENT {};
          //struct AST_EXPRESSION {};

          //struct AST_BOOLEAN_EXPRESSION {};
          //struct AST_BOOLEAN_FACTOR {};
       
          //struct AST_ARG_LIST {};
          //struct AST_NAMED_ARG {};

          char* string_literal;
          char character_literal;
          int ast_int;
          double doub_literal;
          bool boolean_literal;
     } token;

     int numNodes;
};


ast_T* init_ast(int type);
#endif

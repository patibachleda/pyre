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
          //struct AST_PROCESS_CALL {};
          //struct AST_HELPER_DEFINITION {};
          //struct AST_HELPER_CALL {};
          //struct AST_STATEMENT {};
          //struct AST_EMIT {};
          struct AST_VARIABLE_DEFINITION { char* type; char* name; ast_T* value; } ast_variable_definition;
          struct AST_VARIABLE { char* name; } ast_variable;
          struct AST_BINARY { char* operator; ast_T* left; ast_T* right; } ast_binary;
          //struct AST_CONDITIONAL { struct AST_BOOLEAN };
          //struct AST_BOOLEAN_EXPRESSION {};
          //struct AST_BOOLEAN_FACTOR {};
          //struct AST_COMPARISON {};
          //struct AST_COMPARISON_OPERATOR {};
          //struct AST_EXPRESSION {};
          //struct AST_TERM {};
          //struct AST_FACTOR {};
          //struct AST_ARG_LIST {};
          //struct AST_NAMED_ARG {};
          //struct AST_TYPE {
          //     union data_type { char* string; char character; int integer; double doub; bool boolean; };
          //} ast_variable_type;
          struct AST_HELPER_DEFINITION { char* name; ast_T** body; } ast_helper_definition;

          char* string_literal;
          char character_literal;
          int integer_literal;
          double doub_literal;
          bool boolean_literal;
          //struct AST_LIST_OF_NODES {};
          //struct AST_NOOP {};
     } token;

     int numNodes;
     struct SCOPE_STRUCT* scope;
};


ast_T* init_ast(int type);
#endif

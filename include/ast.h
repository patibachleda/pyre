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
          AST_HELPER_DEFINITION,
          AST_VARIABLE_DEFINITION,
          AST_VARIABLE,

          AST_PROCESS_CALL,
          //AST_HELPER_CALL,
          AST_EMIT,

          AST_CONDITIONAL,

          AST_EXPRESSION,
          AST_EQUALITY,
          AST_COMPARISON,
          AST_TERM,
          AST_FACTOR,
          AST_UNARY,

          AST_ARG_LIST,
          AST_NAMED_ARG,
          AST_UNNAMED_ARG,
          AST_DECLARED_ARG,

          AST_STRING,
          AST_CHARACTER,
          AST_INT,
          AST_DOUBLE,
          AST_BOOLEAN,

          AST_NOOP
     } type;

     struct SCOPE_STRUCT* scope;

     union Token {
          struct AST_MAIN { ast_T** body; } ast_main;
          struct AST_PROCESS_DEFINITION { char* return_type; char* name; ast_T* args;  ast_T* func;  ast_T** helpers; } ast_process_definition;
          struct AST_FUNC_DEFINITION { ast_T** body; } ast_func_definition;
          struct AST_HELPER_DEFINITION { char* return_type; char* name; ast_T* args; ast_T** body; } ast_helper_definition;
          struct AST_VARIABLE_DEFINITION { char* type; char* name; ast_T* value; } ast_variable_definition;
          struct AST_VARIABLE { char* name; } ast_variable;

          struct AST_PROCESS_CALL { char* name; ast_T* args; } ast_process_call;
          //struct AST_HELPER_CALL { char* name; ast_T** arguments; } ast_helper_call;
          struct AST_EMIT { ast_T* stmt; } ast_emit;

          struct AST_CONDITIONAL { ast_T* condition; ast_T** then_stmts; int num_thens; int num_elses; ast_T** else_stmts; } ast_conditional; // !=, ==
          
          // from crafting interpretters
          struct AST_EXPRESSION { ast_T* left; char* operator; ast_T* right; } ast_expression;
          struct AST_EQUALITY { ast_T* left; char* operator; ast_T* right; } ast_equality;
          struct AST_COMPARISON { ast_T* left; char* operator; ast_T* right; } ast_comparison; // ">" | ">=" | "<" | "<="
          struct AST_TERM { ast_T* left; char* operator; ast_T* right; } ast_term; // + -
          struct AST_FACTOR { ast_T* left; char* operator; ast_T* right; } ast_factor; // * /
          struct AST_UNARY { char* operator; ast_T* stmt; } ast_unary; // !

          struct AST_ARG_LIST { ast_T** args; } ast_arg_list;
          struct AST_NAMED_ARG { char* name; ast_T* expression; } ast_named_arg;
          struct AST_UNNAMED_ARG { ast_T* expression; } ast_unnamed_arg;
          struct AST_DECLARED_ARG { char* type;  char* name; } ast_declared_arg;

          char* ast_string;
          char ast_character;
          int ast_int;
          double ast_double;
          bool ast_boolean;
     } token;

     int numNodes;
};


ast_T* init_ast(int type);
#endif

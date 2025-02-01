#ifndef SCOPE_H
#define SCOPE_H
#include "AST.h"

typedef struct SCOPE_STRUCT
{
     ast_T** function_definitions;
     size_t function_definitions_size;

     ast_T** variable_definitions;
     size_t variable_definitions_size;
} scope_T;

scope_T* init_scope();

ast_T* scope_add_function_definition(scope_T* scope, ast_T* fdef);

ast_T* scope_get_function_definition(scope_T* scope, const char* fname);

ast_T* scope_add_variable_definition(scope_T* scope, ast_T* vdef);

ast_T* scope_get_variable_definition(scope_T* scope, const char* name);
#endif
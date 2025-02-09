#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/scope.h"


scope_T* init_scope() {
     scope_T* scope = calloc(1, sizeof(struct SCOPE_STRUCT));

     if (scope) {
          scope->variable_definitions = (void*) 0;
          scope->variable_definitions_size = 0;

          scope->function_definitions = (void*)0;
          scope->function_definitions_size = 0;
     }

     return scope;
}

ast_T* scope_add_function_definition(scope_T* scope, ast_T* fdef){
}

ast_T* scope_get_function_definition(scope_T* scope, const char* fname) {

}

ast_T* scope_add_variable_definition(scope_T* scope, ast_T* vdef) {
    
     if (scope->variable_definitions == (void*) 0) {
          scope->variable_definitions = calloc(1, sizeof(struct AST_NODE));
          scope->variable_definitions[0] = vdef;
          scope->variable_definitions_size += 1;
     }
     else {
          scope->variable_definitions_size += 1;
          scope->variable_definitions = realloc(scope->variable_definitions, scope->variable_definitions_size * sizeof(struct AST_NODE*));
          scope->variable_definitions[scope->variable_definitions_size - 1] = vdef;
     }

     return vdef;
}

ast_T* scope_get_variable_definition(scope_T* scope, const char* name) {
     for (int i = 0; i < scope->variable_definitions_size; i++)
     {
          ast_T* vdef = scope->variable_definitions[i];

          if (strcmp(vdef->token.ast_variable_definition.name, name) == 0)
          {
               return vdef;
          }
     }

     return (void*)0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/scope.h"


scope_T* init_scope() {
     scope_T* scope = calloc(1, sizeof(struct SCOPE_STRUCT));

     if (scope) {
          scope->process_definitions = (void*)0;
          scope->process_definitions_size = 0;

          scope->variable_definitions = (void*) 0;
          scope->variable_definitions_size = 0;

          scope->function_definitions = (void*)0;
          scope->function_definitions_size = 0;
     }

     return scope;
}

ast_T* scope_add_process_definition(scope_T* scope, ast_T* pdef) {
     if (scope->process_definitions == (void*)0) {
          scope->process_definitions = calloc(1, sizeof(struct AST_NODE*));
          scope->process_definitions[0] = pdef;
          scope->process_definitions_size += 1;
     }
     else {
          scope->process_definitions_size += 1;
          scope->process_definitions = realloc(scope->process_definitions, scope->process_definitions_size * sizeof(struct AST_NODE*));
          scope->process_definitions[scope->process_definitions_size - 1] = pdef;
     }

     return pdef;
}

ast_T* scope_get_process_definition(scope_T* global_scope, scope_T* local_scope, const char* pname) {
     for (int i = 0; i < global_scope->process_definitions_size; i++)
     {
          ast_T* pdef = global_scope->process_definitions[i];

          if (strcmp(pdef->token.ast_process_definition.name, pname) == 0)
          {
               return pdef;
          }
     }

     for (int i = 0; i < local_scope->function_definitions_size; i++)
     {
          ast_T* pdef = local_scope->function_definitions[i];

          if (strcmp(pdef->token.ast_helper_definition.name, pname) == 0)
          {
               return pdef;
          }
     }

     return (void*)0;
}

ast_T* scope_add_function_definition(scope_T* scope, ast_T* fdef){
     //for (int i = 0; i < scope->function_definitions_size; i++)
     //{
     //     ast_T* temp = scope->function_definitions[i];

     //     if (strcmp(temp->token.ast_fu.name, fdef->token.) == 0)
     //     {
     //          printf("Variable already declared %s\n", fdef->token.ast_function_definition.name);
     //          exit(1);
     //     }
     //}

     if (scope->function_definitions == (void*)0) {
          scope->function_definitions = calloc(1, sizeof(struct AST_NODE));
          scope->function_definitions[0] = fdef;
          scope->function_definitions_size += 1;
     }
     else {
          scope->function_definitions += 1;
          scope->function_definitions = realloc(scope->function_definitions, scope->function_definitions_size * sizeof(struct AST_NODE*));
          scope->function_definitions[scope->function_definitions_size - 1] = fdef;
     }

     return fdef;
}

ast_T* scope_get_function_definition(scope_T* scope, const char* fname) {
     for (int i = 0; i < scope->variable_definitions_size; i++)
     {
          ast_T* fdef = scope->variable_definitions[i];

          if (strcmp(fdef->token.ast_variable_definition.name, fname) == 0)
          {
               return fdef;
          }
     }

     return (void*)0;
}

ast_T* scope_add_variable_definition(scope_T* scope, ast_T* vdef) {

     for (int i = 0; i < scope->variable_definitions_size; i++)
     {
          ast_T* temp = scope->variable_definitions[i];

          if (strcmp(temp->token.ast_variable_definition.name, vdef->token.ast_variable_definition.name) == 0)
          {
               temp->token.ast_variable_definition.value = vdef->token.ast_variable_definition.value;
               //if (temp->token.ast_variable_definition.value->type == AST_PROCESS_CALL) {
               //     temp->token.ast_variable_definition.value = vdef->token.ast_variable_definition.value;
               //}
               //else {
               //     printf("Variable already declared %s\n", vdef->token.ast_variable_definition.name);
               //     exit(1);
               //}
          }
     }
    
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

ast_T* scope_add_emit_variable(scope_T* scope, ast_T* emit_def, ast_T* emit_value) {

     for (int i = 0; i < scope->variable_definitions_size; i++)
     {
          ast_T* temp = scope->variable_definitions[i];

          //if (strcmp(temp->token.ast_emit.stmt->token, emit_def->token.ast_emit.var_name) == 0)
          //{
          //     temp->token.ast_emit.stmt = emit_def->token.ast_emit.stmt;
          //}
     }

     if (scope->variable_definitions == (void*)0) {
          scope->variable_definitions = calloc(1, sizeof(struct AST_NODE));
          scope->variable_definitions[0] = emit_value;
          scope->variable_definitions_size += 1;
     }
     else {
          scope->variable_definitions_size += 1;
          scope->variable_definitions = realloc(scope->variable_definitions, scope->variable_definitions_size * sizeof(struct AST_NODE*));
          scope->variable_definitions[scope->variable_definitions_size - 1] = emit_value;
     }

     return emit_def;
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
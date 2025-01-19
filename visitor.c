#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/visitor.h"

void visitor_visit(ast_T** nodeList, int numNodes) {
     if (nodeList == NULL || nodeList[0] == NULL) { return; }

     for (int i = 0; i < numNodes; i++) {
          ast_T* node = nodeList[i];
          // Process the node based on its type
          switch (node->type) {
               case AST_MAIN: visitor_visit_main(node);  break;
               case AST_PROCESS_DEFINITION:visitor_visit_process_definition(node); break;
               case AST_HELPER_DEFINITION: visitor_visit_helper_definition(node); break;
               case AST_VARIABLE_DEFINITION: visitor_visit_variable_definition(node); break;
               case AST_VARIABLE: visitor_visit_variable(node); break;
               case AST_PROCESS_CALL: visitor_visit_process_call(node);  break;
               case AST_EMIT: visitor_visit_emit(node); break;
               case AST_CONDITIONAL: visitor_visit_conditional(node); break;
               case AST_EXPRESSION: visitor_visit_expression(node); break;
               case AST_EQUALITY: visitor_visit_equality(node); break;
               case AST_COMPARISON: visitor_visit_comparison(node); break;
               case AST_TERM: visitor_visit_term(node); break;
               case AST_FACTOR: visitor_visit_factor(node); break;
               case AST_UNARY: visitor_visit_unary(node);  break;
               case AST_ARG_LIST: visitor_visit_arg_list(node); break;
               case AST_INT: printf("Processing AST_INT: %d\n", node->token.ast_int); break;
               case AST_DOUBLE: printf("Processing AST_DOUBLE: %f\n", node->token.ast_double); break;
               case AST_BOOLEAN: printf("Processing AST_BOOLEAN: %d\n", node->token.ast_boolean); break;
               case AST_STRING: printf("Processing AST_STRING: %s\n", node->token.ast_string); break;
               case AST_CHARACTER: printf("Processing AST_CHARACTER: %c\n", node->token.ast_character); break;
               default: printf("Unknown node type %d \n", node->type); break;
          }
     }
}

void visitor_visit_main(ast_T* node) {
     printf("Processing AST_MAIN\n");
     visitor_visit(node->token.ast_main.body, node->numNodes); // Recursively process the body
}

void visitor_visit_process_definition(ast_T* node) {
     printf("Processing AST_PROCESS_DEFINITION: %s\n", node->token.ast_process_definition.name);
     printf("Processing AST_FUNC_DEFINITION\n");
     visitor_visit(node->token.ast_process_definition.func->token.ast_func_definition.body, node->token.ast_process_definition.func->numNodes);
     visitor_visit(node->token.ast_process_definition.helpers, node->numNodes);
}

void visitor_visit_helper_definition(ast_T* node) {
     printf("Processing AST_HELPER_DEFINITION: %s\n", node->token.ast_helper_definition.name);
     visitor_visit(node->token.ast_helper_definition.body, node->numNodes);
}

void visitor_visit_variable_definition(ast_T* node) {
     if (node->token.ast_variable_definition.name != NULL) {
          printf("Processing AST_VARIABLE_DEFINITION: %s\n", node->token.ast_variable_definition.name);
     }
     ast_T* stmt[1] = { node->token.ast_variable_definition.value };
     visitor_visit(stmt, 1);
}

void visitor_visit_variable(ast_T* node) {
}

void visitor_visit_process_call(ast_T* node) {
     printf("Processing AST_PROCESS_CALL: %s\n", node->token.ast_process_call.name);
     if (strcmp(node->token.ast_process_call.name, "print") == 0) {
          visitor_visit_arg_list(node);
     }
}

void visitor_visit_emit(ast_T* node) {
     printf("Processing AST_EMIT\n");
     ast_T* stmt[1] = { node->token.ast_emit.stmt };
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_conditional(ast_T* node) {
     printf("Processing AST_CONDITIONAL: \n");
     ast_T* stmt[1] = { node->token.ast_conditional.condition };
     visitor_visit(stmt, 1); // Recursively process the body
     visitor_visit(node->token.ast_conditional.then_stmts, 1); // Recursively process the body
     visitor_visit(node->token.ast_conditional.else_stmts, 1); // Recursively process the body
}

void visitor_visit_expression(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          printf("Processing AST_EXPRESSION: %s\n", node->token.ast_expression.operator);
     }
     ast_T* stmt[1] = { node->token.ast_expression.left };
     visitor_visit(stmt, 1); // Recursively process the body
     stmt[0] = node->token.ast_expression.right;
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_equality(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          printf("Processing AST_EQUALITY: %s\n", node->token.ast_equality.operator);
     }
     ast_T* stmt[1] = { node->token.ast_equality.left };
     visitor_visit(stmt, 1); // Recursively process the body
     stmt[0] = node->token.ast_equality.right;
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_comparison(ast_T* node) {
     if (node->token.ast_comparison.operator != NULL) {
          printf("Processing AST_COMPARISON: %s\n", node->token.ast_comparison.operator);
     }
     ast_T* stmt[1] = { node->token.ast_comparison.left };
     visitor_visit(stmt, 1); // Recursively process the body
     stmt[0] = node->token.ast_comparison.right;
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_term(ast_T* node) {
     if (node->token.ast_term.operator != NULL) {
          printf("Processing AST_TERM: %s\n", node->token.ast_term.operator);
     }
     ast_T* stmt[1] = { node->token.ast_term.left };
     visitor_visit(stmt, 1); // Recursively process the body
     stmt[0] = node->token.ast_term.right;
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_factor(ast_T* node) {
     if (node->token.ast_factor.operator != NULL) {
          printf("Processing AST_FACTOR: %s\n", node->token.ast_factor.operator);
     }
     ast_T* stmt[1] = { node->token.ast_factor.left };
     visitor_visit(stmt, 1); // Recursively process the body
     stmt[0] = node->token.ast_factor.right;
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_unary(ast_T* node) {
     if (node->token.ast_unary.operator != NULL) {
          printf("Processing AST_UNARY: %s\n", node->token.ast_unary.operator);
     }
     ast_T* stmt[1] = { node->token.ast_unary.stmt };
     visitor_visit(stmt, 1); // Recursively process the body
}

void visitor_visit_arg_list(ast_T* node) {
     for (int i = 0; i < node->token.ast_process_call.args->numNodes; i++) {
          ast_T* asList[1];
          switch (node->token.ast_process_call.args->token.ast_arg_list.args[i]->type) {
               case AST_UNNAMED_ARG:
                    asList[0] = node->token.ast_process_call.args->token.ast_arg_list.args[i]->token.ast_unnamed_arg.expression;
                    visitor_visit(asList, 1);
                    break;

               case AST_STRING: printf("%s\n", node->token.ast_process_call.args[i].token.ast_string); break;
               default: printf("%p\n", node->token.ast_process_call.args[i]); break;
          }
     }

}
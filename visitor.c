#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/visitor.h"
#include "include/scope.h"


static pyre_print(ast_T** args, int args_size) {
     for (int i = 0; i < args_size; i++) {
          ast_T* visited_ast = visitor_visit(args[i]);
          switch (visited_ast->type)
          {
               case AST_STRING: printf("%s\n", visited_ast->token.ast_string); break;
               default: printf("%p\n", visited_ast); break;
          }
     }
}

ast_T* visitor_visit(ast_T* node) {
     if ( node == NULL) { return; }

     // Process the node based on its type
     printf("type= %d \n", node->type);
     switch (node->type) {
          case AST_MAIN: visitor_visit_main(node);  break;
          case AST_PROCESS_DEFINITION:visitor_visit_process_definition(node); break;
          case AST_FUNC_DEFINITION:visitor_visit_func_definition(node); break;
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

ast_T* visitor_visit_main(ast_T* node) {
     visitor_visit(node->token.ast_main.body); // Recursively process the body
}

ast_T* visitor_visit_process_definition(ast_T* node) {
     visitor_visit(node->token.ast_process_definition.func->token.ast_func_definition.body);

     for (int i = 0; i < node->numNodes; i++) {
          visitor_visit(node->token.ast_process_definition.helpers[i]);
     }
}

ast_T* visitor_visit_func_definition(ast_T* node) {

}

ast_T* visitor_visit_helper_definition(ast_T* node) {
     for (int i = 0; i < node->numNodes; i++) {
          visitor_visit(node->token.ast_helper_definition.body[i]);
     }

}

ast_T* visitor_visit_variable_definition(ast_T* node) {
     scope_add_variable_definition(
          node->scope,
          node
     );

     //ast_T* stmt[1] = { node->token.ast_variable_definition.value };
//visitor_visit(stmt, 1);

     return node;
}

ast_T* visitor_visit_variable(ast_T* node) {
     ast_T* vdef = scope_get_variable_definition(
          node->scope,
          node->token.ast_variable.name
     );

     if (vdef != (void*)0)
          return visitor_visit( vdef->token.ast_variable_definition.value);

     printf("Undefined variable `%s`\n", node->token.ast_variable.name);
     exit(1);
}

ast_T* visitor_visit_process_call(ast_T* node) {
     if (strcmp(node->token.ast_process_call.name, "print") == 0) {
          pyre_print(node->token.ast_process_call.args, node->token.ast_process_call.args->numNodes);
     }
}

ast_T* visitor_visit_emit(ast_T* node) {
     ast_T* stmt[1] = { node->token.ast_emit.stmt };
     visitor_visit(stmt); // Recursively process the body
}

ast_T* visitor_visit_conditional(ast_T* node) {
     ast_T* stmt[1] = { node->token.ast_conditional.condition };
     visitor_visit(stmt);

     for (int i = 0; i < node->token.ast_conditional.num_thens; i++) {
          visitor_visit(node->token.ast_conditional.then_stmts[i]);
     }

     for (int j = 0; j < node->token.ast_conditional.num_elses; j++) {
          visitor_visit(node->token.ast_conditional.else_stmts[j]);
     }
}

ast_T* visitor_visit_expression(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_term.left);
          ast_T* eval_right = visitor_visit(node->token.ast_term.right);
          if (strcmp(node->token.ast_term.operator, "==") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_int && visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_double || visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_term.operator, "!=") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_int != visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_double != visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_term.left, 1);
}

ast_T* visitor_visit_equality(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_term.left);
          ast_T* eval_right = visitor_visit(node->token.ast_term.right);
          if (strcmp(node->token.ast_term.operator, "and") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_int == visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_double == visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_term.operator, "or") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_int != visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_double != visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_term.left);
}

ast_T* visitor_visit_comparison(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_term.left);
          ast_T* eval_right = visitor_visit(node->token.ast_term.right);
          if (strcmp(node->token.ast_term.operator, "==") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_int == visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_double == visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_term.operator, "!=") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_int != visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_BOOLEAN;
                    node->token.ast_term.left->token.ast_boolean = visitor_visit(node->token.ast_term.left)->token.ast_double != visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_term.left, 1);
}

ast_T* visitor_visit_term(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_term.left, 1);
          ast_T* eval_right = visitor_visit(node->token.ast_term.right, 1);
          if (strcmp(node->token.ast_term.operator, "+") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_INT;
                    node->token.ast_term.left->token.ast_int = visitor_visit(node->token.ast_term.left)->token.ast_int + visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_DOUBLE;
                    node->token.ast_term.left->token.ast_double = visitor_visit(node->token.ast_term.left)->token.ast_double + visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Cannot add types other than int and double on line");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_term.operator, "-") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_INT;
                    node->token.ast_term.left->token.ast_int = visitor_visit(node->token.ast_term.left)->token.ast_int - visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_DOUBLE;
                    node->token.ast_term.left->token.ast_double = visitor_visit(node->token.ast_term.left)->token.ast_double - visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Cannot subtract types other than int and double on line");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_term.left);
}

ast_T* visitor_visit_factor(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_term.left, 1);
          ast_T* eval_right = visitor_visit(node->token.ast_term.right, 1);
          if (strcmp(node->token.ast_term.operator, "*") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_INT;
                    node->token.ast_term.left->token.ast_int = visitor_visit(node->token.ast_term.left)->token.ast_int * visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_DOUBLE;
                    node->token.ast_term.left->token.ast_double = visitor_visit(node->token.ast_term.left)->token.ast_double * visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Cannot multiple types other than int and double on line");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_term.operator, "/") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    node->token.ast_term.left->type = AST_INT;
                    node->token.ast_term.left->token.ast_int = visitor_visit(node->token.ast_term.left)->token.ast_int / visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return node;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    node->token.ast_term.left->type = AST_DOUBLE;
                    node->token.ast_term.left->token.ast_double = visitor_visit(node->token.ast_term.left)->token.ast_double / visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return node;
               }
               else {
                    printf("Cannot divide types other than int and double on line");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_term.left);
}

ast_T* visitor_visit_unary(ast_T* node) {
     if (node->token.ast_unary.operator != NULL) {
          if (strcmp(node->token.ast_term.operator, "!") == 0) {

          }
          else {

          }
     }
     ast_T* stmt[1] = { node->token.ast_unary.stmt };
     visitor_visit(stmt, 1); // Recursively process the body
}

ast_T* visitor_visit_arg_list(ast_T* node) {
     for (int i = 0; i < node->token.ast_process_call.args->numNodes; i++) {
          ast_T* asList[1];
          switch (node->token.ast_process_call.args->token.ast_arg_list.args[i]->type) {
               case AST_UNNAMED_ARG:
                    asList[0] = node->token.ast_process_call.args->token.ast_arg_list.args[i]->token.ast_unnamed_arg.expression;
                    return visitor_visit(asList);
                    break;
               case AST_NAMED_ARG:
                    asList[0] = node->token.ast_process_call.args->token.ast_arg_list.args[i]->token.ast_named_arg.expression;
                    return visitor_visit(asList);
                    break;
               case AST_DECLARED_ARG:
                    break;
               default: printf("%p\n", node->token.ast_process_call.args[i]); break;
          }
     }

}
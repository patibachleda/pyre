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
               case AST_STRING: printf("%s ", visited_ast->token.ast_string); break;
               case AST_CHARACTER: printf("%c ", visited_ast->token.ast_character); break;
               case AST_INT: printf("%d ", visited_ast->token.ast_int); break;
               case AST_DOUBLE: printf("%f ", visited_ast->token.ast_double); break;
               case AST_BOOLEAN: printf("%d ", visited_ast->token.ast_boolean); break;
               default: printf("%p ", visited_ast); break;
          }
     }

     printf("\n");
}

ast_T* visitor_visit(ast_T* node) {
     if ( node == NULL) { return (void*)0; }

     // Process the node based on its type
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
          case AST_UNNAMED_ARG: visitor_visit_unnamed_arg(node); break;

          case AST_INT: 
          case AST_DOUBLE:
          case AST_BOOLEAN:
          case AST_STRING: 
          case AST_CHARACTER:
               return node; break;
          default: printf("Unknown node type %d \n", node->type); break;
     }
}

ast_T* visitor_visit_main(ast_T* node) {
     for (int i = 0; i < node->numNodes; i++) {
          visitor_visit(node->token.ast_main.body[i]);
     }

     return init_ast(AST_NOOP);
}

ast_T* visitor_visit_process_definition(ast_T* node) {
     scope_add_process_definition(
          node->global_scope,
          node
     );

     // add helpers
     for (int i = 0; i < node->numNodes; i++) {
          scope_add_function_definition(
               node->token.ast_process_definition.helpers[i]->process_scope,
               node->token.ast_process_definition.helpers[i]
          );
     }

     return node;
}

ast_T* visitor_visit_func_definition(ast_T* node) {
     scope_add_function_definition(
          node->process_scope,
          node
     );

     return node;
}

ast_T* visitor_visit_helper_definition(ast_T* node) {
     scope_add_function_definition(
          node->process_scope,
          node
     );

     return node;
}

ast_T* visitor_visit_variable_definition(ast_T* node) {
     scope_add_variable_definition(
          node->local_scope,
          node
     );

     return node;
}

ast_T* visitor_visit_variable(ast_T* node) {
     ast_T* vdef = scope_get_variable_definition(
          node->local_scope,
          node->token.ast_variable.name
     );

     if (vdef != (void*)0) {
          return visitor_visit( vdef->token.ast_variable_definition.value);
     }

     printf("Undefined variable `%s`\n", node->token.ast_variable.name);
     exit(1);
}

ast_T* visitor_visit_process_call(ast_T* node) {
     if (strcmp(node->token.ast_process_call.name, "print") == 0) {
          pyre_print(node->token.ast_process_call.args->token.ast_arg_list.args, node->token.ast_process_call.args->numNodes);
          return node;
     }

     ast_T* pdef = scope_get_process_definition(
          node->global_scope,
          node->process_scope,
          node->token.ast_process_call.name
     );

     if (pdef == (void*)0) {

          printf("Undefined method `%s` in program\n", node->token.ast_process_call.name);
          exit(1);
     }

     ast_T* ret_val;

     if (pdef->type == AST_PROCESS_DEFINITION) {
          for (int i = 0; i < pdef->token.ast_process_definition.func->numNodes; i++) {
               ret_val = visitor_visit(pdef->token.ast_process_definition.func->token.ast_func_definition.body[i]);
               if (pdef->token.ast_process_definition.func->token.ast_func_definition.body[i]->type == AST_EMIT) {
                    scope_add_emit_variable(
                         node->local_scope,
                         pdef->token.ast_helper_definition.body[i],
                         ret_val
                    );
                    return ret_val;
               }
          }
          return pdef;
     }
     else if (pdef->type == AST_HELPER_DEFINITION) {
          for (int i = 0; i < pdef->numNodes; i++) {
               ret_val = visitor_visit(pdef->token.ast_helper_definition.body[i]);
               if (pdef->token.ast_helper_definition.body[i]->type == AST_EMIT) {
                    scope_add_emit_variable(
                         node->local_scope,
                         pdef->token.ast_helper_definition.body[i],
                         ret_val
                    );
                    return ret_val->token.ast_variable_definition.value;
               }
          }
          return pdef;
     }
}

ast_T* visitor_visit_emit(ast_T* node) {
     ast_T* stmt[1] = { node->token.ast_emit.stmt };
     return visitor_visit(stmt[0]); // Recursively process the body
}

ast_T* visitor_visit_conditional(ast_T* node) {
     ast_T* stmt[1] = { node->token.ast_conditional.condition };
     ast_T* final = visitor_visit(stmt[0]);

     if (final->token.ast_boolean == true) {
          for (int i = 0; i < node->token.ast_conditional.num_thens; i++) {
               visitor_visit(node->token.ast_conditional.then_stmts[i]);
          }
     }
     else {
          for (int j = 0; j < node->token.ast_conditional.num_elses; j++) {
               visitor_visit(node->token.ast_conditional.else_stmts[j]);
          }
     }
     
     return node;
}

ast_T* visitor_visit_expression(ast_T* node) {
     if (node->token.ast_expression.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_expression.left);
          ast_T* eval_right = visitor_visit(node->token.ast_expression.right);
          if (strcmp(node->token.ast_expression.operator, "and") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_expression.left)->token.ast_int && visitor_visit(node->token.ast_expression.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_BOOLEAN && eval_right->type == AST_BOOLEAN) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_boolean == visitor_visit(node->token.ast_equality.right)->token.ast_boolean;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_expression.left)->token.ast_double && visitor_visit(node->token.ast_expression.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_expression.operator, "or") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_expression.left)->token.ast_int || visitor_visit(node->token.ast_expression.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_expression.left)->token.ast_double || visitor_visit(node->token.ast_expression.right)->token.ast_double;
                    return ret_val;
               }
               else if (eval_left->type == AST_BOOLEAN && eval_right->type == AST_BOOLEAN) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_boolean == visitor_visit(node->token.ast_equality.right)->token.ast_boolean;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an expression");
                    exit(3);
               }
          }
     }

     ast_T* eva = visitor_visit(node->token.ast_expression.left);
     return eva;
}

ast_T* visitor_visit_equality(ast_T* node) {
     if (node->token.ast_equality.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_equality.left);
          ast_T* eval_right = visitor_visit(node->token.ast_equality.right);
          if (strcmp(node->token.ast_equality.operator, "==") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_int == visitor_visit(node->token.ast_equality.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_double == visitor_visit(node->token.ast_equality.right)->token.ast_double;
                    return ret_val;
               }
               else if (eval_left->type == AST_BOOLEAN && eval_right->type == AST_BOOLEAN) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_boolean == visitor_visit(node->token.ast_equality.right)->token.ast_boolean;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an equality");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_equality.operator, "!=") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_int != visitor_visit(node->token.ast_equality.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_double != visitor_visit(node->token.ast_equality.right)->token.ast_double;
                    return ret_val;
               }
               else if (eval_left->type == AST_BOOLEAN && eval_right->type == AST_BOOLEAN) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_equality.left)->token.ast_boolean != visitor_visit(node->token.ast_equality.right)->token.ast_boolean;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an equality");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_equality.left);
}

ast_T* visitor_visit_comparison(ast_T* node) {
     if (node->token.ast_comparison.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_comparison.left);
          ast_T* eval_right = visitor_visit(node->token.ast_comparison.right);
          if (strcmp(node->token.ast_comparison.operator, ">=") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_int >= visitor_visit(node->token.ast_comparison.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_double >= visitor_visit(node->token.ast_comparison.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an comparison");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_comparison.operator, ">") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_int > visitor_visit(node->token.ast_comparison.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_double > visitor_visit(node->token.ast_comparison.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an comparison");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_comparison.operator, "<=") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_int <= visitor_visit(node->token.ast_comparison.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_double <= visitor_visit(node->token.ast_comparison.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an comparison");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_comparison.operator, "<") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_int < visitor_visit(node->token.ast_comparison.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_BOOLEAN);
                    ret_val->token.ast_boolean = visitor_visit(node->token.ast_comparison.left)->token.ast_double < visitor_visit(node->token.ast_comparison.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Types do not match so they cannot be used in an comparison");
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_comparison.left, 1);
}

ast_T* visitor_visit_term(ast_T* node) {
     if (node->token.ast_term.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_term.left, 1);
          ast_T* eval_right = visitor_visit(node->token.ast_term.right, 1);
          if (strcmp(node->token.ast_term.operator, "+") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_INT);
                    ret_val->token.ast_int = visitor_visit(node->token.ast_term.left)->token.ast_int + visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_DOUBLE);
                    ret_val->token.ast_double = visitor_visit(node->token.ast_term.left)->token.ast_double + visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Cannot add types other than int and double on line");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_term.operator, "-") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_INT);
                    ret_val->token.ast_int = visitor_visit(node->token.ast_term.left)->token.ast_int - visitor_visit(node->token.ast_term.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_DOUBLE);
                    ret_val->token.ast_double = visitor_visit(node->token.ast_term.left)->token.ast_double - visitor_visit(node->token.ast_term.right)->token.ast_double;
                    return ret_val;
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
     if (node->token.ast_factor.operator != NULL) {
          ast_T* eval_left = visitor_visit(node->token.ast_factor.left, 1);
          ast_T* eval_right = visitor_visit(node->token.ast_factor.right, 1);
          if (strcmp(node->token.ast_factor.operator, "*") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_INT);
                    ret_val->token.ast_int = visitor_visit(node->token.ast_factor.left)->token.ast_int * visitor_visit(node->token.ast_factor.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_DOUBLE);
                    ret_val->token.ast_double = visitor_visit(node->token.ast_factor.left)->token.ast_double * visitor_visit(node->token.ast_factor.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("Cannot multiple types other than int and double on line");
                    exit(3);
               }
          }
          else if (strcmp(node->token.ast_factor.operator, "/") == 0) {
               if (eval_left->type == AST_INT && eval_right->type == AST_INT) {
                    ast_T* ret_val = init_ast(AST_INT);
                    ret_val->token.ast_int = visitor_visit(node->token.ast_factor.left)->token.ast_int / visitor_visit(node->token.ast_factor.right)->token.ast_int;
                    return ret_val;
               }
               else if (eval_left->type == AST_DOUBLE && eval_right->type == AST_DOUBLE) {
                    ast_T* ret_val = init_ast(AST_DOUBLE);
                    ret_val->token.ast_double = visitor_visit(node->token.ast_factor.left)->token.ast_double / visitor_visit(node->token.ast_factor.right)->token.ast_double;
                    return ret_val;
               }
               else {
                    printf("%d: Cannot divide types other than int and double on line", node);
                    exit(3);
               }
          }
     }

     return visitor_visit(node->token.ast_factor.left);
}

ast_T* visitor_visit_unary(ast_T* node) {
     if (node->token.ast_unary.operator != NULL) {
          if (strcmp(node->token.ast_unary.operator, "!") == 0) {

          }
     }
     ast_T* stmt[1] = { node->token.ast_unary.stmt };
     return visitor_visit(stmt[0]); // Recursively process the body
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

ast_T* visitor_visit_unnamed_arg(ast_T* node) {
     ast_T* expr = node->token.ast_unnamed_arg.expression;
     return visitor_visit(expr);
}
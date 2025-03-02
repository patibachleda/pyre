#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"


ast_T* visitor_visit(ast_T* node);

ast_T* visitor_visit_main(ast_T* node);

ast_T* visitor_visit_process_definition(ast_T* node);

ast_T* visitor_visit_func_definition(ast_T* node);

ast_T* visitor_visit_helper_definition(ast_T* node);

ast_T* visitor_visit_variable_definition(ast_T* node);

ast_T* visitor_visit_variable(ast_T* node);

ast_T* visitor_visit_process_call(ast_T* node);

ast_T* visitor_visit_emit(ast_T* node);

ast_T* visitor_visit_conditional(ast_T* node);

ast_T* visitor_visit_expression(ast_T* node);

ast_T* visitor_visit_equality(ast_T* node);

ast_T* visitor_visit_comparison(ast_T* node);

ast_T* visitor_visit_term(ast_T* node);

ast_T* visitor_visit_factor(ast_T* node);

ast_T* visitor_visit_unary(ast_T* node);

ast_T* visitor_visit_arg_list(ast_T* node);

ast_T* visitor_visit_declared_arg(ast_T* node);

ast_T* visitor_visit_unnamed_arg(ast_T* node, int order, ast_T** process_args);

ast_T* visitor_visit_named_arg(ast_T* node);

#endif
#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

void visitor_visit(ast_T** nodeList, int numNodes);

void visitor_visit_main(ast_T* node);

void visitor_visit_process_definition(ast_T* node);

void visitor_visit_helper_definition(ast_T* node);

void visitor_visit_variable_definition(ast_T* node);

void visitor_visit_process_call(ast_T* node);

void visitor_visit_emit(ast_T* node);

void visitor_visit_conditional(ast_T* node);

void visitor_visit_expression(ast_T* node);

void visitor_visit_equality(ast_T* node);

void visitor_visit_comparison(ast_T* node);

void visitor_visit_term(ast_T* node);

void visitor_visit_factor(ast_T* node);

void visitor_visit_unary(ast_T* node);

void visitor_visit_arg_list(ast_T* node);

void visitor_visit_named_arg(ast_T* node);

void visitor_visit_unnamed_arg(ast_T* node);

void visitor_visit_declared_arg(ast_T* node);

#endif
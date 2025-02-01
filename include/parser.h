#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"

typedef struct PARSER_STRUCT
{
     lexer_T* lexer;
     token_T* current_token;
     token_T* prev_token;
     struct SCOPE_STRUCT* scope;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_move_forward(parser_T* parser, int token_type);

ast_T* parser_parse_statement(parser_T* parser);

ast_T** parser_parse_statements(parser_T* parser, int* ll_size);

ast_T* parser_parse_factor(parser_T* parser);

ast_T* parser_parse_term(parser_T* parser);

ast_T* parser_parse_function_call(parser_T* parser);

ast_T* parser_parse_variable_definition(parser_T* parser, int type);

ast_T* parser_parse_int(parser_T* parser);

ast_T* parser_parse_double(parser_T* parser);

ast_T* parser_parse_string(parser_T* parser);

ast_T* parser_parse_character(parser_T* parser);

ast_T* parser_parse_boolean(parser_T* parser);

ast_T* parser_parse_func_definition(parser_T* parser);

ast_T* parser_parse_process_definition(parser_T* parser);

ast_T* parser_parse_helper_definition(parser_T* parser);

ast_T* parser_parse_id(parser_T* parser);

ast_T* parser_parse_conditional(parser_T* parser);

ast_T* parser_parse_emit(parser_T* parser);

ast_T* parser_parse_main(parser_T* parser);

ast_T* parser_parse_main(parser_T* parser);

ast_T* parser_parse_conditional(parser_T* parser);

ast_T* parser_parse_unary(parser_T* parser);

ast_T* parser_parse_factor(parser_T* parser);

ast_T* parser_parse_term(parser_T* parser);

ast_T* parser_parse_comparison(parser_T* parser);

ast_T* parser_parse_equality(parser_T* parser);

ast_T* parser_parse_expression(parser_T* parser);

ast_T* parser_parse_conditional(parser_T* parser);

ast_T* parser_parse_arguments_declared(parser_T* parser);

ast_T* parser_parse_arguments_called(parser_T* parser);


#endif
#include "parser.h"
#include "include/ast.h"

parser_T* init_parser(lexer_T* lexer) {
     parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
     parser->lexer = lexer;
     parser->current_token = lexer_get_next_token(lexer);
     parser->prev_token = parser->current_token;

     return parser;
}

void parser_eat(parser_T* parser, int token_type) {

}

ast_T* parser_parse(parser_T* parser){
}

ast_T* parser_parse_statement(parser_T* parser){
}

ast_T* parser_parse_statements(parser_T* parser){
}

ast_T* parser_parse_expr(parser_T* parser){
}

ast_T* parser_parse_factor(parser_T* parser){
}

ast_T* parser_parse_term(parser_T* parser){
}

ast_T* parser_parse_function_call(parser_T* parser){
}

ast_T* parser_parse_variable_definition(parser_T* parser){
}

ast_T* parser_parse_function_definition(parser_T* parser){
}

ast_T* parser_parse_variable(parser_T* parser){
}

ast_T* parser_parse_string(parser_T* parser){
}

ast_T* parser_parse_id(parser_T* parser){
}
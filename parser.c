#include "parser.h"
#include "include/ast.h"

parser_T* init_parser(lexer_T* lexer) {
     parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
     parser->lexer = lexer;
     parser->current_token = lexer_get_next_token(lexer);
     parser->prev_token = parser->current_token;

     return parser;
}

void parser_move_forward(parser_T* parser, int token_type) {

     if (parser->current_token->type == token_type) {
          parser->current_token = lexer_get_next_token(parser->lexer);
          parser->prev_token = parser->current_token;
     }
     else {
          printf("Unexpected token %s of type %d", parser->current_token->value, parser->current_token->type);
          exit(1);
     }
}

ast_T* parser_parse_statement(parser_T* parser){
     switch (parser->current_token->type)
     {
          case TOKEN_ID: return parser_parse_id(parser);
     }

     return init_ast(AST_NOOP);
}

ast_T* parser_parse_statements(parser_T* parser){
     ast_T* compound = init_ast(AST_LIST_OF_NODES);
     compound->list_of_nodes = calloc(1, sizeof(struct AST_STRUCT*));

     compound->list_of_nodes[0] = parser_parse_statement(parser);
     compound->list_of_nodes_size += 1;
     
     while (parser->current_token->type == TOKEN_SEMICOLON) {
          parser_move_forward(parser, parser->current_token->type);

          ast_T* statement = parser_parse_statement(parser);

          if (statement) {
               compound->list_of_nodes = realloc(compound->list_of_nodes, compound->list_of_nodes_size * sizeof(struct AST_STRUCT*));
               compound->list_of_nodes[compound->list_of_nodes_size] = statement;
               compound->list_of_nodes_size += 1;
          }
     }

     return compound;
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
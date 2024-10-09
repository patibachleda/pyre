#include "include/parser.h"
#include "include/ast.h"
#include <stdio.h>

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
          case TOKEN_MAIN: return parser_parse_main(parser);
          // Literals
          //case TOKEN_ID: return parser_parse_id(parser); break;
          case TOKEN_INT_TYPE: return parser_parse_int_variable_definition(parser); break;
          //case TOKEN_IS: return parser_parse_conditional(parser); break;
     }

     return init_ast(AST_NOOP);
}

ast_T** parser_parse_statements(parser_T* parser){
     ast_T** ll_nodes = calloc(1, sizeof(struct AST_STRUCT*));
     int ll_size = 0;

     ll_nodes[0] = parser_parse_statement(parser);
     ll_size += 1;
     
     while (parser->current_token->type == TOKEN_SEMICOLON) {
          parser_move_forward(parser, parser->current_token->type);

          ast_T* statement = parser_parse_statement(parser);

          if (statement) {
               ll_nodes = realloc(ll_nodes, ll_size * sizeof(struct AST_STRUCT*));
               ll_nodes[ll_size] = statement;
               ll_size += 1;
          }
     }

     return ll_nodes;
}

//ast_T* parser_parse_expr(parser_T* parser){
//}
//
//ast_T* parser_parse_factor(parser_T* parser){
//}
//
//ast_T* parser_parse_term(parser_T* parser){
//}
//
//ast_T* parser_parse_function_call(parser_T* parser){
//}

ast_T* parser_parse_int_variable_definition(parser_T* parser){
     ast_T* node = init_ast(AST_VARIABLE_DEFINITION);
     node->data.ast_variable_defitinion.type = parser->current_token->value;
     parser_move_forward(parser, TOKEN_INT_TYPE);
     node->data.ast_variable_defitinion.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     node->data.ast_variable_defitinion.value = parser->current_token->value;

     return node;
}

//ast_T* parser_parse_function_definition(parser_T* parser){
//}
//
//ast_T* parser_parse_variable(parser_T* parser){
//}
//
//ast_T* parser_parse_string(parser_T* parser){
//}
//
//ast_T* parser_parse_id(parser_T* parser){
//}
//
//ast_T* parser_parse_conditional(parser_T* parser) {
//}

ast_T* parser_parse_main(parser_T* parser) {
     ast_T* main_node = init_ast(AST_MAIN);
     parser_move_forward(parser, TOKEN_MAIN);
     parser_move_forward(parser, TOKEN_LPAREN);
     parser_move_forward(parser, TOKEN_RPAREN);
     parser_move_forward(parser, TOKEN_LCURLY);

     int num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          main_node->data.ast_main.statements = realloc(main_node->data.ast_main.statements, sizeof(main_node->data.ast_main.statements) * sizeof(struct ast_T*));
          main_node->data.ast_main.statements[num] = statement;
          num += 1;
     }

     return main_node;
}
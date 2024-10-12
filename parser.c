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
          printf("Unexpected token %s of type %d\n", parser->current_token->value, parser->current_token->type);
          exit(1);
     }
}

ast_T* parser_parse_statement(parser_T* parser){
     switch (parser->current_token->type)
     {
          case TOKEN_MAIN: return parser_parse_main(parser);
          // Literals
          //case TOKEN_ID: return parser_parse_id(parser); break;
          case TOKEN_INT_TYPE: return parser_parse_int_variable_definition(parser);
          case TOKEN_PROCESS: return parser_parse_process_definition(parser);
          case TOKEN_FUNC: return parser_parse_func_definition(parser);
          case TOKEN_HELPER: return parser_parse_helper_definition(parser);
          //case TOKEN_IS: return parser_parse_conditional(parser); break;
          case TOKEN_INT:
          case TOKEN_STRING: 
          case TOKEN_DOUBLE:
          case TOKEN_BOOLEAN:
          case TOKEN_CHARACTER:
               return init_ast(AST_INT);
     }

     return init_ast(AST_NOOP);
}

ast_T** parser_parse_statements(parser_T* parser){
     ast_T** ll_nodes = calloc(1, sizeof(ast_T*));
     int ll_size = 0;

     ll_nodes[0] = parser_parse_statement(parser);
     ll_size += 1;
     
     while (parser->current_token->type == TOKEN_SEMICOLON) {
          parser_move_forward(parser, parser->current_token->type);

          ast_T* statement = parser_parse_statement(parser);

          if (statement) {
               ll_nodes = realloc(ll_nodes, (ll_size * sizeof(ast_T*)));
               ll_nodes[ll_size] = statement;
               ll_size += 1;
          }
     }

     return ll_nodes;
}
ast_T* parser_parse_int_variable_definition(parser_T* parser){
     ast_T* node = init_ast(AST_VARIABLE_DEFINITION);
     node->data.ast_variable_definition.type = parser->current_token->value;
     parser_move_forward(parser, TOKEN_INT_TYPE);
     node->data.ast_variable_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     parser_move_forward(parser, TOKEN_EQUALS);
     node->data.ast_variable_definition.value = parser_parse_statement(parser);
     node->data.ast_variable_definition.value->data.integer_literal = atoi(parser->current_token->value);
     parser_move_forward(parser, TOKEN_INT);
     parser_move_forward(parser, TOKEN_SEMICOLON);

     return node;
}


ast_T* parser_parse_main(parser_T* parser) {
     ast_T* main_node = init_ast(AST_MAIN);
     parser_move_forward(parser, TOKEN_MAIN);
     parser_parse_arguments_definition(parser);

     int num = 0;
     ast_T** statement_list = malloc(sizeof(ast_T*));
     ast_T* statement = parser_parse_statement(parser);
     main_node->data.ast_main.body = calloc(1, sizeof(ast_T*));
     main_node->data.ast_main.body[num] = statement;
     num += 1;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          //main_node->data.ast_main.body = realloc(main_node->data.ast_main.body, (num + 1) * sizeof(ast_T*));
          main_node->data.ast_main.body[num] = statement;
          num += 1;
     }

     return main_node;
}

ast_T* parser_parse_process_definition(parser_T* parser) {
     ast_T* process_node = init_ast(AST_PROCESS_DEFINITION);

     parser_move_forward(parser, TOKEN_PROCESS);
     process_node->data.ast_process_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);

     parser_parse_arguments_definition(parser);

     int num = 0;
     process_node->data.ast_process_definition.body = malloc(sizeof(ast_T*));
     ast_T* func_node = parser_parse_func_definition(parser);
     process_node->data.ast_process_definition.body[num] = func_node;
     // free(func_node);
     num += 1;

     // parse helper methods
     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* helper_node = parser_parse_helper_definition(parser);
          process_node->data.ast_process_definition.body = realloc(process_node->data.ast_process_definition.body, (num + 1) * sizeof(ast_T*));
          process_node->data.ast_process_definition.body[num] = helper_node;
          num += 1;
     }

     return process_node;
}

ast_T* parser_parse_func_definition(parser_T* parser) {
     ast_T* func_node = init_ast(AST_FUNC_DEFINITION);
     parser_move_forward(parser, TOKEN_FUNC);
     parser_parse_arguments_definition(parser);

     int num = 0;
     ast_T** statement_list = malloc(sizeof(ast_T*));
     ast_T* statement = parser_parse_statement(parser);
     func_node->data.ast_main.body = calloc(1, sizeof(ast_T*));
     func_node->data.ast_main.body[num] = statement;
     num += 1;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          func_node->data.ast_main.body = realloc(func_node->data.ast_main.body, (num + 1) * sizeof(ast_T*));
          func_node->data.ast_main.body[num] = statement;
          num += 1;
     }

     parser_move_forward(parser, TOKEN_RCURLY);

     return func_node;
}

ast_T* parser_parse_helper_definition(parser_T* parser) {
     ast_T* helper_node = init_ast(AST_HELPER_DEFINITION);
     parser_move_forward(parser, TOKEN_HELPER);
     helper_node->data.ast_helper_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     parser_parse_arguments_definition(parser);

     int num = 0;
     ast_T** statement_list = malloc(sizeof(ast_T*));
     ast_T* statement = parser_parse_statement(parser);
     helper_node->data.ast_helper_definition.body = calloc(1, sizeof(ast_T*));
     helper_node->data.ast_helper_definition.body[num] = statement;
     num += 1;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          helper_node->data.ast_helper_definition.body = realloc(helper_node->data.ast_helper_definition.body, (num + 1) * sizeof(ast_T*));
          helper_node->data.ast_helper_definition.body[num] = statement;
          num += 1;
     }

     return helper_node;
}

ast_T* parser_add_statements_to_node(ast_T* node, parser_T* parser) {
     int num = 0;
     ast_T** statement_list = malloc(sizeof(ast_T*));
     ast_T* statement = parser_parse_statement(parser);
     node->data.ast_main.body = calloc(1, sizeof(ast_T*));
     node->data.ast_main.body[num] = statement;
     num += 1;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          node->data.ast_main.body = realloc(node->data.ast_main.body, (num + 1) * sizeof(ast_T*));
          node->data.ast_main.body[num] = statement;
          num += 1;
     }
}

void parser_parse_arguments_definition(parser_T* parser) {
     parser_move_forward(parser, TOKEN_LPAREN);
     parser_move_forward(parser, TOKEN_RPAREN);
     parser_move_forward(parser, TOKEN_LCURLY);
}
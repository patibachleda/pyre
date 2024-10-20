#include "include/parser.h"
#include "include/ast.h"
#include <stdio.h>

parser_T* init_parser(lexer_T* lexer) {
     parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));

     if (parser) {
          parser->lexer = lexer;
          parser->current_token = lexer_get_next_token(lexer);
          parser->prev_token = parser->current_token;
     }
     return parser;
}


void parser_move_forward(parser_T* parser, int token_type) {
     if (parser->current_token->type == token_type) {
          parser->prev_token = parser->current_token;

          // this is kind of ugly, why are these even necessary in the parser?
          if ( parser->current_token->type == TOKEN_SEMICOLON ||
               parser->current_token->type == TOKEN_PROCESS ||
               parser->current_token->type == TOKEN_FUNC ||
               parser->current_token->type == TOKEN_HELPER ||
               parser->current_token->type == TOKEN_MAIN ||
               parser->current_token->type == TOKEN_EQUALS ||
               parser->current_token->type == TOKEN_RCURLY ||
               parser->current_token->type == TOKEN_LCURLY ||
               parser->current_token->type == TOKEN_RPAREN ||
               parser->current_token->type == TOKEN_LPAREN ||
               parser->current_token->type == TOKEN_EQUALS )
          {
               printf("freeing token type %d: %p\n", parser->prev_token->type, parser->prev_token);
               printf("freeing value %s: %p\n", parser->prev_token->value, parser->prev_token->value);
               free(parser->prev_token->value);
               free(parser->prev_token);
               parser->prev_token = NULL;
          }

          parser->current_token = lexer_get_next_token(parser->lexer);
     }
     else {
          fprintf(stderr, "Unexpected token %s of type %d on line %d\n", parser->current_token->value, parser->current_token->type, parser->current_token->line);
          exit(1);
     }
}

ast_T** parser_parse_statements(parser_T* parser, int* ll_size) {
     ast_T** ll_nodes = calloc(1, sizeof(ast_T*));
     *ll_size = 0;

     while (parser->current_token->type != TOKEN_RCURLY) { 
          ast_T* statement = parser_parse_statement(parser);
          ll_nodes = realloc(ll_nodes, ((*ll_size+1) * sizeof(ast_T*)));
          ll_nodes[*ll_size] = statement;
          *ll_size += 1;
     }

     // if main is not the last in ll_nodes then this will break because process already handles the last curly
     // you should think of a way to exit while loop diff than last curly
     printf("freeing value %s: %p\n", parser->current_token->value, parser->current_token->value);
     free(parser->current_token->value);
     free(parser->current_token);

     return ll_nodes;
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
               return parser_parse_int(parser);
     }

     return init_ast(AST_NOOP);
}

ast_T* parser_parse_int(parser_T* parser) {
     ast_T* node = init_ast(AST_INT);
     node->token.ast_int = atoi(parser->current_token->value);
     free(parser->current_token->value);

     return node;
}


ast_T* parser_parse_int_variable_definition(parser_T* parser){
     ast_T* node = init_ast(AST_VARIABLE_DEFINITION);
     node->token.ast_variable_definition.type = parser->current_token->value;
     parser_move_forward(parser, TOKEN_INT_TYPE);
     printf("freeing token type %d: %p\n", parser->prev_token->type, parser->prev_token);
     free(parser->prev_token);
     node->token.ast_variable_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     printf("freeing token type %d: %p\n", parser->prev_token->type, parser->prev_token);
     free(parser->prev_token);
     parser_move_forward(parser, TOKEN_EQUALS);
     node->token.ast_variable_definition.value = parser_parse_statement(parser);
     parser_move_forward(parser, TOKEN_INT);
     printf("freeing token type %d: %p\n", parser->prev_token->type, parser->prev_token);
     free(parser->prev_token);
     parser_move_forward(parser, TOKEN_SEMICOLON);
     node->numNodes = 0;

     return node;
}

ast_T* parser_parse_main(parser_T* parser) {
     ast_T* main_node = init_ast(AST_MAIN);
     parser_move_forward(parser, TOKEN_MAIN);
     parser_parse_arguments_definition(parser);

     int num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          main_node->token.ast_main.body = realloc(main_node->token.ast_main.body, (num + 1) * sizeof(ast_T*));
          main_node->token.ast_main.body[num] = statement;
          num += 1;
          main_node->numNodes = num;
     }

     return main_node;
}

ast_T* parser_parse_process_definition(parser_T* parser) {
     ast_T* process_node = init_ast(AST_PROCESS_DEFINITION);

     parser_move_forward(parser, TOKEN_PROCESS);

     process_node->token.ast_process_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     printf("freeing token type %d: %p\n", parser->prev_token->type, parser->prev_token);
     free(parser->prev_token);

     parser_parse_arguments_definition(parser);
     // maybe i want this more generic, think about once you get everything how you can optimize
     // the code. generic = use parser_parse_statement
 
     // each process is only allowed one func so no need to keep track
     process_node->token.ast_process_definition.func = parser_parse_func_definition(parser);

     // parse helper methods
     int num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          process_node->token.ast_process_definition.helpers = realloc(process_node->token.ast_process_definition.helpers, (num + 1) * sizeof(ast_T*));
          process_node->token.ast_process_definition.helpers[num] = parser_parse_helper_definition(parser); // if there's no helpers this will break
          num += 1;
          process_node->numNodes = num; // how many helpers a process node has
     }

     parser_move_forward(parser, TOKEN_RCURLY);

     return process_node;
}

ast_T* parser_parse_func_definition(parser_T* parser) {
     ast_T* func_node = init_ast(AST_FUNC_DEFINITION);
     parser_move_forward(parser, TOKEN_FUNC);
     parser_parse_arguments_definition(parser);

     int num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          func_node->token.ast_func_definition.body = realloc(func_node->token.ast_func_definition.body, (num + 1) * sizeof(ast_T*));
          func_node->token.ast_func_definition.body[num] = statement;
          num += 1;
          func_node->numNodes = num;
     }

     parser_move_forward(parser, TOKEN_RCURLY);

     return func_node;
}

ast_T* parser_parse_helper_definition(parser_T* parser) {
     ast_T* helper_node = init_ast(AST_HELPER_DEFINITION);
     parser_move_forward(parser, TOKEN_HELPER);
     helper_node->token.ast_helper_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     printf("freeing token type %d: %p\n", parser->prev_token->type, parser->prev_token);
     free(parser->prev_token);
     parser_parse_arguments_definition(parser);

     int num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          helper_node->token.ast_helper_definition.body = realloc(helper_node->token.ast_helper_definition.body, (num + 1) * sizeof(ast_T*));
          helper_node->token.ast_helper_definition.body[num] = statement;
          num += 1;
          helper_node->numNodes = num;
     }

     parser_move_forward(parser, TOKEN_RCURLY);

     return helper_node;
}

void parser_parse_arguments_definition(parser_T* parser) {
     parser_move_forward(parser, TOKEN_LPAREN);
     parser_move_forward(parser, TOKEN_RPAREN);
     parser_move_forward(parser, TOKEN_LCURLY);
}
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

ast_T** parser_parse_statements(parser_T* parser, int* ll_size) {
     ast_T** ll_nodes = calloc(1, sizeof(ast_T*));
     *ll_size = 0;

     while (parser->current_token->type != TOKEN_RCURLY) { 
          ast_T* statement = parser_parse_statement(parser);
          ll_nodes = realloc(ll_nodes, ((*ll_size+1) * sizeof(ast_T*)));
          ll_nodes[*ll_size] = statement;
          *ll_size += 1;
     }

     return ll_nodes;
}

ast_T* parser_parse_statement(parser_T* parser){
     switch (parser->current_token->type)
     {
          case TOKEN_MAIN: return parser_parse_main(parser);
          // Literals
          case TOKEN_ID: return parser_parse_id(parser); break;
          case TOKEN_INT_TYPE:
          case TOKEN_DOUBLE_TYPE:
          case TOKEN_BOOLEAN_TYPE:
          case TOKEN_STRING_TYPE:
          case TOKEN_CHAR_TYPE:
               return parser_parse_variable_definition(parser, parser->current_token->type);
          case TOKEN_PROCESS: return parser_parse_process_definition(parser);
          case TOKEN_FUNC: return parser_parse_func_definition(parser);
          case TOKEN_HELPER: return parser_parse_helper_definition(parser);
          case TOKEN_EMIT: return parser_parse_emit(parser);
          case TOKEN_IS: return parser_parse_conditional(parser);
          case TOKEN_EXPRESSION_INT:
          case TOKEN_EXPRESSION_DOUBLE: 
          case TOKEN_EXPRESSION_STRING:
          case TOKEN_EXPRESSION_CHAR:
          case TOKEN_EXPRESSION_BOOLEAN:
               return parser_parse_expression(parser);
          case TOKEN_INT:
               return parser_parse_int(parser);
          case TOKEN_STRING: 
               return parser_parse_string(parser);
          case TOKEN_DOUBLE:
               return parser_parse_double(parser);
          case TOKEN_CHARACTER:
               return parser_parse_character(parser);
          case TOKEN_BOOLEAN:
               return parser_parse_boolean(parser);
     }

     return init_ast(AST_NOOP);
}

// --------------------------------------------------------------------------
//                          VARIABLE DECLARATIONS
// <Type> ::= "int" | "double" |  "boolean" | "string" |  "char" | IDENTIFIER
// <VarDec> ::= <Type> IDENTIFIER = <Expression>
// --------------------------------------------------------------------------
ast_T* parser_parse_variable_definition(parser_T* parser, int type) {
     ast_T* node = init_ast(AST_VARIABLE_DEFINITION);
     node->token.ast_variable_definition.type = parser->current_token->value;
     parser_move_forward(parser, type);
     node->token.ast_variable_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);
     parser_move_forward(parser, TOKEN_EQUALS);
     node->token.ast_variable_definition.value = parser_parse_statement(parser);
     parser_move_forward(parser, TOKEN_SEMICOLON);
     node->numNodes = 0;

     return node;
}

// --------------------------------------------------------------------------
//                              PARSER MOVING
// --------------------------------------------------------------------------
void parser_move_forward(parser_T* parser, int token_type) {
     if (parser->current_token->type == token_type) {
          parser->prev_token = parser->current_token;
          parser->current_token = lexer_get_next_token(parser->lexer);
     }
     else {
          fprintf(stderr, "Unexpected token %s of type %d on line %d\n", parser->current_token->value, parser->current_token->type, parser->current_token->line);
          exit(1);
     }
}

// --------------------------------------------------------------------------
//                          LITERAL DEFINITIONS
// --------------------------------------------------------------------------
ast_T* parser_parse_int(parser_T* parser) {
     ast_T* node = init_ast(AST_INT);
     node->token.ast_int = atoi(parser->current_token->value);
     parser_move_forward(parser, TOKEN_INT);

     return node;
}

ast_T* parser_parse_double(parser_T* parser) {
     ast_T* node = init_ast(AST_DOUBLE);
     node->token.ast_double = atoi(parser->current_token->value);
     parser_move_forward(parser, TOKEN_DOUBLE);

     return node;
}

ast_T* parser_parse_string(parser_T* parser) {
     ast_T* node = init_ast(AST_STRING);
     node->token.ast_string = parser->current_token->value;
     parser_move_forward(parser, TOKEN_STRING);

     return node;
}

ast_T* parser_parse_character(parser_T* parser) {
     ast_T* node = init_ast(AST_CHARACTER);
     node->token.ast_character = parser->current_token->value[0];
     parser_move_forward(parser, TOKEN_CHARACTER);

     return node;
}

ast_T* parser_parse_boolean(parser_T* parser) {
     ast_T* node = init_ast(AST_BOOLEAN);
     node->token.ast_boolean = (strcmp(parser->current_token->value, "true") == 0);
     parser_move_forward(parser, TOKEN_BOOLEAN);

     return node;
}

ast_T* parser_parse_id(parser_T* parser) {
     ast_T* node = NULL;
     char* name = parser->current_token->value; // get the user declared name
     parser_move_forward(parser, TOKEN_ID);

     // process call
     if (parser->current_token->type == TOKEN_LPAREN) {
          node = init_ast(AST_PROCESS_CALL);
          node->token.ast_process_call.name = name;
          node->token.ast_process_call.args = parser_parse_arguments_called(parser);

          if (parser->current_token->type == TOKEN_ARROW) {
               parser_move_forward(parser, TOKEN_ARROW);

          }
          else {
               parser_move_forward(parser, TOKEN_SEMICOLON);
          }
     }
     //else if (parser->prev_token->type == TOKEN_HELPER){
     //}

     // variable
     else {
          node = init_ast(AST_VARIABLE);
          node->token.ast_variable.name = name;
     }

     return node;
}

// --------------------------------------------------------------------------
//                               PROCESSES
// <Process> ::= "process" IDENTIFIER "(" <ArgList> ")" "{" <Function> <Helper>* "}"
// <Func> ::=  <TYPE> "func" "(" ")" "{" <Stmt>* "}"
// <Helper> :: = "helper" < Type > IDENTIFIER "(" < ArgList > ")" "{" < Stmt >* "}"
// --------------------------------------------------------------------------
ast_T* parser_parse_process_definition(parser_T* parser) {
     ast_T* process_node = init_ast(AST_PROCESS_DEFINITION);

     parser_move_forward(parser, TOKEN_PROCESS);

     // return type
     process_node->token.ast_process_definition.return_type = parser->current_token->value;
     parser_move_forward(parser, parser->current_token->type);

     // name
     process_node->token.ast_process_definition.name = parser->current_token->value;
     parser_move_forward(parser, TOKEN_ID);

     // args 
     process_node->token.ast_process_definition.args = parser_parse_arguments_declared(parser);
     // body
     parser_move_forward(parser, TOKEN_LCURLY);
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
     parser_move_forward(parser, TOKEN_LPAREN);
     parser_move_forward(parser, TOKEN_RPAREN);
     parser_move_forward(parser, TOKEN_LCURLY);

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

     helper_node->token.ast_helper_definition.args = parser_parse_arguments_declared(parser);
     parser_move_forward(parser, TOKEN_LCURLY);

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

ast_T* parser_parse_arguments_declared(parser_T* parser) {
     ast_T* node = init_ast(AST_ARG_LIST);

     parser_move_forward(parser, TOKEN_LPAREN);
     int num = 0;

     while (parser->current_token->type != TOKEN_RPAREN) {
          node->token.ast_arg_list.args = realloc(node->token.ast_arg_list.args, (num + 1) * sizeof(ast_T*));
          ast_T* arg = init_ast(AST_DECLARED_ARG);
          arg->token.ast_declared_arg.type = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
          arg->token.ast_declared_arg.name = parser->current_token->value;
          node->token.ast_arg_list.args[num] = arg;
          parser_move_forward(parser, TOKEN_ID);
          num += 1;
          node->numNodes = num;

          if (parser->current_token->type == TOKEN_COMMA) {
               parser_move_forward(parser, TOKEN_COMMA);
          }
     }


     parser_move_forward(parser, TOKEN_RPAREN);

     return node;
}

ast_T* parser_parse_arguments_called(parser_T* parser) {
     ast_T* node = init_ast(AST_ARG_LIST);

     parser_move_forward(parser, TOKEN_LPAREN);
     int num = 0;

     while (parser->current_token->type != TOKEN_RPAREN) {
          node->token.ast_arg_list.args = realloc(node->token.ast_arg_list.args, (num + 1) * sizeof(ast_T*));
          node->token.ast_arg_list.args[num]->token.ast_named_arg.name = parser->current_token->value;
          parser_move_forward(parser, TOKEN_ID);
          parser_move_forward(parser, TOKEN_COLON);
          ast_T* statement = parser_parse_statement(parser);
          node->token.ast_arg_list.args[num]->token.ast_named_arg.expression = statement;
          num += 1;
          node->numNodes = num;
     }

     parser_move_forward(parser, TOKEN_RPAREN);

     return node;
}

// --------------------------------------------------------------------------
//                                 MAIN()
// <Main> ::= "main" "(" ")" "{" <Stmt>* "}"
// --------------------------------------------------------------------------

ast_T* parser_parse_main(parser_T* parser) {
     ast_T* main_node = init_ast(AST_MAIN);
     parser_move_forward(parser, TOKEN_MAIN);

     parser_move_forward(parser, TOKEN_LPAREN);
     parser_move_forward(parser, TOKEN_RPAREN);

     parser_move_forward(parser, TOKEN_LCURLY);

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

// --------------------------------------------------------------------------
//                            EXPRESSIONS
//  <Expr> ::= <Equality> (("and" | "or") <Equality>)
//  <Equality> ::= <Comparison> (("!=" | "==") <Comparison>)
//  <Comparison> ::= <Term> ((">" | ">=" | "<" | "<=" ) <Term>)
//  <Term> ::= <Factor> (("+" | "-") <Factor>)
//  <Factor> ::= <Unary> (("*" | "/") <Unary>)
//  <Unary> ::= "!" <Expr> | <Literal>
// --------------------------------------------------------------------------
ast_T* parser_parse_expression(parser_T* parser) {
     ast_T* node = init_ast(AST_EXPRESSION);

     node->token.ast_expression.left = parser_parse_equality(parser);

     while (parser->current_token->type == TOKEN_AND || parser->current_token->type == TOKEN_OR) {
          node->token.ast_expression.operator = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
          node->token.ast_expression.right = parser_parse_equality(parser);
     }

     return node;
}

ast_T* parser_parse_equality(parser_T* parser) {
     ast_T* node = init_ast(AST_EQUALITY);

     node->token.ast_equality.left = parser_parse_comparison(parser);

     while (parser->current_token->type == TOKEN_BANG_EQUAL || parser->current_token->type == TOKEN_EQUAL_EQUAL) {
          node->token.ast_equality.operator = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
          node->token.ast_equality.right = parser_parse_comparison(parser);
     }

     return node;
}

ast_T* parser_parse_comparison(parser_T* parser) {
     ast_T* node = init_ast(AST_COMPARISON);

     node->token.ast_comparison.left = parser_parse_term(parser);

     while (parser->current_token->type == TOKEN_GREATER ||
          parser->current_token->type == TOKEN_GREATER_EQUAL ||
          parser->current_token->type == TOKEN_LESS ||
          parser->current_token->type == TOKEN_LESS_EQUAL) {
          node->token.ast_comparison.operator = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
          node->token.ast_comparison.right = parser_parse_term(parser);
     }

     return node;
}

ast_T* parser_parse_term(parser_T* parser) {
     ast_T* node = init_ast(AST_TERM);

     node->token.ast_term.left = parser_parse_factor(parser);

     while (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS) {
          node->token.ast_term.operator = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
          node->token.ast_term.right = parser_parse_factor(parser);
     }

     return node;
}

ast_T* parser_parse_factor(parser_T* parser) {
     ast_T* node = init_ast(AST_FACTOR);

     node->token.ast_term.left = parser_parse_unary(parser);

     while (parser->current_token->type == TOKEN_DIVIDE || parser->current_token->type == TOKEN_STAR) {
          node->token.ast_term.operator = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
          node->token.ast_term.right = parser_parse_unary(parser);
     }

     return node;
}

ast_T* parser_parse_unary(parser_T* parser) {
     ast_T* node = init_ast(AST_UNARY);

     while (parser->current_token->type == TOKEN_BANG) {
          node->token.ast_unary.operator = parser->current_token->value;
          parser_move_forward(parser, parser->current_token->type);
     }

     if (parser->current_token->type == TOKEN_EXPRESSION_INT) {
          parser->current_token->type = TOKEN_INT;
     }
     else if (parser->current_token->type == TOKEN_EXPRESSION_DOUBLE) {
          parser->current_token->type = TOKEN_DOUBLE;
     }
     else if (parser->current_token->type == TOKEN_EXPRESSION_STRING) {
          parser->current_token->type = TOKEN_STRING;
     }
     else if (parser->current_token->type == TOKEN_EXPRESSION_CHAR) {
          parser->current_token->type = TOKEN_CHARACTER;
     }
     else if (parser->current_token->type == TOKEN_EXPRESSION_BOOLEAN) {
          parser->current_token->type = TOKEN_BOOLEAN;
     }

     node->token.ast_unary.stmt = parser_parse_statement(parser);

     return node;
}

// --------------------------------------------------------------------------
//                             OTHER
// --------------------------------------------------------------------------

ast_T* parser_parse_conditional(parser_T* parser) {
     ast_T* node = init_ast(AST_CONDITIONAL);
     parser_move_forward(parser, TOKEN_IS);
     parser_move_forward(parser, TOKEN_LPAREN);


     node->token.ast_conditional.condition = parser_parse_statement(parser);

     parser_move_forward(parser, TOKEN_RPAREN);
     parser_move_forward(parser, TOKEN_LCURLY);

     // then body
     int num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          node->token.ast_conditional.then_stmts = realloc(node->token.ast_conditional.then_stmts, (num + 1) * sizeof(ast_T*));
          node->token.ast_conditional.then_stmts[num] = statement;
          num += 1;
     }

     parser_move_forward(parser, TOKEN_RCURLY);
     parser_move_forward(parser, TOKEN_ELSE);
     parser_move_forward(parser, TOKEN_LCURLY);

     //else body
     num = 0;

     while (parser->current_token->type != TOKEN_RCURLY) {
          ast_T* statement = parser_parse_statement(parser);
          node->token.ast_conditional.else_stmts = realloc(node->token.ast_conditional.else_stmts, (num + 1) * sizeof(ast_T*));
          node->token.ast_conditional.else_stmts[num] = statement;
          num += 1;
     }

     parser_move_forward(parser, TOKEN_RCURLY);

     return node;
}

ast_T* parser_parse_emit(parser_T* parser) {
     ast_T* node = init_ast(AST_EMIT);
     parser_move_forward(parser, TOKEN_EMIT);
     node->token.ast_emit.stmt = parser_parse_statement(parser);
     parser_move_forward(parser, TOKEN_SEMICOLON);
     return node;
}

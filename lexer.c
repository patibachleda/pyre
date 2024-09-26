#define _CRT_SECURE_NO_WARNINGS
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

lexer_T* init_lexer(char* contents) {
     lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));

     lexer->contents = contents;
     lexer->index = 0;
     lexer->current_char = &contents[lexer->index];

     return lexer;
}

// helper methods 
bool is_character_not_null(lexer_T* lexer) {
     return *lexer->current_char != '\0' && lexer->index < strlen(lexer->contents);
}

bool is_next_character_not_null(char character, int index, int contents_len) {
     return character != '\0' && index < contents_len;
}

int get_line_num(lexer_T* lexer) {
     int lineNumber = 1;
     char* target = lexer->current_char;
     char* ptr = lexer->contents;  // Pointer to traverse the buffer

     while (*ptr != '\0') {  // Loop until the end of the string
          if (ptr == target) {
               return lineNumber;  // Return the line number when the pointer matches the target
          }
          if (*ptr == '\n') {
               lineNumber++;  // Increment line number on encountering a newline
          }
          ptr++;  // Move to the next character
     }

     return -1;  // Return -1 if the pointer is not found in the buffer
}

char lexer_look_forward(lexer_T* lexer) {
     if (is_next_character_not_null(lexer->contents[lexer->index + 1], lexer->index + 1, strlen(lexer->contents))) {
          return lexer->contents[(lexer->index+1)];
     }
}

bool is_match_forward(lexer_T* lexer, char expected) {
     if (is_next_character_not_null(lexer->contents[lexer->index + 1], lexer->index + 1, strlen(lexer->contents))) {
          if (lexer->contents[lexer->index + 1] == expected) {
               return true;
          }
     }
     return false;
}

// core methods
void lexer_move_forward(lexer_T* lexer) {
     if (is_character_not_null(lexer)) {
          lexer->index += 1;
          lexer->current_char = &lexer->contents[lexer->index];
     }
}

token_T* lexer_read_forward_with_token(lexer_T* lexer, token_T* token) {
     lexer_move_forward(lexer);
     return token;
}

void lexer_skip_whitespace(lexer_T* lexer) {
     while (isspace(*lexer->current_char)) {
          lexer_move_forward(lexer);
     }
}

token_T* lexer_get_next_token(lexer_T* lexer) {
     while (is_character_not_null(lexer)) {
          lexer_skip_whitespace(lexer);

          if (isalpha(*lexer->current_char)) { //start of a identifier
               return lexer_parse_identifier(lexer);
          }

          if (isdigit(*lexer->current_char)) { //start of a identifier
               return lexer_parse_number(lexer);
          }

          char* current_char_as_string = lexer_get_current_char_as_string(lexer);
          int line = get_line_num(lexer);

          switch (*lexer->current_char) {
               case ',':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_COMMA, current_char_as_string, line));
                    break;
               case '.':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_DOT, current_char_as_string, line));
                    break;
               case '-':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_MINUS, current_char_as_string, line));
                    break;
               case '+':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_PLUS, current_char_as_string, line));
                    break;
               case '*':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_STAR, current_char_as_string, line));
                    break;
               case ';':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_SEMICOLON, current_char_as_string, line));
                    break;
               case '(':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_LPAREN, current_char_as_string, line));
                    break;
               case ')':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_RPAREN, current_char_as_string, line));
                    break;
               case '{':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_LCURLY, current_char_as_string, line));
                    break;
               case '}':
                    return lexer_read_forward_with_token(lexer, init_token(TOKEN_RCURLY, current_char_as_string, line));
                    break;
               case '=': {
                    int type = is_match_forward(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUALS;
                    char* value = type == TOKEN_EQUAL_EQUAL ? "==" : current_char_as_string;
                    return lexer_read_forward_with_token(lexer, init_token(type, value, line));
                    break;
                    }
               case '!': {
                    int type = is_match_forward(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG;
                    char* value = type == TOKEN_BANG_EQUAL ? "!=" : current_char_as_string;
                    return lexer_read_forward_with_token(lexer, init_token(type, value, line));
                    break;
               }
               case '<': {
                    int type = is_match_forward(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS;
                    char* value = type == TOKEN_LESS_EQUAL ? "<=" : current_char_as_string;
                    return lexer_read_forward_with_token(lexer, init_token(type, value, line));
                    break;
               }
               case '>': {
                    int type = is_match_forward(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER;
                    char* value = type == TOKEN_GREATER_EQUAL ? ">=" : current_char_as_string;
                    return lexer_read_forward_with_token(lexer, init_token(type, value, line));
                    break;
               }
               case '/': {
                    if (is_match_forward(lexer, '/')) {
                         while (*lexer->current_char != '\n' && is_next_character_not_null(lexer->contents[lexer->index + 1], lexer->index + 1, strlen(lexer->contents))) {
                              lexer_move_forward(lexer);
                         }
                    }
                    else {
                         return lexer_read_forward_with_token(lexer, init_token(TOKEN_DIVIDE, current_char_as_string, line));
                    }
                    
                    break;
               }
               case '"':
                    return lexer_parse_string(lexer); break;

               default: printf("Unexpected character %c on line %d", *lexer->current_char, line); exit(3); break;
          }
     }

     return (void*)0; //return null to indicate done
}

token_T* lexer_parse_string(lexer_T* lexer) {
     lexer_move_forward(lexer);
     char* string = calloc(1, sizeof(char));
     string[0] = '\0';

     while (*lexer->current_char != '"') { // read everything inside of string
          char* s = lexer_get_current_char_as_string(lexer);
          string = realloc(string, strlen(string) * strlen(s) + 1 * sizeof(char));
          strcat(string, s);

          lexer_move_forward(lexer);
     }

     lexer_move_forward(lexer);

     return init_token(TOKEN_STRING, string, get_line_num(lexer));
}

token_T* lexer_parse_number(lexer_T* lexer) {
     char* string = calloc(1, sizeof(char));
     string[0] = '\0';

     while (isdigit(*lexer->current_char)) { // read everything inside of string
          char* s = lexer_get_current_char_as_string(lexer);
          string = realloc(string, strlen(string) * strlen(s) + 1 * sizeof(char));
          strcat(string, s);

          lexer_move_forward(lexer);
     }

     // for doubles
     if (*lexer->current_char == '.') {
          char* s = lexer_get_current_char_as_string(lexer);
          string = realloc(string, strlen(string) * strlen(s) + 1 * sizeof(char));
          strcat(string, s);

          lexer_move_forward(lexer);
          while (isdigit(*lexer->current_char)) { // read everything inside of string
               char* s = lexer_get_current_char_as_string(lexer);
               string = realloc(string, strlen(string) * strlen(s) + 1 * sizeof(char));
               strcat(string, s);

               lexer_move_forward(lexer);
          }

          return init_token(TOKEN_DOUBLE, string, get_line_num(lexer));
     }

     return init_token(TOKEN_INT, string, get_line_num(lexer));
}

token_T* lexer_parse_identifier(lexer_T* lexer) {
     char* identifier = calloc(1, sizeof(char));
     identifier[0] = '\0';
     int line_num = get_line_num(lexer);

     while (isalnum(*lexer->current_char)) { // read everything inside of string
          char* s = lexer_get_current_char_as_string(lexer);
          identifier = realloc(identifier, (strlen(identifier) + strlen(s) + 1) * sizeof(char));
          strcat(identifier, s);

          lexer_move_forward(lexer);
     }



     return init_token(TOKEN_ID, identifier, line_num);
}

char* lexer_get_current_char_as_string(lexer_T* lexer) {
     char* current_char_as_string = calloc(2, sizeof(char));
     current_char_as_string[0] = *lexer->current_char;
     current_char_as_string[1] = '\0';

     return current_char_as_string;
}
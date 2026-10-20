#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preproc.h"

#define SCRIPT_NAME "preproc"

constexpr char USAGE_STRING[] = 
  "preproc - preprocess MIPS assembly for MARS simulator\n"
  "Usage: %s [ASM FILE]\n"
  ;


static inline const char * const TokenType_to_str(enum TokenType type) {
  switch (type) {
    case TokenType_WHITESPACE:
      return "TokenType_WHITESPACE";
    case TokenType_NEWLINE:
      return "TokenType_NEWLINE";
    case TokenType_PAREN_OPEN:
      return "TokenType_PAREN_OPEN";
    case TokenType_PAREN_CLOSE:
      return "TokenType_PAREN_CLOSE";
    case TokenType_COMMENT:
      return "TokenType_COMMENT";
    case TokenType_MINUS:
      return "TokenType_MINUS";
    case TokenType_COMMA:
      return "TokenType_COMMA";
    case TokenType_INTEGER:
      return "TokenType_INTEGER";
    case TokenType_DIRECTIVE:
      return "TokenType_DIRECTIVE";
    case TokenType_REGISTER:
      return "TokenType_REGISTER";
    case TokenType_INSTRUCTION:
      return "TokenType_INSTRUCTION";
    case TokenType_LABEL:
      return "TokenType_LABEL";
    case TokenType_EOF:
      return "TokenType_EOF";
    case TokenType_UNKNOWN:
      return "TokenType_UNKNOWN";
    }
  return nullptr;
}

static inline const char * const Registers_to_str(enum Registers reg) {
  switch (reg) {
    case Registers_zero:
      return "zero";
    case Registers_at:
      return "at";
    case Registers_v0:
      return "v0";
    case Registers_v1:
      return "v1";
    case Registers_a0:
      return "a0";
    case Registers_a1:
      return "a1";
    case Registers_a2:
      return "a2";
    case Registers_a3:
      return "a3";
    case Registers_t0:
      return "t0";
    case Registers_t1:
      return "t1";
    case Registers_t2:
      return "t2";
    case Registers_t3:
      return "t3";
    case Registers_t4:
      return "t4";
    case Registers_t5:
      return "t5";
    case Registers_t6:
      return "t6";
    case Registers_t7:
      return "t7";
    case Registers_s0:
      return "s0";
    case Registers_s1:
      return "s1";
    case Registers_s2:
      return "s2";
    case Registers_s3:
      return "s3";
    case Registers_s4:
      return "s4";
    case Registers_s5:
      return "s5";
    case Registers_s6:
      return "s6";
    case Registers_s7:
      return "s7";
    case Registers_t8:
      return "t8";
    case Registers_t9:
      return "t9";
    case Registers_k0:
      return "k0";
    case Registers_k1:
      return "k1";
    case Registers_gp:
      return "gp";
    case Registers_sp:
      return "sp";
    case Registers_fp:
      return "fp";
    case Registers_ra:
      return "ra";
    case Registers_float:
      return "float";
    case Registers_coproc:
      return "coproc";
    case Registers_unknown:
      return "unknown";
  }
  return nullptr;
}


Token *Token_new(void) {
  Token *token = malloc(sizeof(Token));
  token->flags = TokenFlag_HEAP_ALLOCATED;
  return token;
}

void Token_free(Token *const token) {
  free(token);
}

static inline void indent_print(FILE *stream, size_t indentLevel) {
  for (size_t i = 0; i < indentLevel; i++) {
    fputc(' ', stream);
  }
}

void Token_print_data(const Token *const token, size_t indentLevel, FILE *stream) {
  indent_print(stream, indentLevel);
  switch (token->type) {
    case TokenType_NEWLINE:
      fputs("\"\\n\"", stream);
      break;
    case TokenType_UNKNOWN:
    case TokenType_WHITESPACE:
    case TokenType_PAREN_OPEN:
    case TokenType_PAREN_CLOSE:
    case TokenType_MINUS:
    case TokenType_COMMA:
      fprintf(stream, "'%c'", token->data.ch);
      break;
    case TokenType_COMMENT:
    case TokenType_INTEGER:
    case TokenType_DIRECTIVE:
    case TokenType_INSTRUCTION:
    case TokenType_LABEL:
      fprintf(stream, "\"%s\"", token->data.str);
      break;
    case TokenType_REGISTER:
      fprintf(stream, "\"%s\"", Registers_to_str(token->data.reg));
      break;
    case TokenType_EOF:
      fputs("[EOF]", stream);
      break;
  }
}

void Token_print(const Token *const token, size_t indentLevel, FILE *stream) {
  indent_print(stream, indentLevel);
  fprintf(stream, "Token {" "\n");

  indent_print(stream, indentLevel);
  fprintf(stream, "  " "type: %s" "\n", TokenType_to_str(token->type));

  indent_print(stream, indentLevel);
  fprintf(stream, "  " "flags: %s" "\n", "");

  indent_print(stream, indentLevel);
  fprintf(stream, "  " "data: ");
  Token_print_data(token, 0, stream);
  fputs("\n", stream);
  
  indent_print(stream, indentLevel);
  fputs("}\n", stream);
}

TokenList *TokenList_new(size_t initialSize) {
  TokenList *list = malloc(sizeof(TokenList));
  list->capacity = initialSize;
  list->tokens = calloc(initialSize, sizeof(Token));
  list->occupied = 0;
  return list;
}

void TokenList_free(TokenList *list) {
  for (size_t i = 0; i < list->occupied; i++) {
    if (list->tokens[i].flags & TokenFlag_DATA_HEAP_ALLOCATED) {
      free(list->tokens[i].data.str);
    }
  }
  free(list->tokens);
  free(list);
}

Token *TokenList_pop(TokenList *list, Token *token) {
  if (list->occupied == 0) {
    return nullptr;
  }
  list->occupied--;
  *token = list->tokens[list->occupied];
  return token;
}

const Token *TokenList_append(TokenList *list, const Token *token) {
  if (list->occupied >= list->capacity) {
    Token *tmp = realloc(list->tokens, list->capacity * 2 * sizeof(Token));
    if (tmp == nullptr) {
      return nullptr;
    }
    list->capacity *= 2;
    list->tokens = tmp;
  }

  if (list->tokens == nullptr) {
    return nullptr;
  }

  list->tokens[list->occupied] = *token;
  list->occupied++;

  return token;
}

void TokenList_print(TokenList *list, size_t indentLevel, FILE *stream) {
  indent_print(stream, indentLevel);
  fputs("TokenList {\n", stream);
  for (size_t i = 0; i < list->occupied; i++) {
    Token_print(&list->tokens[i], indentLevel + 2, stream);
  }
  fputs("}\n", stream);
}

CharStack *CharStack_new(size_t initialSize) {
  CharStack *stack = malloc(sizeof(CharStack));
  stack->capacity = initialSize;
  stack->str = calloc(initialSize, sizeof(char));
  stack->occupied = 0;
  return stack;
}

void CharStack_free(CharStack *stack) {
  free(stack->str);
  free(stack);
}

char CharStack_pop(CharStack *stack) {
  if (stack->occupied == 0) {
    return '\0';
  }
  stack->occupied--;
  char ch = stack->str[stack->occupied];
  return ch;
}

char CharStack_push(CharStack *stack, char ch) {
  constexpr size_t GROWTH_FACTOR = 2;
  if (stack->occupied >= stack->capacity) {
    char *tmp = realloc(stack->str, stack->capacity * sizeof(char) * GROWTH_FACTOR);
    if (tmp == nullptr) {
      return '\0';
    }
    stack->capacity *= GROWTH_FACTOR;
  }
  stack->str[stack->occupied] = ch;
  stack->occupied++;
  return ch;
}



void buffer_clear(size_t bufferSize, char buffer[bufferSize]) {
  memset(buffer, '\0', bufferSize * sizeof(char));
}

static inline size_t gobble_string_space(FILE *stream, size_t bufferSize, char buffer[bufferSize]) {
  char ch = '\0';
  size_t i = 0;
  for (; i < bufferSize && !feof(stream); i++) {
    ch = (char)fgetc(stream);
    if (isblank(ch)) {
      return i;
    }
    buffer[i] = ch;
  }
  return i;
}

// Return Value: `bool` that represents whether a newline was reached
static inline bool gobble_string_newline(FILE *stream, size_t bufferSize, char buffer[bufferSize]) {
  char ch = '\0';
  size_t i = 0;
  for (; i < bufferSize && !feof(stream); i++) {
    ch = (char)fgetc(stream);
    if (ch == '\n') {
      return true;
    }
    buffer[i] = ch;
  }
  return false;
}

static inline CharStack *gobble_string_newline_complete(FILE *stream) {
  constexpr size_t CHARSTACK_INITIAL_SIZE = 512;
  CharStack *stack = CharStack_new(CHARSTACK_INITIAL_SIZE);
  char ch = '\0';
  for (size_t i = 0; !feof(stream); i++) {
    (void)i;
    ch = (char)fgetc(stream);
    if (ch == '\n') {
      return stack;
    }
    CharStack_push(stack, ch);
  }
  return stack;
}

static inline void Token_clear(Token *const token) {
  token->type = TokenType_UNKNOWN;
  token->flags = 0;
  token->data.str = 0;
}

// TODO: Finish
TokenList *tokenize(FILE *stream) {
  TokenList *list = TokenList_new(1024);
  enum TokenType expect = 0;

  constexpr size_t bufferSize = 256;
  static_assert(bufferSize > 1);
  char buffer[bufferSize] = {'\0'};
  
  //constexpr size_t stackSize = 256;
  //static_assert(stackSize > 1);
  //char stack[stackSize]  = {'\0'};
  CharStack * stack = nullptr;

  char ch = '\0';
  Token *token = &(Token){.flags = 0};
  for (size_t i = 0; !feof(stream); i++) {
    ch = (char)fgetc(stream);
    switch (ch) {
      case '\n':
        TokenList_append(list, &TOKEN_NEWLINE);
        break;
      case '\t':
        [[fallthrough]];
      case ' ':
        TokenList_append(list, &TOKEN_WHITESPACE);
        break;
      case '(':
        TokenList_append(list, &TOKEN_PAREN_OPEN);
        break;
      case ')':
        TokenList_append(list, &TOKEN_PAREN_CLOSE);
        break;
      case ',':
        TokenList_append(list, &TOKEN_COMMA);
        break;
      case '.':
        //token->type = TokenType_DIRECTIVE;
        token->type = TokenType_DIRECTIVE;
        token->flags = 0;
        token->data.str = nullptr;

        buffer_clear(bufferSize, buffer);
        buffer[0] = '.';
        bool reachedNewline = gobble_string_newline(stream, bufferSize - 1, buffer);
        token->data.str = strndup(buffer, bufferSize-1);
        token->flags = TokenFlag_DATA_HEAP_ALLOCATED;
        size_t strBufferSize = bufferSize;

        

        while (!reachedNewline) {
          strBufferSize *= 2;
          token->data.str = realloc((void*)token->data.str, strBufferSize);
          gobble_string_newline(stream, strBufferSize - 1, (char*)token->data.str);
          token->flags |= TokenFlag_DATA_HEAP_ALLOCATED;
        }

        TokenList_append(list, token);
        Token_clear(token);
        break;
      case EOF:
        TokenList_append(list, &TOKEN_EOF);
        break;
      case '#':
        stack = gobble_string_newline_complete(stream);
        token->type = TokenType_COMMENT;
        token->flags = TokenFlag_DATA_HEAP_ALLOCATED,
        token->data.str = malloc(sizeof(char) * (stack->occupied+1));
        strncpy(token->data.str, stack->str, stack->occupied);
        token->data.str[stack->occupied] = '\0';
        
        CharStack_free(stack);
        stack = nullptr;

        TokenList_append(list, token);
      default:
        *token = TOKEN_UNKNOWN;
        token->data.ch = ch;
        TokenList_append(list, token);

        Token_clear(token);
        break;
      
    
    }
  }

  return list;
}

void show_usage(int argc, char *argv[argc]) {
  printf(USAGE_STRING, argc == 0? SCRIPT_NAME : argv[0]);
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
    show_usage(argc, argv);
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "r");
  if (file == nullptr) {
    fprintf(stderr, "Failed to open a file: %s", strerror(errno));
    return 1;
  }

  TokenList *list = tokenize(file);
  TokenList_print(list, 0, stdout);

  TokenList_free(list);


  return 0;
}

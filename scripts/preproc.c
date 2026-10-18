#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preproc.h"

#define SCRIPT_NAME "preproc"

constexpr char USAGE_STRING[] = 
  "preproc - preprocess MIPS assembly for MARS simulator\n"
  "Usage: %s [ASM FILE]\n"
  ;


void show_usage(int argc, char *argv[argc]) {
  printf(USAGE_STRING, argc == 0? SCRIPT_NAME : argv[0]);
}

Token *Token_new(void) {
  Token *token = malloc(sizeof(Token));
  token->flags |= TokenFlag_HEAP_ALLOCATED;
  return token;
}

void Token_free(Token *const token) {
  free(token);
}

TokenList *TokenList_new(size_t initialSize) {
  TokenList *list = malloc(sizeof(TokenList));
  list->capacity = initialSize;
  list->tokens = calloc(initialSize, sizeof(Token));
  list->occupied = 0;
  return list;
}

void TokenList_free(TokenList *list) {
  free(list->tokens);
  free(list);
}

Token *TokenList_pop(TokenList *list, Token *token) {
  if (list->occupied == 0) {
    return nullptr;
  }
  list->occupied--;
  if (token == nullptr) {
    return nullptr;
  }
  *token = list->tokens[list->occupied];
  return token;
}

const Token *TokenList_append(TokenList *list, const Token *token) {
  if (list->occupied >= list->capacity) {
    list->capacity *= 2;
    list->tokens = realloc(list->tokens, list->capacity);
  }

  if (list->tokens == nullptr) {
    return nullptr;
  }

  list->tokens[list->occupied] = *token;
  list->occupied++;

  return token;
}

void buffer_cleat(size_t bufferSize, char buffer[bufferSize]) {
  memset(buffer, '\0', bufferSize * sizeof(char));
}
// TODO: Finish
TokenList *tokenize(FILE *stream) {
  TokenList *list = TokenList_new(1024);
  enum TokenType expect = 0;
  char buffer[256] = {'\0'};
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
      case '.':
        expect = TokenType_DIRECTIVE;
        break;


    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    show_usage(argc, argv);
    return EXIT_FAILURE;
  }

    


  return 0;
}

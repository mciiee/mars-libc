#include <stddef.h>
#include <stdint.h>
// MIPS I registers
enum Registers: uint32_t {
  Registers_zero = 0,
  Registers_at = 1,
  Registers_v0 = 2, Registers_v1 = 3,
  Registers_a0 = 4, Registers_a1 = 5, Registers_a2 = 6, Registers_a3 = 7,
  Registers_t0 = 8, Registers_t1 = 9, Registers_t2 = 10, Registers_t3 = 11, Registers_t4 = 12, Registers_t5 = 13, Registers_t6 = 14, Registers_t7 = 15,
  Registers_s0 = 16, Registers_s1 = 17, Registers_s2 = 18, Registers_s3 = 19, Registers_s4 = 20, Registers_s5 = 21, Registers_s6 = 22, Registers_s7 = 23,
  Registers_t8 = 24, Registers_t9 = 25,
  Registers_k0 = 26, Registers_k1 = 27,
  Registers_gp = 28, Registers_sp = 29, Registers_fp = 30, Registers_ra = 31,
  // TODO: Implement Floating-point and coproc registers
  Registers_float = 253,
  Registers_coproc = 254,
  Registers_unknown = 255,
};

enum TokenType: uint16_t {
  TokenType_UNKNOWN = 0,

  TokenType_WHITESPACE,
  TokenType_NEWLINE,

  TokenType_PAREN_OPEN,
  TokenType_PAREN_CLOSE,

  TokenType_COMMENT_START,

  TokenType_MINUS,
  TokenType_COMMA,

  TokenType_INTEGER,

  TokenType_DIRECTIVE,
  TokenType_REGISTER,
  TokenType_INSTRUCTION,
  TokenType_LABEL,


  TokenType_EOF,

};

enum TokenFlags: uint16_t {
  TokenFlag_NONE = 0,
  TokenFlag_HEAP_ALLOCATED = 1,
  TokenFlag_DATA_HEAP_ALLOCATED = 2,
};

union TokenData {
  const char *str;
  char ch;
  enum Registers reg;
};


typedef struct Token {
  enum TokenType type;
  enum TokenFlags flags;
  union TokenData data;
} Token;

typedef struct TokenList {
  Token *tokens;
  size_t capacity;
  size_t occupied;
} TokenList;

static constexpr Token TOKEN_NEWLINE = {
  .type = TokenType_NEWLINE,
  .flags = TokenFlag_NONE,
  .data.ch = '\n',
};

static constexpr Token TOKEN_WHITESPACE = {
  .type = TokenType_NEWLINE,
  .flags = TokenFlag_NONE,
  .data.ch = ' ',
};


static constexpr Token TOKEN_PAREN_OPEN = {
  .type = TokenType_PAREN_OPEN,
  .flags = TokenFlag_NONE,
  .data.ch = '('
};

static constexpr Token TOKEN_PAREN_CLOSE = {
  .type = TokenType_PAREN_CLOSE,
  .flags = TokenFlag_NONE,
  .data.ch = ')'
};

static const Token TOKEN_TEXT_SEGMENT = {
  .type = TokenType_DIRECTIVE,
  .flags = TokenFlag_NONE,
  .data.str = ".text"
};

static const Token TOKEN_DATA_SEGMENT = {
  .type = TokenType_DIRECTIVE,
  .flags = TokenFlag_NONE,
  .data.str = ".data"
};

static const Token TOKEN_KTEXT_SEGMENT = {
  .type = TokenType_DIRECTIVE,
  .flags = TokenFlag_NONE,
  .data.str = ".ktext"
};

static const Token TOKEN_KDATA_SEGMENT = {
  .type = TokenType_DIRECTIVE,
  .flags = TokenFlag_NONE,
  .data.str = ".kdata"
};

static const Token TOKEN_ASCII_DIRECTIVE = {
  .type = TokenType_DIRECTIVE,
  .flags = TokenFlag_NONE,
  .data.str = ".ascii"
};

static const Token TOKEN_ASCIIZ_DIRECTIVE = {
  .type = TokenType_DIRECTIVE,
  .flags = TokenFlag_NONE,
  .data.str = ".asciiz"
};

static const char *ALLOWED_DIRECTIVES[] = {
    "align",
    "ascii",
    "asciiz",
    "byte",
    "data",
    "double",
    "end_macro",
    "eqv",
    // "extern",
    "float",
    "globl",
    "half",
    "include",
    "kdata",
    "ktext",
    "macro",
    // "set",
    "space",
    "text",
    "space",
    "word",
};

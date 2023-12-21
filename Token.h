#include <stdlib.h>
#include <stdio.h>

struct Token{
  char *lexem;
  int token_type;
  char *name;
};

struct TokenList{
  struct Token *array;
  size_t used;
  size_t size;
};

struct Token createToken(const char *name, int tokenType, const char *lexem);
void printToken(struct Token *token);

void initTokenList(struct TokenList *list, size_t init_size);
void freeTokenList(struct TokenList *list);
int addToken(struct TokenList *list, struct Token token);

struct Token * find_last_token_id(struct TokenList *list);
int find_last_assign_token(struct TokenList *list);
int find_last_id_token_with_right_br(struct TokenList *list);

void printTokenList(struct TokenList *list);

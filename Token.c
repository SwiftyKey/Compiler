#include "Token.h"
#include <string.h> 
#include "c++.tab.h"


struct Token createToken(const char *name, int tokenType, const char *lexem){
  struct Token token;
  if (lexem != NULL) {
    token.lexem = malloc((strlen(lexem) + 1)) ;
    strcpy(token.lexem, lexem);
  } else token.lexem = NULL; 
  token.name = malloc((strlen(name) + 1)) ;
  strcpy(token.name, name);
  token.token_type = tokenType;  
  return token;
}

void initTokenList(struct TokenList *list, size_t init_size){
  list->array = malloc(init_size * sizeof(struct Token));
  list->used = 0;
  list->size = init_size;
}

void freeToken(struct Token *token){
  if (token->name)
    free(token->name);
  if(token->lexem)
    free(token->lexem);
}

void freeTokenList(struct TokenList *list){
  free(list->array);
  list->array = NULL;
  list->used = list->size = 0;
}

int addToken(struct TokenList *list, struct Token token){
  if (list->used == list->size) {
    list->size *= 2;
    list->array = realloc(list->array, list->size * sizeof(struct Token));
  }
  list->array[list->used++] = token; 
  return list->used - 1;
}

void printToken(struct Token *token){
  if (token->lexem == NULL)
    printf("%s\n", token->name);
  else
    printf("%s %s\n", token->name, token->lexem);
}

void printTokenList(struct TokenList *list){
  printf("-----------------Tokens-----------------\n");
  for(int i=0;i<list->used;++i){
    printf("%d ", i);
    printToken(&list->array[i]);
  }
}

struct Token * find_last_token_id(struct TokenList *list){
  int size = list->used;
  for(int i=size-1;i>=0;i--)
    if (list->array[i].token_type == TOK_NAME)
      return &list->array[i]; 
  return NULL;
}

int find_last_assign_token(struct TokenList *list){
  int size = list->used;
  for(int i=size-1;i>=0;i--)
    if (list->array[i].token_type == TOK_ASSIGNMENT)
      return i; 
  return -1;
}

int find_last_id_token_with_right_br(struct TokenList *list){
  int size = list->used;
  for(int i=size-1;i>=1;i--)
    if (list->array[i].token_type == TOK_OPEN_BRACKET && 
	list->array[i-1].token_type == TOK_NAME)
      return i-1; 
  return -1;
}


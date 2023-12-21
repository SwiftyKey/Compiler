#include "lists.h"
#include "generator.h"

int yyparse (void);


int main(){
  initSymbolTable(&table, 128);
  initCommandList(&commands, 128);
  initTokenList(&tokens, 128);
  
  initStack(&command_before_while, 128);
  initStack(&last_command_log_expr, 128);
  yyparse();

//  printTokenList(&tokens);
//  printSymbolTable(&table);
//  printCommandList(&commands);
  
  generate(&table, &commands);

  freeCommandList(&commands);
  freeSymbolTable(&table);
  freeTokenList(&tokens);
  freeStack(&command_before_while);
  freeStack(&last_command_log_expr);
  return 0;
}


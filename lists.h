#ifndef __LISTS__
#define __LISTS__

#include "SymbolTable.h"
#include "Command.h"
#include "Token.h"
#include "Stack.h"

extern struct SymbolTable table;
extern struct CommandList commands;
extern struct TokenList tokens;

extern struct Stack command_before_while; 
extern struct Stack last_command_log_expr; 


#endif
#include "lists.h"

struct SymbolTable table;
struct CommandList commands;
struct TokenList tokens;

struct Stack command_before_while;
struct Stack last_command_log_expr;   

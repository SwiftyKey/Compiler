%{
#include <stdio.h>
#include "lists.h"
#define YYDEBUG 1

void yyerror(const char *mes);

int yylex();
%}

%token TOK_INCLUDE TOK_OPEN_FIGURE_BRACKET TOK_CLOSE_FIGURE_BRACKET TOK_OPEN_BRACKET TOK_CLOSE_BRACKET TOK_WHILE TOK_RETURN TOK_END_COMMAND TOK_ELSE_IF TOK_IF TOK_ELSE TOK_SEP_COMMA TOK_ASSIGNMENT TOK_LESS_OR_EQUAL TOK_GREATER_OR_EQUAL TOK_OUTPUT_STREAM TOK_INPUT_STREAM TOK_EQUAL TOK_NOT_EQUAL TOK_LESS TOK_INCREMENT TOK_DECREMENT TOK_GREATER TOK_ADDITION TOK_MULTIPLY TOK_DIFFERENCE TOK_DIVISION TOK_REMAINDER TOK_AND TOK_OR TOK_STRING TOK_DOUBLE_QUOTES TOK_QUOTE TOK_BOOL TOK_INT TOK_FLOAT TOK_DOUBLE TOK_USING TOK_NAMESPACE TOK_ENDL TOK_COUT TOK_CIN TOK_LIBRARY_NAME TOK_REAL_CONST TOK_INT_CONST TOK_NAME TOK_NOT TOK_WHILE

%left TOK_LESS_OR_EQUAL TOK_GREATER_OR_EQUAL TOK_EQUAL TOK_LESS TOK_GREATER TOK_NOT_EQUAL
%left TOK_ADDITION TOK_DIFFERENCE
%left TOK_MULTIPLY TOK_DIVISION TOK_REMAINDER

%left TOK_OR
%left TOK_AND

%right UMINUS TOK_NOT

%%

start : libs namespace main | error {yyerror("OOPS\n");};

libs : TOK_INCLUDE TOK_LIBRARY_NAME libs 
       |;

namespace : TOK_USING TOK_NAMESPACE TOK_NAME TOK_END_COMMAND namespace |;

main : TOK_INT TOK_NAME TOK_OPEN_BRACKET TOK_CLOSE_BRACKET TOK_OPEN_FIGURE_BRACKET body TOK_CLOSE_FIGURE_BRACKET;

body : cons body  {$$ = $1;}|;

cons : var_def | assign TOK_END_COMMAND | cout | cin | while| if | expr TOK_END_COMMAND | TOK_RETURN expr TOK_END_COMMAND;

var_def : type TOK_NAME TOK_END_COMMAND {
       const char *var_name = tokens.array[tokens.used - 2].lexem;
       addItem(&table, create_var($1, var_name));
};

assign : type TOK_NAME TOK_ASSIGNMENT expr {
       int assign_index = find_last_assign_token(&tokens);
       int id_index = assign_index-1;
       struct SymbolTableItem var = create_var($1, tokens.array[id_index].lexem); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("ASSIGNMENT", ASSIGNMENT, $4, 0, res_index);
       $$ = addCommand(&commands, command);
}
       | TOK_NAME TOK_ASSIGNMENT expr {
       int assign_index = find_last_assign_token(&tokens);
       int id_index = findVarByName(&table, tokens.array[assign_index-1].lexem);
       struct Command command = createCommand("ASSIGNMENT", ASSIGNMENT, $3, 0, id_index);
       $$ = addCommand(&commands, command);
};

type : TOK_INT {$$ = INT;}
     | TOK_FLOAT {$$ = FLOAT;}
     | TOK_DOUBLE {$$ = DOUBLE;};

cin : TOK_CIN TOK_INPUT_STREAM expr TOK_END_COMMAND;

cout : TOK_COUT TOK_OUTPUT_STREAM TOK_ENDL TOK_END_COMMAND | TOK_COUT TOK_OUTPUT_STREAM expr TOK_END_COMMAND;

expr : expr TOK_ADDITION expr {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$3].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("ADD", ADD, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
} 
     | expr TOK_DIFFERENCE expr {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$3].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("DIF", DIF, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
     | expr TOK_MULTIPLY expr {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$3].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("MULT", MULT, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
     | expr TOK_DIVISION expr {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$3].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("DIV", DIV, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
     | expr TOK_REMAINDER expr {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$3].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("REMAINDER", REMAINDER, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
     | TOK_OPEN_BRACKET expr TOK_CLOSE_BRACKET {
       $$ = $2;
}
     | TOK_NAME TOK_OPEN_BRACKET params TOK_CLOSE_BRACKET {
       int id_token = find_last_id_token_with_right_br(&tokens);
       struct SymbolTableItem fun = create_fun(tokens.array[id_token].lexem); 
       int fun_index = addItem(&table, fun);
       struct Command command = createCommand("CALL", CALL, fun_index, 0, 0);
       addCommand(&commands, command);
}
     | TOK_DIFFERENCE expr %prec UMINUS {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(table.array[$1].type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("U_MINUS", U_MINUS, $1, 0, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
     | TOK_REAL_CONST {
	   char *temp_var_name = generateTempVarName();
       double value = atof(tokens.array[tokens.used-1].lexem);
       $$ = addItem(&table, create_item_double(temp_var_name, value, true));
	   free(temp_var_name);
}
     | TOK_INT_CONST {
       int value = atoi(tokens.array[tokens.used-1].lexem);
       $$ = addItem(&table, create_item_int("", value, true));
}
     | TOK_STRING {
	   char *temp_var_name = generateTempVarName();
       char *value = tokens.array[tokens.used-1].lexem;
       $$ = addItem(&table, create_item_string(temp_var_name, value, true));
	   free(temp_var_name);
}
     | TOK_BOOL {
       int value = atoi(tokens.array[tokens.used-1].lexem);
       $$ = addItem(&table, create_item_int("", value, true));
}
     | TOK_NAME {
       struct Token * id_token = find_last_token_id(&tokens);
       $$ = findVarByName(&table, id_token->lexem);
}
     | TOK_ENDL 
     | expr TOK_OUTPUT_STREAM expr 
     | expr TOK_INPUT_STREAM expr 
     | TOK_NAME TOK_INCREMENT {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$1].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("ADD", ADD, $1, $1, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
} 
     | TOK_NAME TOK_DECREMENT {
       char *temp_var_name = generateTempVarName();
       enum Type res_type = estimateType(table.array[$1].type, table.array[$1].type);
       struct SymbolTableItem var = create_var(res_type, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("DIF", DIF, $1, $1, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
     |;

params : expr TOK_SEP_COMMA params {
       struct Command command = createCommand("PARAM", PARAM, $1, 0, 0);
       addCommand(&commands, command);
}
       | expr {
       struct Command command = createCommand("PARAM", PARAM, $1, 0, 0);
       addCommand(&commands, command);
}
       |;

while: TOK_WHILE TOK_OPEN_BRACKET bool_expr TOK_CLOSE_BRACKET TOK_OPEN_FIGURE_BRACKET body TOK_CLOSE_FIGURE_BRACKET {
       struct Command command = createCommand("GOTO", GOTO, pop(&command_before_while), 0, 0);
       addCommand(&commands, command);
       struct Command new_command = createCommand("IFELSE", IFELSE, $3, commands.used, 0);
       insertCommand(&commands, new_command, pop(&last_command_log_expr));
       $$ = commands.used-1;
};

bool_expr : part_expr {
       $$ = $1;
       push(&last_command_log_expr, commands.used);
}
       | part_expr TOK_AND bool_expr {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(1, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("AND", AND, $1, $3, res_index);
       addCommand(&commands, command);
       $$ = res_index;
       free(temp_var_name);
       push(&last_command_log_expr, commands.used);
}
       | part_expr TOK_OR bool_expr {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(BOOL, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("OR", OR, $1, $3, res_index);
       addCommand(&commands, command);
       $$ = res_index;
       free(temp_var_name);
       push(&last_command_log_expr, commands.used);
}
       | TOK_OPEN_BRACKET bool_expr TOK_CLOSE_BRACKET {
       $$ = $2;
} 
       | TOK_NOT bool_expr {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(BOOL, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("NEG", NEG, $1, 0, res_index);
       addCommand(&commands, command);
       $$ = res_index;
       free(temp_var_name);
       push(&last_command_log_expr, commands.used);
};

part_expr : expr {
       $$ = $1;
}
       | expr comp_op expr {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(BOOL, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command;
       if ($2 == TOK_LESS)
              command = createCommand("LT", LT, $1, $3, res_index);
       else if ($2 == TOK_GREATER)
              command = createCommand("GT", GT, $1, $3, res_index);
       else if ($2 == TOK_LESS_OR_EQUAL)
              command = createCommand("LE", LE, $1, $3, res_index);
       else if ($2 == TOK_GREATER_OR_EQUAL)
              command = createCommand("GE", GE, $1, $3, res_index);
       else if ($2 == TOK_EQUAL)
              command = createCommand("EQ", EQ, $1, $3, res_index);
       else if ($2 == TOK_NOT_EQUAL)
              command = createCommand("NEQ", NEQ, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
       | TOK_OPEN_BRACKET expr comp_op expr TOK_CLOSE_BRACKET  {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(BOOL, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command;
       if ($3 == TOK_LESS)
              command = createCommand("LT", LT, $2, $4, res_index);
       else if ($3 == TOK_GREATER)
              command = createCommand("GT", GT, $2, $4, res_index);
       else if ($3 == TOK_LESS_OR_EQUAL)
              command = createCommand("LE", LE, $2, $4, res_index);
       else if ($3 == TOK_GREATER_OR_EQUAL)
              command = createCommand("GE", GE, $2, $4, res_index);
       else if ($3 == TOK_EQUAL)
              command = createCommand("EQ", EQ, $2, $4, res_index);
       else if ($3 == TOK_NOT_EQUAL)
              command = createCommand("NEQ", NEQ, $1, $3, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
}
       | TOK_NOT expr {
       char *temp_var_name = generateTempVarName();
       struct SymbolTableItem var = create_var(BOOL, temp_var_name); 
       int res_index = addItem(&table, var); 
       struct Command command = createCommand("NEG", NEG, $1, 0, res_index);
       addCommand(&commands, command);
       free(temp_var_name);
       $$ = res_index;
};

comp_op : TOK_LESS_OR_EQUAL {$$ = TOK_LESS_OR_EQUAL;}
       | TOK_GREATER_OR_EQUAL {$$ = TOK_GREATER_OR_EQUAL;}
       | TOK_EQUAL {$$ = TOK_EQUAL;}
       | TOK_LESS {$$ = TOK_LESS;}
       | TOK_GREATER {$$ = TOK_GREATER;}
       | TOK_NOT_EQUAL {$$ = TOK_NOT_EQUAL;};

if : TOK_IF TOK_OPEN_BRACKET bool_expr TOK_CLOSE_BRACKET TOK_OPEN_FIGURE_BRACKET body TOK_CLOSE_FIGURE_BRACKET else_if else {
       struct Command new_command = createCommand("IFELSE", IFELSE, $3, $8, 0);
       insertCommand(&commands, new_command, pop(&last_command_log_expr));
};

else_if : TOK_ELSE_IF TOK_OPEN_BRACKET bool_expr TOK_CLOSE_BRACKET TOK_OPEN_FIGURE_BRACKET body TOK_CLOSE_FIGURE_BRACKET else_if {
       struct Command new_command = createCommand("IFELSE", IFELSE, $3, $8, 0);
       insertCommand(&commands, new_command, pop(&last_command_log_expr));
}
       | {$$ = commands.used;};

else : TOK_ELSE TOK_OPEN_FIGURE_BRACKET body TOK_CLOSE_FIGURE_BRACKET {
       $$ = $3;
       struct Command command = createCommand("GOTO", GOTO, commands.used, 0, 0);
       insertCommand(&commands, command, $3-1);
}
       | {$$ = commands.used;};

%%
void yyerror(const char *mes) {printf("OPS: %s\n", mes);}

#include "lists.h"

void generate(struct SymbolTable *table, struct CommandList *commands); 
void generate_header(struct SymbolTable *table);
void generate_bottom();
void fill_stack_labels(struct CommandList *command, struct Stack *stack);

void generate_st_const(struct SymbolTable *table);
void generate_assign_int(struct SymbolTableItem left_op,
                         struct SymbolTableItem right_op);
void generate_assign_float(struct SymbolTableItem left_op,
                           struct SymbolTableItem right_op);
void generate_assign_double(struct SymbolTableItem left_op,
                            struct SymbolTableItem right_op);

void generate_push(struct SymbolTableItem arg);
void generate_call(struct SymbolTableItem arg);

void generate_add(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res);

void generate_sub(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res);

void generate_mult(struct SymbolTableItem arg1,
                   struct SymbolTableItem arg2,
                   struct SymbolTableItem res);

void generate_div(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res);

void generate_neg(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res);

void generate_uminus(struct SymbolTableItem arg1,
                     struct SymbolTableItem arg2,
                     struct SymbolTableItem res);

void generate_remainder(struct SymbolTableItem arg1,
                        struct SymbolTableItem arg2,
                        struct SymbolTableItem res);

void generate_ge(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res);

void generate_gt(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res);

void generate_lt(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res);

void generate_le(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res);

void generate_eq(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res);

void generate_neq(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res);

void generate_ifelse(struct SymbolTableItem arg1, int arg2);
void generate_goto(int arg1);

void generate_and(struct SymbolTableItem arg1, 
		          struct SymbolTableItem arg2,
		          struct SymbolTableItem res);

void generate_or(struct SymbolTableItem arg1, 
		         struct SymbolTableItem arg2,
		         struct SymbolTableItem res);

#include <stdbool.h>

struct Stack{
  int *array;
  int size;
  int used;
};

void initStack(struct Stack *stack, int init_size);
void freeStack(struct Stack *stack);
void push(struct Stack *stack, int item);
int pop(struct Stack *stack);
bool in(struct Stack *stack, int val);

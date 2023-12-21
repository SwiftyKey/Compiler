#include "Stack.h"
#include <stdlib.h>

void initStack(struct Stack *stack, int init_size){
  stack->array = malloc(init_size * sizeof(int));
  stack->used = 0;
  stack->size = init_size;
}

void freeStack(struct Stack *stack){
  free(stack->array);
}

void push(struct Stack *stack, int item){
  if (stack->used == stack->size) {
    stack->size *= 2;
    stack->array = realloc(stack->array, stack->size*sizeof(int));
  }
  stack->array[stack->used++] = item;
}

int pop(struct Stack *stack){
  if (stack->used == 0)
    return 0;

  return stack->array[--stack->used];
}

bool in(struct Stack *stack, int val){
  for(int i=0;i<stack->used; ++i)
    if (stack->array[i] == val)
      return true;
  return false;
}

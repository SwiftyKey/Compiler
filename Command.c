#include "Command.h"
#include <stdio.h> 

struct Command createCommand(const char *name, enum Operation op, int arg1, int arg2, int result){
  struct Command command;

  command.op_name = malloc((strlen(name) + 1)) ;
  strcpy(command.op_name, name);

  command.op = op;
  command.arg1 = arg1;
  command.arg2 = arg2;
  command.result = result;
  return command;
}

void printCommand(const struct Command *command){
  printf("%s, %d, %d, %d\n", command->op_name, command->arg1, command->arg2, command->result);
}

void initCommandList(struct CommandList *list, size_t init_size){
  list->array = malloc(init_size * sizeof(struct Command));
  list->used = 0;
  list->size = init_size;
}

void freeCommandList(struct CommandList *list){
  free(list->array);
  list->array = NULL;
  list->used = list->size = 0;
}

int addCommand(struct CommandList *list, struct Command command){
  if (list->used == list->size) {
    list->size *= 2;
    list->array = realloc(list->array, list->size * sizeof(struct Command));
  }
  list->array[list->used++] = command;
  return list->used - 1;
}

void insertCommand(struct CommandList *list, struct Command command, int index){
  if (list->used +1 == list->size) {
    list->size *= 2;
    list->array = realloc(list->array, list->size * sizeof(struct Command));
  }
  for(int i=list->used; i>=index; i--)
    list->array[i+1] = list->array[i];
  list->array[index] = command;

  for(int i=0;i<list->used;++i)
    if (list->array[i].op == IFELSE && list->array[i].arg2 > index)
      list->array[i].arg2++;
    else if (list->array[i].op == GOTO && list->array[i].arg1 > index)
      list->array[i].arg1++;

  list->used++;
}

void printCommandList(const struct CommandList *list){
  printf("---------------Command List---------------\n");
  for(int i=0;i<list->used;++i){ 
    printf("%d ", i);
    printCommand(&list->array[i]);
  }
}


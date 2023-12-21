#include <string.h>
#include <stdlib.h>

enum Operation {
  ASSIGNMENT,
  LE,
  LT,
  EQ,
  NEQ,
  GT,
  GE,
  IFELSE,
  GOTO,
  PARAM,
  MULT,
  ADD,
  DIF,
  DIV,
  U_MINUS,
  REMAINDER,
  CALL,
  OR,
  AND,
  NEG,
  RESERVED
};

struct Command{
  enum Operation op;
  char *op_name;
  int arg1;
  int arg2;
  int result;
};

struct Command createCommand(const char *name, enum Operation op, int arg1, int arg2, int result);
void printCommand(const struct Command *command);

struct CommandList{
  struct Command *array;
  size_t used;
  size_t size;
};

void initCommandList(struct CommandList *list, size_t init_size);
void freeCommandList(struct CommandList *list);
int addCommand(struct CommandList *list, struct Command command);
void insertCommand(struct CommandList *list, struct Command command, int index);
void printCommandList(const struct CommandList *list);

#include <string.h>
#include <stdlib.h>
#include "SymbolTable.h"
#include <stdio.h>

struct SymbolTableItem create_var(enum Type type, const char *name){
  switch (type){
    case BOOL:
      return create_item_bool(name, 0, false);
    case INT:
      return create_item_int(name, 0, false);
    case FLOAT:
      return create_item_float(name, 0.0, false);
    case DOUBLE:
      return create_item_double(name, 0.0, false);
    case STRING:
      return create_item_string(name, "", false);
  }
}

struct SymbolTableItem create_fun(const char *name){
  struct SymbolTableItem item;
  item.name = malloc((strlen(name) + 1)) ;
  strcpy(item.name, name);
  item.type = FUN;
  item.value = NULL;
  item.is_const = false;
  return item;
}

struct SymbolTableItem create_item_bool(const char*var_name, int value, bool is_const){
  struct SymbolTableItem item;
  item.name = malloc((strlen(var_name) + 1)) ;
  strcpy(item.name, var_name);
  item.type = BOOL;
  item.value = malloc(sizeof(int));
  *((int*)item.value) = value;
  item.is_const = is_const;
  return item;
}

struct SymbolTableItem create_item_int(const char*var_name, int value, bool is_const){
  struct SymbolTableItem item;
  item.name = malloc((strlen(var_name) + 1)) ;
  strcpy(item.name, var_name);
  item.type = INT;
  item.value = malloc(sizeof(int));
  *((int*)item.value) = value;
  item.is_const = is_const;
  return item;
}

struct SymbolTableItem create_item_float(const char*var_name, float value, bool is_const){
  struct SymbolTableItem item;
  item.name = malloc((strlen(var_name) + 1)) ;
  strcpy(item.name, var_name);
  item.type = FLOAT;
  item.value = malloc(sizeof(float));
  *((float*)item.value) = value;
  item.is_const = is_const;
  return item;
}

struct SymbolTableItem create_item_double(const char*var_name, double value, bool is_const){
  struct SymbolTableItem item;
  item.name = malloc((strlen(var_name) + 1)) ;
  strcpy(item.name, var_name);
  item.type = DOUBLE;
  item.value = malloc(sizeof(double));
  *((double*)item.value) = value;
  item.is_const = is_const;
  return item;
}

struct SymbolTableItem create_item_string(const char*var_name, char *value, bool is_const){
  struct SymbolTableItem item;
  item.name = malloc((strlen(var_name) + 1)) ;
  strcpy(item.name, var_name);
  item.type = STRING;
  item.value = malloc((strlen(value) + 1));
  strcpy((char*)item.value, value);
  item.is_const = is_const;
  return item;
}

void freeTableItem(struct SymbolTableItem *item){
  if (item->name != NULL)
    free(item->name);
  if (item->value != NULL)
    free(item->value);
}

int to_int_value(const struct SymbolTableItem *item){
  return *((const int*) item->value);
}

float to_float_value(const struct SymbolTableItem *item){
  return *((const float*) item->value);
}

double to_double_value(const struct SymbolTableItem *item){
  return *((const double*) item->value);
}

const char *to_string_value(const struct SymbolTableItem *item){
  return (const char*) item->value;
}

void printSymbolTableItem(const struct SymbolTableItem *item){
  if (strlen(item->name) > 0)
    printf("%s ", item->name);

  printf("const: %d ", item->is_const);
  
  printf("shift: %d ", item->shift);
  
  switch (item->type){
    case BOOL:
      printf("BOOL %d\n", to_int_value(item));
      break;
    case INT:
      printf("INT %d\n", to_int_value(item));
      break;
    case FLOAT:
      printf("FLOAT %f\n", to_float_value(item));
      break;
    case STRING:
      printf("STRING %s\n", to_string_value(item));
      break;
    case FUN:
      printf("FUN\n");
      break;
  }
}

void initSymbolTable(struct SymbolTable *table, size_t init_size){
  table->array = malloc(init_size * sizeof(struct SymbolTable));
  table->used = 0;
  table->size = init_size;
}

void freeSymbolTable(struct SymbolTable *table){
  for(int i=0;i<table->used; ++i)
    freeTableItem(&table->array[i]);
  free(table->array);
  table->array = NULL;
  table->used = table->size = 0;
}

int addItem(struct SymbolTable *table, struct SymbolTableItem item){
  if (table->used == table->size) {
    table->size *= 2;
    table->array = realloc(table->array, table->size * sizeof(struct SymbolTableItem));
  }
  table->array[table->used++] = item;
  int shift = sizes[(int)table->array[table->used - 1].type];
  if (table->used > 1){
    shift += table->array[table->used - 2].shift;
  }
  table->array[table->used-1].shift = shift;
  return table->used - 1;
}

int findVarByName(struct SymbolTable *table, const char *name){
  for (int i=0;i<table->used; ++i)
    if (table->array[i].name != NULL && strcmp(table->array[i].name, name) == 0){
      return i;
    }
  return -1;
}

int counter = 0;

char * generateTempVarName(){
  char num[128];
  int n = sprintf(num, "%d", counter++);
  char * distanation = (char*) malloc(n + 2); 
  distanation[0] = 't';
  strcpy(distanation + 1, num);
  return distanation;
}

enum Type estimateType(enum Type type1, enum Type type2){
  return type1 < type2 ? type2: type1;
}

void printSymbolTable(struct SymbolTable *table){
  printf("------------SymbolTable-----------------\n");
  for(int i=0;i<table->used;++i){
    printf("%d ", i);
    printSymbolTableItem(&table->array[i]);
  }
}

int memoryByte(struct SymbolTable *table){
  struct SymbolTableItem last_item = table->array[table->used - 1];
  return last_item.shift; 
}
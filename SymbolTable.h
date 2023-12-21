#include <stddef.h>
#include <stdbool.h>

enum Type {
  BOOL,
  INT,
  FLOAT,
  STRING,
  DOUBLE,
  FUN
};
static int sizes[] = {4, 4, 4, 0, 8, 0};

struct SymbolTableItem{
  char *name;
  void *value;
  enum Type type;
  int shift;
  bool is_const;
};

void freeTableItem(struct SymbolTableItem *item);

struct SymbolTableItem create_fun(const char *name);
struct SymbolTableItem create_var(enum Type type, const char *name);
struct SymbolTableItem create_item_bool(const char* var_name, int value, bool is_const);
struct SymbolTableItem create_item_int(const char* var_name, int value, bool is_const);
struct SymbolTableItem create_item_float(const char* var_name, float value, bool is_const);
struct SymbolTableItem create_item_double(const char* var_name, double value, bool is_const);
struct SymbolTableItem create_item_string(const char* var_name, char *value, bool is_const);
void printSymbolTableItem(const struct SymbolTableItem *item);

int to_int_value(const struct SymbolTableItem *item);
float to_float_value(const struct SymbolTableItem *item);
double to_double_value(const struct SymbolTableItem *item);
const char *to_string_value(const struct SymbolTableItem *item);

struct SymbolTable{
  struct SymbolTableItem *array; 
  size_t used;
  size_t size;
};

void initSymbolTable(struct SymbolTable *table, size_t init_size);
void freeSymbolTable(struct SymbolTable *table);
int addItem(struct SymbolTable *table, struct SymbolTableItem item);
int findVarByName(struct SymbolTable *table, const char *name);
char * generateTempVarName();
enum Type estimateType(enum Type type1, enum Type type2);
void printSymbolTable(struct SymbolTable *table);
int memoryByte(struct SymbolTable *table);

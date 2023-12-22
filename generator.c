#include "generator.h" 

void generate(struct SymbolTable *table, struct CommandList *commands){
  struct Stack labels; 
  initStack(&labels, 128);
  fill_stack_labels(commands, &labels);

  generate_header(table);
  generate_st_const(table);

  for(int i=0;i<commands->used;++i){
    if(in(&labels, i))
      printf(".L%d:\n", i);
    int arg1 = commands->array[i].arg1; 
    int arg2 = commands->array[i].arg2; 
    int result = commands->array[i].result; 
    switch(commands->array[i].op){
      case ASSIGNMENT:
        if (table->array[result].type == INT)
          generate_assign_int(table->array[result], table->array[arg1]);
        else if(table->array[result].type == FLOAT)
          generate_assign_float(table->array[result], table->array[arg1]);
        else if(table->array[result].type == DOUBLE)
          generate_assign_double(table->array[result], table->array[arg1]);
        break;
      case PARAM:
        generate_push(table->array[arg1]);
        break;
      case CALL:
        generate_call(table->array[arg1]);
        break;
      case ADD:
        generate_add(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case DIF:
        generate_sub(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case MULT:
        generate_mult(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case GE:
        generate_ge(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case GT:
        generate_gt(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case LE:
        generate_le(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case LT:
        generate_lt(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case EQ:
        generate_eq(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case NEQ:
        generate_neq(table->array[arg1], table->array[arg2], table->array[result]);
        break;
      case IFELSE:
        generate_ifelse(table->array[arg1], arg2);
        break;
      case GOTO:
        generate_goto(arg1);
        break; 
      case OR:
        generate_or(table->array[arg1], table->array[arg2], table->array[result]);
        break; 
      case AND:
        generate_and(table->array[arg1], table->array[arg2], table->array[result]);
        break;
	    case REMAINDER:
        generate_remainder(table->array[arg1], table->array[arg2], table->array[result]);
	      break;
	    case U_MINUS:
        generate_uminus(table->array[arg1], table->array[arg2], table->array[result]);
	      break;
	    case NEG:
        generate_neg(table->array[arg1], table->array[arg2], table->array[result]);
	      break;
	    case DIV:
        generate_div(table->array[arg1], table->array[arg2], table->array[result]);
	      break;
    }
  }
  generate_bottom();
  freeStack(&labels);
}

void fill_stack_labels(struct CommandList *commands, struct Stack *stack){
  for(int i=0;i<commands->used; ++i){
    enum Operation op = commands->array[i].op; 
    if(op == IFELSE)
      push(stack, commands->array[i].arg2);
    else if (op == GOTO)
      push(stack, commands->array[i].arg1);
  }
}

void generate_header(struct SymbolTable *table){
  for(int i=0; i<table->used; ++i) { 
    struct SymbolTableItem item = table->array[i];
    if(item.type == STRING){
      printf("%s:\n\t.string %s\n", item.name, to_string_value(&item));
    }
    else if((item.type == FLOAT || item.type == DOUBLE) && item.is_const == true){
      printf("%s:\n\t.double %f\n", item.name, to_float_value(&item));
    }
  }
  printf("format_int:\n\t.ascii \"%%d\\n\"\n");
  printf("format_float:\n\t.ascii \"%%f\\n\"\n");

  printf(".text\n.globl main\nmain:\n");
  printf("pushl %%ebp\nmovl %%esp, %%ebp\n");

  int mem = memoryByte(table);
  if (mem > 0)
    printf("subl $%d, %%esp\n", mem);
}

void generate_bottom(){
  printf("movl $0, %%eax\nleave\nret\n");
}

void generate_st_const(struct SymbolTable *table){
  for(int i=0; i<table->used; ++i) { 
    struct SymbolTableItem item = table->array[i];
    if(item.is_const){
      if(item.type == INT || item.type == BOOL){
        printf("movl $%d, -%d(%%ebp)\n", to_int_value(&item), item.shift);
      } 
      else if(item.type == FLOAT){
	      printf("flds %s\nfstpl -%d(%%ebp)\n",item.name, item.shift);
      }
      else if(item.type == DOUBLE){
        printf("fldl %s\nfstpl -%d(%%ebp)\n",item.name, item.shift);
      }
    }
  }
}

void generate_assign_int(struct SymbolTableItem left_op, struct SymbolTableItem right_op){
  printf("movl -%d(%%ebp), %%eax\n", right_op.shift);
  printf("movl %%eax, -%d(%%ebp)\n", left_op.shift);
}

void generate_assign_float(struct SymbolTableItem left_op, struct SymbolTableItem right_op){
  printf("flds -%d(%%ebp)\n", right_op.shift);
  printf("fstps -%d(%%ebp)\n", left_op.shift);
}

void generate_assign_double(struct SymbolTableItem left_op, struct SymbolTableItem right_op) {
  printf("fldl -%d(%%ebp)\n", right_op.shift);
  printf("fstpl -%d(%%ebp)\n", left_op.shift);
}

void generate_push(struct SymbolTableItem arg){
  if(arg.type == INT || arg.type == BOOL){
    printf("push -%d(%%ebp)\n", arg.shift);
    printf("push $format_int\n");
  }
  else if(arg.type == FLOAT){
    printf("push -%d(%%ebp)\n", arg.shift-4);
    printf("push -%d(%%ebp)\n", arg.shift);
    printf("push $format_float\n");
  }
  else if(arg.type == DOUBLE){
    printf("pushl -%d(%%ebp)\n", arg.shift-4);
    printf("pushl -%d(%%ebp)\n", arg.shift);
    printf("push $format_float\n");
  }
  else if(arg.type == STRING)
    printf("push $%s\n", arg.name);
}

void generate_call(struct SymbolTableItem arg){
  if(strcmp(arg.name, "print") == 0)
    printf("call printf\n");
}

void generate_add(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res){

  if (res.type == INT){
    printf("movl -%d(%%ebp), %%edx\n", arg1.shift); 
    printf("movl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("addl %%edx, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (res.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fadds -%d(%%ebp)\n", arg2.shift); 
    printf("fstps -%d(%%ebp)\n", res.shift); 
  }
  else if (res.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("faddl -%d(%%ebp)\n", arg2.shift); 
    printf("fstpl -%d(%%ebp)\n", res.shift); 
  }
}

void generate_sub(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res){

  if (res.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("subl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (res.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fsubs -%d(%%ebp)\n", arg2.shift); 
    printf("fstps -%d(%%ebp)\n", res.shift); 
  } 
  else if (res.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fsubl -%d(%%ebp)\n", arg2.shift); 
    printf("fstpl -%d(%%ebp)\n", res.shift); 
  }
}

void generate_mult(struct SymbolTableItem arg1,
                   struct SymbolTableItem arg2,
                   struct SymbolTableItem res){

  if (res.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("imull -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (res.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fmuls -%d(%%ebp)\n", arg2.shift); 
    printf("fstps -%d(%%ebp)\n", res.shift); 
  }
  else if (res.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fmull -%d(%%ebp)\n", arg2.shift); 
    printf("fstpl -%d(%%ebp)\n", res.shift); 
  } 
}

void generate_div(struct SymbolTableItem arg1,
                   struct SymbolTableItem arg2,
                   struct SymbolTableItem res){
  if (res.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift);
    printf("cltd\n");
    printf("idivl -%d(%%ebp)\n", arg2.shift); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift);
  }
  else if(res.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fdivs -%d(%%ebp)\n", arg2.shift); 
    printf("fstps -%d(%%ebp)\n", res.shift);
  }
  else if(res.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fdivl -%d(%%ebp)\n", arg2.shift); 
    printf("fstpl -%d(%%ebp)\n", res.shift);
  }
}

void generate_neg(struct SymbolTableItem arg1,
                   struct SymbolTableItem arg2,
                   struct SymbolTableItem res){
  printf("movzbl -%d(%%ebp), %%eax\n", arg1.shift);
  printf("testl %%eax, %%eax\n");
  printf("setne %%al\n");
  printf("xorl $1, %%eax\n");
  printf("movzbl %%al, -%d(%%eax)\n", res.shift);
}

void generate_uminus(struct SymbolTableItem arg1,
                   struct SymbolTableItem arg2,
                   struct SymbolTableItem res){
  if (res.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift);
    printf("negl %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift);
  }
  else if(res.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fchs\n"); 
    printf("fstps -%d(%%ebp)\n", res.shift);
  }
  else if(res.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fchs\n"); 
    printf("fstpl -%d(%%ebp)\n", res.shift);
  }
}

void generate_remainder(struct SymbolTableItem arg1,
                   struct SymbolTableItem arg2,
                   struct SymbolTableItem res){
  if (res.type == INT && arg1.type == INT && arg2.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift);
    printf("cltd\n");
    printf("idivl -%d(%%ebp)\n", arg2.shift); 
    printf("movl %%edx, -%d(%%ebp)\n", res.shift);
  }
}

void generate_ge(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res){
  if (arg1.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("cmpl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("setge %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (arg1.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("setnb %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
  else if (arg1.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("setnb %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
}

void generate_gt(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res){
  if (arg1.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("cmpl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("setg %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (arg1.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("seta %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (arg1.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("seta %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
}

void generate_le(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res){
  if (arg1.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("cmpl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("setle %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (arg1.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("setnb %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
  else if (arg1.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("setnb %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
}

void generate_lt(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res){
  if (arg1.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("cmpl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("setl %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (arg1.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("seta %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
  else if (arg1.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fcomip  %%st(1), %%st\n");
    printf("fstp    %%st(0)\n");
    printf("seta %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
}

void generate_neq(struct SymbolTableItem arg1,
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res){
  if (arg1.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("cmpl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("setne %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  } 
  else if (arg1.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("setnp %%al\n");
    printf("movl $1, %%edx\n");

    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("cmovne %%edx, %%eax\n");
    printf("movzbl %%al, %%eax\n");

    printf("movl %%eax, -%d(%%ebp)\n", res.shift);
  }
  else if (arg1.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("setnp %%al\n");
    printf("movl $1, %%edx\n");

    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("cmovne %%edx, %%eax\n");
    printf("movzbl %%al, %%eax\n");

    printf("movl %%eax, -%d(%%ebp)\n", res.shift);
  }
}


void generate_eq(struct SymbolTableItem arg1,
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res){
  if (arg1.type == INT){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("cmpl -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("sete %%al\n"); 
    printf("movzbl %%al, %%eax\n"); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
  }
  else if (arg1.type == FLOAT){
    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("setnp %%al\n");
    printf("movl $0, %%edx\n");

    printf("flds -%d(%%ebp)\n", arg1.shift); 
    printf("flds -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("cmovne %%edx, %%eax\n");
    printf("movzbl %%al, %%eax\n");

    printf("movl %%eax, -%d(%%ebp)\n", res.shift);
  }
  else if (arg1.type == DOUBLE){
    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("setnp %%al\n");
    printf("movl $0, %%edx\n");

    printf("fldl -%d(%%ebp)\n", arg1.shift); 
    printf("fldl -%d(%%ebp)\n", arg2.shift); 
    printf("fucomip  %%st(1), %%st\n");
    printf("fstp %%st(0)\n");

    printf("cmovne %%edx, %%eax\n");
    printf("movzbl %%al, %%eax\n");

    printf("movl %%eax, -%d(%%ebp)\n", res.shift);
  }
}

void generate_ifelse(struct SymbolTableItem arg1, int arg2){
  printf("cmpl $0, -%d(%%ebp)\n", arg1.shift); 
  printf("je .L%d\n", arg2);
}

void generate_goto(int arg1){
    printf("jmp .L%d\n", arg1);
}

void generate_and(struct SymbolTableItem arg1, 
                  struct SymbolTableItem arg2,
                  struct SymbolTableItem res){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("imull -%d(%%ebp), %%eax\n", arg2.shift); 
    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
}

void generate_or(struct SymbolTableItem arg1, 
                 struct SymbolTableItem arg2,
                 struct SymbolTableItem res){
    printf("movl -%d(%%ebp), %%eax\n", arg1.shift); 
    printf("addl -%d(%%ebp), %%eax\n", arg2.shift); 
    
    printf("movl -%d(%%ebp), %%edx\n", arg1.shift); 
    printf("imull -%d(%%ebp), %%edx\n", arg2.shift); 

    printf("subl %%edx, %%eax\n", arg2.shift); 

    printf("movl %%eax, -%d(%%ebp)\n", res.shift); 
}

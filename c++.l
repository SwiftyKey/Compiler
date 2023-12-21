%{
#include <stdio.h>
#include "c++.tab.h"
#include "lists.h"
%}


NAME [a-zA-Z\_][a-zA-Z0-9\_]*
DIGITS [0-9]+
REAL {DIGITS}\.{DIGITS}

%%
#include {
           addToken(&tokens, createToken("TOK_INCLUDE", TOK_INCLUDE, NULL)); 
           return TOK_INCLUDE; 
         }
\{ {
           addToken(&tokens, createToken("TOK_OPEN_FIGURE_BRACKET", TOK_OPEN_FIGURE_BRACKET, NULL)); 
           return TOK_OPEN_FIGURE_BRACKET;
   }
\} {
           addToken(&tokens, createToken("TOK_CLOSE_FIGURE_BRACKET", TOK_CLOSE_FIGURE_BRACKET, NULL)); 
           return TOK_CLOSE_FIGURE_BRACKET;
   }
\( {
           addToken(&tokens, createToken("TOK_OPEN_BRACKET", TOK_OPEN_BRACKET, NULL)); 
           return TOK_OPEN_BRACKET;
   }
\) {
           addToken(&tokens, createToken("TOK_CLOSE_BRACKET", TOK_CLOSE_BRACKET, NULL)); 
           return TOK_CLOSE_BRACKET;
   }
while {
           push(&command_before_while, commands.used);
           addToken(&tokens, createToken("TOK_WHILE", TOK_WHILE, NULL)); 
           return TOK_WHILE;
    }
return {
           addToken(&tokens, createToken("TOK_RETURN", TOK_RETURN, NULL)); 
           return TOK_RETURN;
       }
; {
           addToken(&tokens, createToken("TOK_END_COMMAND", TOK_END_COMMAND, NULL)); 
           return TOK_END_COMMAND;
  }
else" "if {
           addToken(&tokens, createToken("TOK_ELSE_IF", TOK_ELSE_IF, NULL)); 
           return TOK_ELSE_IF;
          }
if {
           addToken(&tokens, createToken("TOK_IF", TOK_IF, NULL)); 
           return TOK_IF;
   }
else {
           addToken(&tokens, createToken("TOK_ELSE", TOK_ELSE, NULL)); 
           return TOK_ELSE;
     }
\, {
           addToken(&tokens, createToken("TOK_SEP_COMMA", TOK_SEP_COMMA, NULL)); 
           return TOK_SEP_COMMA;
   }
= {
           addToken(&tokens, createToken("TOK_ASSIGNMENT", TOK_ASSIGNMENT, NULL)); 
           return TOK_ASSIGNMENT;
  }
\<= {
           addToken(&tokens, createToken("TOK_LESS_OR_EQUAL", TOK_LESS_OR_EQUAL, NULL)); 
           return TOK_LESS_OR_EQUAL;
    }
\>= {
           addToken(&tokens, createToken("TOK_GREATER_OR_EQUAL", TOK_GREATER_OR_EQUAL, NULL)); 
           return TOK_GREATER_OR_EQUAL;
    }
\<\< {
           addToken(&tokens, createToken("TOK_OUTPUT_STREAM", TOK_OUTPUT_STREAM, NULL)); 
           return TOK_OUTPUT_STREAM;
     }
\>\> {
           addToken(&tokens, createToken("TOK_INPUT_STREAM", TOK_INPUT_STREAM, NULL)); 
           return TOK_INPUT_STREAM;
     }
== {
           addToken(&tokens, createToken("TOK_EQUAL", TOK_EQUAL, NULL)); 
           return TOK_EQUAL;
   }
\! {
           addToken(&tokens, createToken("TOK_NOT", TOK_NOT, NULL)); 
           return TOK_NOT;
   }
\!= {
           addToken(&tokens, createToken("TOK_NOT_EQUAL", TOK_NOT_EQUAL, NULL)); 
           return TOK_NOT_EQUAL;
   }
\< {
           addToken(&tokens, createToken("TOK_LESS", TOK_LESS, NULL)); 
           return TOK_LESS;
   }
\+\+ {
           addToken(&tokens, createToken("TOK_INCREMENT", TOK_INCREMENT, NULL)); 
           return TOK_INCREMENT;
     }
\-\- {
           addToken(&tokens, createToken("TOK_DECREMENT", TOK_DECREMENT, NULL)); 
           return TOK_DECREMENT;
     }
\> {
           addToken(&tokens, createToken("TOK_GREATER", TOK_GREATER, NULL)); 
           return TOK_GREATER;
   }
\+ {
           addToken(&tokens, createToken("TOK_ADDITION", TOK_ADDITION, NULL)); 
           return TOK_ADDITION;
   }
\* {
           addToken(&tokens, createToken("TOK_MULTIPLY", TOK_MULTIPLY, NULL)); 
           return TOK_MULTIPLY;
   }
\- {
           addToken(&tokens, createToken("TOK_DIFFERENCE", TOK_DIFFERENCE, NULL)); 
           return TOK_DIFFERENCE;
   }
\/ {
           addToken(&tokens, createToken("TOK_DIVISION", TOK_DIVISION, NULL)); 
           return TOK_DIVISION;
   }
% {
           addToken(&tokens, createToken("TOK_REMAINDER", TOK_REMAINDER, NULL)); 
           return TOK_REMAINDER;
  }
\&\& {
           addToken(&tokens, createToken("TOK_AND", TOK_AND, NULL)); 
           return TOK_AND;
    }
\|\| {
           addToken(&tokens, createToken("TOK_OR", TOK_OR, NULL)); 
           return TOK_OR;
   }
\"[^\"]\" {
           addToken(&tokens, createToken("TOK_STRING", TOK_STRING, yytext)); 
           return TOK_STRING;
          }
\" {
           addToken(&tokens, createToken("TOK_DOUBLE_QUOTES", TOK_DOUBLE_QUOTES, NULL)); 
           return TOK_DOUBLE_QUOTES;
   }
\' {
           addToken(&tokens, createToken("TOK_QUOTE", TOK_QUOTE, NULL)); 
           return TOK_QUOTE;
   }
true|false {
           addToken(&tokens, createToken("TOK_BOOL", TOK_BOOL, yytext)); 
           return TOK_BOOL;
           }
int {
           addToken(&tokens, createToken("TOK_INT", TOK_INT, NULL)); 
           return TOK_INT;
    }
double {
           addToken(&tokens, createToken("TOK_DOUBLE", TOK_DOUBLE, NULL)); 
           return TOK_DOUBLE;
       }
float {
           addToken(&tokens, createToken("TOK_FLOAT", TOK_FLOAT, NULL)); 
           return TOK_FLOAT;
      }
using {
           addToken(&tokens, createToken("TOK_USING", TOK_USING, NULL)); 
           return TOK_USING;
      }
namespace {
           addToken(&tokens, createToken("TOK_NAMESPACE", TOK_NAMESPACE, NULL)); 
           return TOK_NAMESPACE;
          }
endl {
           addToken(&tokens, createToken("TOK_ENDL", TOK_ENDL, NULL)); 
           return TOK_ENDL;
     }
cout {
           addToken(&tokens, createToken("TOK_COUT", TOK_COUT, NULL)); 
           return TOK_COUT;
     }
cin {
           addToken(&tokens, createToken("TOK_CIN", TOK_CIN, NULL)); 
           return TOK_CIN;
    }
\<{NAME}\> {
           addToken(&tokens, createToken("TOK_LIBRARY_NAME", TOK_LIBRARY_NAME, yytext)); 
           return TOK_LIBRARY_NAME;
           }
{REAL} {
           addToken(&tokens, createToken("TOK_REAL_CONST", TOK_REAL_CONST, yytext)); 
           return TOK_REAL_CONST;
       }
{DIGITS} {
           addToken(&tokens, createToken("TOK_INT_CONST", TOK_INT_CONST, yytext)); 
           return TOK_INT_CONST;
         }
{NAME} {
           addToken(&tokens, createToken("TOK_NAME", TOK_NAME, yytext)); 
           return TOK_NAME;
       }
[\t \n]

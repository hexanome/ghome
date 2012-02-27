/* ------------------------------
   $Id: config.y 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "internals.h"
int yylex(void);
void yyerror(char *);
extern int yylineno;
%}

%token EQUAL
%token NEWLINE
%token <Number> NUMBER
%token <String> STRING
%token <String> IDENTIFIER

%union { 
    int Number;
    char String[5000];
}

%%

Config: Configline Config { }
    |
    ;

Configline: Numberline NEWLINE | Stringline NEWLINE | NEWLINE;

Numberline: IDENTIFIER EQUAL NUMBER
{ setIValue($1,$3); } ;

Stringline: IDENTIFIER EQUAL STRING
{ setSValue($1,$3); } ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s line %d\n", s, yylineno);
}

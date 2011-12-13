/* ------------------------------
   $Id: config.l 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

*/

%{
    #include "config_yacc.h"
    #include <stdlib.h>
    #include <string.h>
    #include "internals.h"
    void yyerror(char *);
%}

%option yylineno

delim           [ \t]
newline			(\n)|(\r\n)
comment         #([^\n])*
equal           =

string          \"([^\"])*\"
number          [0-9]*
hexanumber		0x[0-9A-Fa-f]*
letter          [a-zA-Z]
identifier      (_|{letter}|{number})+

%%

{delim}           { }
{comment}         { }
{newline}		  { return NEWLINE; }
{equal}           { return EQUAL; }
{string}          { strcpy(yylval.String, yytext+1); yylval.String[strlen(yylval.String) - 1] = 0; return STRING; }
{number}          { yylval.Number = strtol(yytext, NULL, 10); return NUMBER; }
{hexanumber}      { yylval.Number = strtol(yytext+2, NULL, 16); return NUMBER; }
{identifier}      { strcpy(yylval.String, yytext); return IDENTIFIER; }

%%

int yywrap(void) {
    return 1;
}

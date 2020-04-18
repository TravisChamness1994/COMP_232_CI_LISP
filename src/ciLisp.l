%option noyywrap
%option nounput
%option noinput

%{
    #include "ciLisp.h"
    #define FLEX_BISON_LOG_PATH "../logs/flex_bison.log"
    #define llog(token) fprintf(flex_bison_log_file, "LEX: %s \"%s\"\n", #token, yytext);
%}

digit [0-9]
letter [a-zA-Z]
int [+-]?{digit}+
double [+-]?{digit}+\.{digit}*
symbol {letter}+
lparen [(]
rparen [)]


func "neg"|"abs"|"add"|"sub"|"mult"|"div"|"rem"|"exp"|"exp2"|"pow"|"log"|"sqrt"|"cbrt"|"hypot"|"max"|"min"|"print"|"read"|"equal"|"less"|"greater"



%%

"quit" {
    return QUIT;
    }

{int} {
    llog(INT);
    yylval.dval = strtod(yytext, NULL);
    return INT;
    }

{double} {
        llog(DOUBLE);
        yylval.dval = strtod(yytext, NULL);
        return DOUBLE;
        }

{func} {
    llog(FUNC);
    yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char)+1);
    strcpy(yylval.sval, yytext);
    return FUNC;
    }

{symbol} {
    llog(SYMBOL);
    yylval.sval = (char*) malloc(strlen(yytext)*sizeof(char)+1);
    strcpy(yylval.sval, yytext);
    return SYMBOL;
    }

{lparen} {
     llog(LPAREN);
     return LPAREN;
         }
{rparen} {
    llog(RPAREN);
    return RPAREN;
}

[\n] {
    llog(EOL);
    return EOL;
    }

<<EOF>>  {
    llog(EOFT);
    return EOFT;
    }

[ \t] ; /* skip whitespace */


. { // anything else
    llog(INVALID);
    }

%%

/*
 * The code below should NOT be edited except to comment or uncomment
 * the "freopen" call in the main.
 */

#include <stdio.h>

int main(int argc, char **argv)
{
    int input_from_file = 0;
    flex_bison_log_file = fopen(FLEX_BISON_LOG_PATH, "w");

    // Comment the line below to type inputs directly into the console.
    // Otherwise, they will be read from the file specified in the first program argument.
    freopen(argv[1], "r", stdin); input_from_file = 1;

    while(1) {
        if (!input_from_file)
            printf("> ");
        if (yyparse()) {
            YY_FLUSH_BUFFER;
        }
    }
}

%{
    #include "ciLisp.h"
    #define ylog(r, p) fprintf(flex_bison_log_file, "BISON: %s ::= %s \n", #r, #p)
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
}

%token <sval> FUNC
%token <dval> INT DOUBLE
%token LPAREN RPAREN EOL QUIT EOFT

%type <astNode> s_expr f_expr number s_expr_list

%%

program:
    s_expr EOL {
        ylog(program, s_expr EOL);
        if ($1) {
            printRetVal(eval($1));
            YYACCEPT;
        }
    }
    | s_expr EOFT {
        ylog(program, s_expr EOFT);
        if ($1) {
            printRetVal(eval($1));
        }
        exit(EXIT_SUCCESS);
    }
    | EOFT {
        exit(EXIT_SUCCESS);
    };

s_expr:
    number {
        ylog(s_expr, number);
        $$ = $1;
    }
    | f_expr {
	ylog(s_expr, f_expr);
	$$ = $1;
    }
    | QUIT {
        ylog(s_expr, QUIT);
        exit(EXIT_SUCCESS);
    }
    | error {
        ylog(s_expr, error);
        yyerror("unexpected token");
        $$ = NULL;
    };

number:
    INT {
    	ylog(number, INT);
	$$ = createNumberNode($1, INT_TYPE);
    }
    | DOUBLE
    {
    	ylog(number, DOUBLE);
	$$ = createNumberNode($1, DOUBLE_TYPE);
    };

f_expr:
    LPAREN FUNC s_expr_list RPAREN {
        ylog(f_expr, s_expr_list);
        $$ = createFunctionNode($2,$3);
    };

s_expr_list:
    s_expr {
        ylog(s_expr_list, s_expr);
        $$ = $1;
    }
    | s_expr s_expr_list{
    	ylog(s_expr_list, s_expr s_expr_list);
    	$$ = addOperandToList($1,$2);
    };

%%


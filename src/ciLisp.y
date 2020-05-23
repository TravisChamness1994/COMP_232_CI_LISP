%{
    #include "ciLisp.h"
    #define ylog(r, p) fprintf(flex_bison_log_file, "BISON: %s ::= %s \n", #r, #p)
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symNode;
}

%token <sval> FUNC SYMBOL TYPE
%token <dval> INT DOUBLE
%token LPAREN RPAREN EOL QUIT EOFT let cond lambda

%type <astNode> s_expr f_expr number s_expr_list
%type <symNode> let_section let_elem let_list arg_list

%%

program:
    s_expr EOL {
        ylog(program, s_expr EOL);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
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
    | SYMBOL
    {
    	ylog(s_expr,SYMBOL);
    	$$ = createSymbolNode($1);
    }
    | LPAREN let_section s_expr RPAREN
    {
    	ylog(s_expr, LPAREN let_section s_expr RPAREN);
    	$$ = listAstLink($2, $3);
    }
    |
     LPAREN cond s_expr s_expr s_expr RPAREN
    {
     ylog(s_expr, cond s_expr s_expr s_expr);
     $$ = createCondNode($3,$4,$5);
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
    LPAREN FUNC RPAREN {
    	ylog(f_expr, FUCN);
        $$ = createFunctionNode($2, NULL);
    }
    |LPAREN FUNC s_expr_list RPAREN {
        ylog(f_expr, FUNC s_expr_list);
        $$ = createFunctionNode($2,$3);
    }
    |LPAREN SYMBOL s_expr_list RPAREN{
    	ylog(f_expr, SYMBOL s_expr_list);
    	$$ = createFunctionNode($2, $3);
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

arg_list:
    SYMBOL{
        ylog(arg_list, SYMBOL);
        $$ = createSymbolTableNode(NULL, $1, NULL);
    }
    | SYMBOL arg_list{
	ylog(arg_list, SYMBOL);
	$$ = addSymbolToList(createSymbolTableNode(NULL, $1, NULL), $2);
    };

let_section:
    %empty
    |
    LPAREN let let_list RPAREN{
        ylog(let_section, let let_list);
        $$ = $3;
    };

let_list:
    let_elem{
        ylog(let_list, let_elem);
        $$ = $1;
    }
    | let_list let_elem{
        ylog(let_list, let_list let_elem);
        $$ = addSymbolToList($1, $2);
    };

let_elem:
    LPAREN SYMBOL s_expr RPAREN{
        ylog(let_elem, SYMBOL s_expr);
        $$ = createSymbolTableNode(NULL,$2,$3);
    }
    |LPAREN TYPE SYMBOL s_expr RPAREN{
    	ylog(let_elem, TYPE SYMBOL s_expr);
    	$$ = createSymbolTableNode($2,$3,$4);
    }|LPAREN SYMBOL lambda LPAREN arg_list RPAREN s_expr RPAREN{
    	ylog(let_elem, LPAREN SYMBOL lambda LPAREN arg_list RPAREN s_expr RPAREN);
    	$$ = createCustomFunction(NULL, $2, $5, $7);
    }
    |LPAREN TYPE SYMBOL lambda LPAREN arg_list RPAREN s_expr RPAREN{
    	ylog(let_elem, LPAREN TYPE SYMBOL lambda LPAREN arg_list RPAREN s_expr RPAREN);
    	$$ = createCustomFunction($2, $3, $6, $8);
    }
    ;

%%


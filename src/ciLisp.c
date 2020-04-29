#include "ciLisp.h"
//declerations for helper funcs
void multFuncResInitializer(RET_VAL*);
void funcResInitializer(RET_VAL*);
void getFirst(AST_NODE *currNode, RET_VAL *result, bool *error);

void yyerror(char *s) {
    fprintf(stdout, "\nERROR: %s\n", s);
    exit(1);
}

void printWarning(char *s) {
    fprintf(stdout, "\nWARNING: %s\n", s);
}

// Array of string values for operations.
// Must be in sync with funcs in the OPER_TYPE enum in order for resolveFunc to work.
char *funcNames[] = {
        "neg",
        "abs",
        "add",
        "sub",
        "mult",
        "div",
        "rem",
        "exp",
        "exp2",
        "pow",
        "log",
        "sqrt",
        "cbrt",
        "hypot",
        "max",
        "min",
        "print",
        "read",
        "rand",
        "equal",
        "less",
        "greater",
        ""
};
char* numNames[] =
        {
        "int",
        "double",
        };
//Expects evalNumNode() to adjust for integer numbers
//Task1 Evaluations
RET_VAL evalNeg(AST_NODE *node)
{
    RET_VAL result;
    if(node == NULL)
    {
        printf("\nERROR: neg called with no operands.\n");
        result = DEFAULT_RET_VAL;
    }
    else
        {
        if(node->next != NULL)
            printWarning("neg called with extra(ignored) operands.");
        //Check for function
        result = eval(node);
        result.value = -result.value;
        }
    return result;
}

RET_VAL evalAbs(AST_NODE *node)
{
    RET_VAL result;
    if(node == NULL)
    {
        printf("\nERROR: abs called with no operands.\n");
        result = DEFAULT_RET_VAL;
    }
    else
        {
        if(node->next != NULL)
            printWarning("abs called with extra(ignored) operands.");
        result = eval(node);
        //2 functions exist for
        if(result.type == INT_TYPE)
            result.value = abs((int)result.value);
        else
            result.value = fabs(result.value);
        }

    return result;
}

void evalAdd(AST_NODE *currNode, RET_VAL *result)
{
    //Base Cases
    if(currNode == NULL)
    {
        printWarning("add call with no operands, 0 returned");
        return;
    }
    if(currNode->next == NULL)
    {
        currNode->data.number = eval(currNode);
        result->value += currNode->data.number.value;
        if(currNode->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
        return;
    }

    currNode->data.number = eval(currNode);
    result->value += currNode->data.number.value;
    if(currNode->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
        result->type = DOUBLE_TYPE;
    //recursive call
    evalAdd(currNode->next,result);
}

void evalSub(AST_NODE *currNode, RET_VAL *result)
{
    if(currNode == NULL)
    {
        printf("\nERROR: sub called with only one arg!\n");
        *result = DEFAULT_RET_VAL;
    }
    else
        {
        if(currNode->next != NULL)
            printWarning("sub called with extra (ignored) operands");

        currNode->data.number = eval(currNode);
        result->value -= currNode->data.number.value;
        if (currNode->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
        }
}

void evalMult(AST_NODE *currNode, RET_VAL *result)
{
    //Base Cases
    if(currNode == NULL)
    {
        printWarning(" mult call with no operands, 1 returned!.");
        return;
    }
    if(currNode->next == NULL)
    {
        currNode->data.number = eval(currNode);
        result->value *= currNode->data.number.value;
        if(currNode->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
        return;
    }
    //perform Multiplication
    currNode->data.number = eval(currNode);
    result->value *= currNode->data.number.value;
    if(currNode->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
        result->type = DOUBLE_TYPE;
    //recursive call
    evalMult(currNode->next, result);
}

void evalDiv(AST_NODE *node, RET_VAL *result)
{
    if(node == NULL)
    {
        printf("\nERROR: div called with only one arg!\n");
        *result = DEFAULT_RET_VAL;
    }
    else
    {
        if(node->next != NULL)
            printWarning("div called with extra (ignored) operands");

        node->data.number = eval(node);
        result->value /= node->data.number.value;
        if (node->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
    }
}

void evalRem(AST_NODE *node, RET_VAL *result)
{
    if(node == NULL)
    {
        printf("\nERROR: rem called with only one arg!\n");
    }
    else
    {
        if(node->next != NULL)
            printWarning("rem called with extra (ignored) operands.");

        node->data.number = eval(node);
        result->value = remainder(result->value, node->data.number.value);
        if (node->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
    }
}

void evalExp(AST_NODE *node, RET_VAL *result)
{
    if(node != NULL)
        printWarning("exp called with extra(ignored) operands.");
    result->value = exp(result->value);
    if(fmod(result->value, 1))
        result->type = DOUBLE_TYPE;
}
void evalExp2(AST_NODE *node, RET_VAL *result)
{
    if(node != NULL)
        printWarning("exp2 called with extra(ignored) operands.");
    result->value = exp2(result->value);
    if(fmod(result->value, 1))
        result->type = DOUBLE_TYPE;
}
void evalPow(AST_NODE *node, RET_VAL *result)
{
    if(node == NULL)
    {
        printf("\nERROR: pow called with only one arg!\n");
        *result = DEFAULT_RET_VAL;
    }
    else
    {
        if(node->next != NULL)
            printWarning("pow called with extra (ignored) operands");

        node->data.number = eval(node);
        result->value = pow(result->value,node->data.number.value);
        if (node->data.number.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
    }
}
void evalLog(AST_NODE *node, RET_VAL *result)
 {
    if(node != NULL)
         printWarning("exp called with extra(ignored) operands.");
    result->value = log(result->value);
    result->type = DOUBLE_TYPE;
 }
void evalSqrt(AST_NODE *node, RET_VAL *result)
{
    if(node != NULL)
        printWarning("exp called with extra(ignored) operands.");
    result->value = sqrt(result->value);
    if(fmod(result->value, 1))
        result->type = DOUBLE_TYPE;
}

void evalCbrt(AST_NODE *node, RET_VAL *result)
{
    if(node != NULL)
        printWarning("cbrt called with extra(ignored) operands.");
    result->value = cbrt(result->value);
    if(fmod(result->value, 1))
        result->type = DOUBLE_TYPE;
}
void evalHypot(AST_NODE *currNode, RET_VAL *result)
{

    if(currNode == NULL)
    {
        return;
    }

    result->value = hypot(result->value, eval(currNode).value);
    //recursive call
    evalHypot(currNode->next,result);

}
void evalMax(AST_NODE *currNode, RET_VAL *result)
{
    if(currNode == NULL)
    {
        return;
    }

    currNode->data.number = eval(currNode);
    if(currNode->data.number.value > result->value)
        *result = currNode->data.number;
    evalMax(currNode->next, result);
}
void evalMin(AST_NODE *currNode, RET_VAL *result)
{
    if(currNode == NULL)
    {
        return;
    }

    currNode->data.number = eval(currNode);
    if(currNode->data.number.value < result->value)
        *result = currNode->data.number;
    evalMin(currNode->next, result);
}

OPER_TYPE resolveFunc(char *funcName)
{
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
            return i;
        i++;
    }
    return CUSTOM_OPER;
}
//Task 3 for num type resolution
//Called when a symbol node is being created to assign the value of the symbol its type
NUM_TYPE resolveNum(char* numType)
{
    int i = 0;
    while (numNames[i][0] != '\0')
    {
        if(strcmp(numNames[i], numType) == 0)
            return i;
        i++;
    }
    return NO_TYPE;
}
//End

// Called when an INT or DOUBLE token is encountered (see ciLisp.l and ciLisp.y).
// Creates an AST_NODE for the number.
// Sets the AST_NODE's type to number.
// Populates the value of the contained NUMBER_AST_NODE with the argument value.
// SEE: AST_NODE, NUM_AST_NODE, AST_NODE_TYPE.
AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    // TODO set the AST_NODE's type, assign values to contained NUM_AST_NODE
    node->type = NUM_NODE_TYPE;
    node->data.number.type = type;
    node->data.number.value = value;

    return node;
}

//Creates an AST_NODE for the SYMBOL.
//sets the AST_NODE's type to symbol.
//Populates the id of the contained SYMBOL_AST_NODE with the argument value.
SYMBOL_TABLE_NODE *createSymbolTableNode(char* type,char* id, AST_NODE *value)
{
    SYMBOL_TABLE_NODE *symbolNode;
    size_t nodeSize;

    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if((symbolNode = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    symbolNode->id = id;
    symbolNode->value = value;
    //Task 3
    if (type == NULL) {
        symbolNode->type = NO_TYPE;
    }
    else
        symbolNode->type = resolveNum(type);
    //End
//    free(type);
    return symbolNode;
}
// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList) {
    // TODO allocate space for the node being created.
    //Task2 - Added opList->parent = node; Links the oplist to the node parent
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = FUNC_NODE_TYPE;

    //assign function to the result of resolve Func
    if (funcName == NULL)
        node->data.function.oper = (OPER_TYPE) NULL;
    else
        node->data.function.oper = resolveFunc(funcName);
    //If oper is a custom_oper, set the ident to funcName
    if (node->data.function.oper == CUSTOM_OPER)
        node->data.function.ident = funcName;
    //Set the oplist
    if (opList == NULL)
        node->data.function.opList = NULL;
    else {
        node->data.function.opList = opList;
        AST_NODE* currNode = opList;
        //TODO - createFunctionNode Task 2 Parent Association - Done
        while(currNode != NULL)
        {
            currNode->parent = node;
            currNode = currNode->next;
        }
    }

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER. X?
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here. X
    // The funcName will be a string identifier for which space should be allocated in the tokenizer. X
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you've assigned the OPER_TYPE.

    //TODO Task 2 -Freeing custom info: only works when I am not using a custom function, needs to be adjusted
    free(funcName);
    return node;
}
AST_NODE *createSymbolNode(char* id)
{
    AST_NODE *node;
    size_t size;

    size = sizeof(AST_NODE);
    if((node = calloc(size, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = SYM_NODE_TYPE;
    node->data.symbol.id = id;

    return node;
}
// Receives an AST_NODE *list (an s_expr_list) and an
// AST_NODE *newHead (the new element to add to the list as
// its head). Links newHead up to list, with newHead as the head,
// and returns the head. That is, prepends newHead to the list.
AST_NODE *addOperandToList(AST_NODE *newHead, AST_NODE *list)
{
    // TODO - addOperandToList - Done
    //Task 2
    //List parents are associated to the owner of the symbol table
    newHead->next = list;
    return newHead;
}

AST_NODE *symbolTreeAstLink(SYMBOL_TABLE_NODE* symbolTable, AST_NODE* node)
{
    //TODO - SymbolTreeAstLink
    //links symbol table to AST_NODE and associates the symolTable value parents to node
    node->symbolTable = symbolTable;
    SYMBOL_TABLE_NODE* currSymbol = symbolTable;
    while(currSymbol != NULL)
    {
        currSymbol->value->parent = node;
        currSymbol = currSymbol->next;
    }
    return node;
}

SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *newHead)
{
    //TODO - AddSymbolToList
    //creates new head to symbolTree
    //Task 2
    //List parents are associated to the owner of the symbol table
    newHead->next = list;

    return newHead;
}
// Evaluates an AST_NODE whose type is NUM_NODE_TYPE.
// Called by the eval function, which evaluates any AST_NODE.
// Returns a RET_VAL with the data stored in the contained NUMBER_AST_NODE.
// SEE: AST_NODE, NUM_AST_NODE, RET_VAL
RET_VAL evalNumNode(AST_NODE *node)
{
    RET_VAL result;
    if (!node)
        result = DEFAULT_RET_VAL;
    else
        {
            result = node->data.number;
        }
    // TODO populate result with the values stored in the node.
    // SEE: AST_NODE, AST_NODE_TYPE, NUM_AST_NODE

    return result;
}


// Evaluates an AST_NODE whose type is FUNC_NODE_TYPE.
// Called by the eval function, which evaluates any AST_NODE.
// Returns the result of running the referenced function on
// the referenced operands (as a RET_VAL);
// SEE: AST_NODE, FUNC_AST_NODE, OPER_TYPE, RET_VAL
// You should create more functions for each OPER_TYPE member,
// which take as input the opList and which output the result
// of performing the specified operation on that opList.
// You should then call the appropriate function in evalFuncNode
// based on the contents of the argument.

//Helper methods that set result value to the appropriate value
//specifically for multiplication

//specifically for add or cumulative results like add
void funcResInitializer(RET_VAL *result)
{
    result->value = 0;
}
void getFirst(AST_NODE *currNode, RET_VAL *result, bool* error)
{
    if(currNode == NULL)
    {
        *error = true;
    } else
        *result = eval(currNode);
}

RET_VAL evalFuncNode(AST_NODE *node)
{
    if (!node)
        return DEFAULT_RET_VAL;

    bool error = false;

    RET_VAL result = DEFAULT_RET_VAL;

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    switch (node->data.function.oper) {
        //neg
        case NEG_OPER:
            result = evalNeg(node->data.function.opList);
            break;
            //abs
        case ABS_OPER:
            result = evalAbs(node->data.function.opList);
            break;
            //Nary should pass 2 parameters, one being for RET_VAL result and one for AST_NODE *currNode
            // add
        case ADD_OPER:
            funcResInitializer(&result);
            evalAdd(node->data.function.opList, &result);
            break;
            //sub
        case SUB_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: sub called with no operators!\n");
                break;
            }
            else
                evalSub(node->data.function.opList->next, &result);
            break;
            //mult
        case MULT_OPER:
            result.value = 1;
            evalMult(node->data.function.opList, &result);
            break;
            //div
        case DIV_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: div called with no operands!\n");
                break;
             }
            evalDiv(node->data.function.opList->next, &result);
            break;
        //rem
        case REMAINDER_OPER://binary function
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: rem called with no operands!\n");
                break;
            }
            evalRem(node->data.function.opList->next, &result);
            break;
//        //exp
        case EXP_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: exp called with no operands!\n");
                break;
            }
            evalExp(node->data.function.opList->next, &result);
            break;
        //exp2
        case EXP2_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: exp2 called with no operands!\n");
                break;
            }
            evalExp2(node->data.function.opList->next, &result);
            break;
//        //pow
        case POW_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: pow called with no operands!\n");
                break;
            }
            evalPow(node->data.function.opList->next, &result);
            break;
//        //log
        case LOG_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: log called with no operands!\n");
                break;
            }
            evalLog(node->data.function.opList->next, &result);
            break;
//        //sqrt
        case SQRT_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if (error)
            {
                printf("\nERROR: sqrt called with no operands!\n");
                break;
            }
            evalSqrt(node->data.function.opList->next, &result);
            break;
//        //cbrt
        case CBRT_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if(error)
            {
                printf("\nERROR: cbrt called with no operands!\n");
                break;
            }
            evalCbrt(node->data.function.opList->next, &result);
            break;
//        //hypot
        case HYPOT_OPER:
            funcResInitializer(&result);
            getFirst(node->data.function.opList, &result, &error);
            //accomodate type
            result.type = DOUBLE_TYPE;
            if(error)
            {
                printf("\nERROR: hypot called with no operands!\n");
                break;
            }
            evalHypot(node->data.function.opList->next, &result);
            break;
//        //max
        case MAX_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if(error)
            {
                printf("\nERROR: max called with no operands!\n");
                break;
            }
            evalMax(node->data.function.opList->next, &result);
            break;
//        //min
        case MIN_OPER:
            getFirst(node->data.function.opList, &result, &error);
            if(error)
            {
                printf("\nERROR: min called with no operands!\n");
                break;
            }
            evalMin(node->data.function.opList, &result);
            break;

        default:
            printf("Switch entered default.");
            break;
    }

    return result;
}

void evalSymbolNode(AST_NODE* node, char* id, RET_VAL *result)
{
    //TODO - EvalSymbolNode - Task 2
    //Basecases
    if (!node)
        return;
    //search through immediate node for id in symbol table
    if(node->symbolTable != NULL)
    {
        SYMBOL_TABLE_NODE *currNode = node->symbolTable;
        while(currNode != NULL)
        {
            if(strcmp(currNode->id, id) == 0){
                *result = eval(currNode->value);

                if(currNode->type != NUM_NODE_TYPE)
                {
                    freeNode(currNode->value);
                    currNode->value = createNumberNode(result->value,result->type);
                }

                //if the result and the Symbol Node type do not agree, then handle the differences
                //TODO task 3 Change to accomodate ALWAYS. Result if Int and curr if Int should still floor the result value. EX: ((int)3/2) * 4 = 6 when it should equal 4
                if(result->type != currNode->type)
                {
                    //if result is of double and currNode is of int, there will be a precision loss
                    if(result->type == DOUBLE_TYPE){
                        printf("\nWARNING: precision loss on int case from %f to %.0f for variable %s\n",result->value, result->value, currNode->id);
                        result->type = currNode->type;
                        result->value = floor(result->value);
                    }
                    //Otherwise there is no precision loss and no value truncation
                    else
                        {
                        result->type = currNode->type;
                        }
                }
                return;
            }
            currNode = currNode->next;
        }
    }
    //Step up to parent and eval symbol table of parent node if symbol table is null
    if(node->parent != NULL)
        evalSymbolNode(node->parent, id, result);
    else{ //if no parent exists and the symbol table is empty or does not contain it, then the symbol is undefined
        printf("\nERROR: Undefined Symbol!");
        return;
    }

}

// Evaluates an AST_NODE.
// returns a RET_VAL storing the the resulting value and type.
// You'll need to update and expand eval (and the more specific eval functions below)
// as the project develops.
RET_VAL eval(AST_NODE *node)
{
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL; // see NUM_AST_NODE, because RET_VAL is just an alternative name for it.

    // TODO complete the switch.
    // Make calls to other eval functions based on node type.
    // Use the results of those calls to populate result.
    switch (node->type)
    {
        case NUM_NODE_TYPE:
            result = evalNumNode(node);
            break;
        case FUNC_NODE_TYPE:
            result = evalFuncNode(node);
            break;
        case SYM_NODE_TYPE:
            evalSymbolNode(node, node->data.symbol.id, &result);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}


// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
    //Val value may not work if casting causes error
    switch (val.type)
    {
        case 0:
            printf("\nInteger: %.0f\n", val.value);
            break;
        case 1:
            printf("\nDouble: %f\n", val.value);
    }
    // TODO print the type and value of the value passed in.
}

//helper method in freeNode method for freeing any symbol tables
void freeSymbolTable(SYMBOL_TABLE_NODE* node)
{
    if(!node)
        return;

    freeSymbolTable(node->next);
    freeNode(node->value);
    free(node->id);
    free(node);
}

// Called after execution is done on the base of the tree.
// (see the program production in ciLisp.y)
// Recursively frees an abstract syntax tree.
// You'll need to update and expand freeNode as the project develops.
// The TODOs below are for task 1, you will need to expand freeNode with
// most of the subsequent tasks.
void freeNode(AST_NODE *node)
{
    if (!node)
        return;
    // TODO if the node's next pointer isn't NULL,
        // make a recursive call to free it
    freeNode(node->next);
    // TODO:
    // if the node is a function node:
        // make a recursive call to free its opList
    switch(node->type)
    {
        case FUNC_NODE_TYPE:
            freeNode(node->data.function.opList);
            free(node->data.function.ident);
            break;
        case SYM_NODE_TYPE:
            free(node->data.symbol.id);
            break;
        default:
            break;

    }

    freeSymbolTable(node->symbolTable);

    free(node);
}


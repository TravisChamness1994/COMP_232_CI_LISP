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
RET_VAL evalCbrt(AST_NODE *node, RET_VAL *result)
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

    //Using types enumeration values to determine values
    if(type == INT_TYPE)
        node->data.number.value = (int)(value);
    else
        node->data.number.value = value;

    return node;
}

// Called when an f_expr is created (see ciLisp.y).
// Creates an AST_NODE for a function call.
// Sets the created AST_NODE's type to function.
// Populates the contained FUNC_AST_NODE with:
//      - An OPER_TYPE (the enum identifying the specific function being called)
//      - 2 AST_NODEs, the operands
// SEE: AST_NODE, FUNC_AST_NODE, AST_NODE_TYPE.
AST_NODE *createFunctionNode(char *funcName, AST_NODE *opList)
{
    // TODO allocate space for the node being created.

    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if((node = calloc(nodeSize, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = FUNC_NODE_TYPE;

    //assign function to the result of resolve Func
    if(funcName == NULL)
        node->data.function.oper = (OPER_TYPE) NULL;
    else
        node->data.function.oper = resolveFunc(funcName);
    //If oper is a custom_oper, set the ident to funcName
    if(node->data.function.oper == CUSTOM_OPER)
        node->data.function.ident = funcName;
    //Set the oplist
    if(opList == NULL)
        node->data.function.opList = NULL;
    else
        node->data.function.opList = opList;

    // TODO set the AST_NODE's type, populate contained FUNC_AST_NODE
    // NOTE: you do not need to populate the "ident" field unless the function is type CUSTOM_OPER. X?
    // When you do have a CUSTOM_OPER, you do NOT need to allocate and strcpy here. X
    // The funcName will be a string identifier for which space should be allocated in the tokenizer. X
    // For CUSTOM_OPER functions, you should simply assign the "ident" pointer to the passed in funcName.
    // For functions other than CUSTOM_OPER, you should free the funcName after you've assigned the OPER_TYPE.

    return node;
}


// Receives an AST_NODE *list (an s_expr_list) and an
// AST_NODE *newHead (the new element to add to the list as
// its head). Links newHead up to list, with newHead as the head,
// and returns the head. That is, prepends newHead to the list.
AST_NODE *addOperandToList(AST_NODE *newHead, AST_NODE *list)
{
    newHead->next = list;
    // TODO - addOperandToList - Done
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

    // TODO:
    // if the node is a function node:
        // make a recursive call to free its opList

    free(node);
}

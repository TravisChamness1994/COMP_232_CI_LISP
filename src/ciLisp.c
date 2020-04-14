#include "ciLisp.h"

RET_VAL evalFuncNode(FUNC_AST_NODE*);
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
        "remainder",
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
        printf("ERROR: neg called with no operands.");
        result = DEFAULT_RET_VAL;
    }
    else
        {
        if(node->next != NULL)
            printf("WARNING: neg called with extra(ignored) operands.");
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
        printf("ERROR: neg called with no operands.");
        result = DEFAULT_RET_VAL;
    }
    else
        {
        if(node->next != NULL)
            printf("WARNING: abs called with extra(ignored) operands.");
        result = eval(node);
        if(result.type == INT_TYPE)
            result.value = abs((int)result.value);

        }

    return result;
}

RET_VAL evalAdd(AST_NODE *currNode)
{

    //Base Case
    if(currNode->next == NULL)
        return currNode->data.number;
    //Adjust for double
    if(currNode->next->data.number.type == DOUBLE_TYPE)
        currNode->data.number.type = DOUBLE_TYPE;
    //perform additioni
    currNode->next->data.number.value += eval(currNode).value;
    //recursive call
    evalAdd(currNode->next);

}

RET_VAL evalSub(AST_NODE *currNode)
{
    //Base Cases
    if(currNode == NULL)
    {
        printf("ERROR: sub called with no operands.");
        return DEFAULT_RET_VAL;
    }
    if(currNode->next == NULL)
        return currNode->data.number;
    //Check Type
    if(currNode->next->data.number.type == DOUBLE_TYPE)
        currNode->data.number.type = DOUBLE_TYPE;
    //perform subtractioni
    currNode->data.number.value -= currNode->next->data.number.value;
    currNode->next->data.number.value = currNode->data.number.value;
    //recursive call
    evalSub(currNode->next);

}

RET_VAL evalMult(AST_NODE *currNode)
{
    //Base Case
    if(currNode == NULL)
    {
        printf("ERROR: mult called with no operands.");
        return DEFAULT_RET_VAL;
    }
    if(currNode->next == NULL)
        return currNode->data.number;
    //Adjust for double
    if(currNode->next->data.number.type == DOUBLE_TYPE)
        currNode->data.number.type = DOUBLE_TYPE;
    //perform Multiplication
    currNode->data.number.value *= currNode->next->data.number.value;
    currNode->next->data.number.value = currNode->next->data.number.value;
    //recursive call
    evalMult(currNode->next);
}

RET_VAL evalDiv(AST_NODE *currNode)
{
    //Base Case
    if(currNode == NULL)
    {
        printf("ERROR: div called with no operands.");
        return DEFAULT_RET_VAL;
    }
    if(currNode->next == NULL)
        return currNode->data.number;
    //Adjust for double
    if(currNode->next->data.number.type == DOUBLE_TYPE)
        currNode->data.number.type = DOUBLE_TYPE;
    //perform Division
    currNode->data.number.value /= currNode->next->data.number.value;
    currNode->next->data.number.value = currNode->next->data.number.value;
    //recursive call
    evalDiv(currNode->next);
}

RET_VAL evalRem()
{

}
RET_VAL evalExp(AST_NODE *node)
{
    RET_VAL result = DEFAULT_RET_VAL;
    //Base cases
    if(node == NULL)
    {
        printf("ERROR: exp called with no operands");
        return result;
    }
    else
        {
        //type check
        if(node->type == FUNC_NODE_TYPE)
            result = evalFuncNode(node->data.function);
        }
}
RET_VAL evalExp2();
RET_VAL evalPow();
RET_VAL evalLog();
RET_VAL evalSqrt();
RET_VAL evalCbrt();
RET_VAL evalHypot();
RET_VAL evalMax();
RET_VAL evalMin(AST_NODE *currNode);

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
    node->data.function.oper = resolveFunc(funcName);
    //If oper is a custom_oper, set the ident to funcName
    if(node->data.function.oper == CUSTOM_OPER)
        node->data.function.ident = funcName;
    //Set the oplist
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
    return NULL;
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
            if(node->data.number.type == INT_TYPE)
                node->data.number.value = floor(node->data.number.value);
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
RET_VAL evalFuncNode(FUNC_AST_NODE *node)
{
    if (!node)
        return DEFAULT_RET_VAL;

    RET_VAL result = DEFAULT_RET_VAL;

    // TODO populate result with the result of running the function on its operands.
    // SEE: AST_NODE, AST_NODE_TYPE, FUNC_AST_NODE

    switch (node->oper)
    {
        //neg
        case NEG_OPER:
            result  = evalNeg(node->opList);
            break;
        //abs
        case ABS_OPER:
            result = evalAbs(node->opList);
            break;
        //add
        case ADD_OPER:
            result = evalAdd(node->opList);
            break;
        //sub
        case SUB_OPER:
            result = evalSub(node->opList);
            break;
        //mult
        case MULT_OPER:
            result = evalMult(node->opList);
            break;
        //div
        case DIV_OPER:
            result = evalDiv(node->opList);
            break;
        //rem
        case REMAINDER_OPER:
            result = evalRem(node->opList);
            break;
        //exp
        case EXP_OPER:
            result = evalExp(node->opList);
            break;
        //exp2
        case EXP2_OPER:
            result = evalExp2(node->opList);
            break;
        //pow
        case POW_OPER:
            result = evalPow(node->opList);
            break;
        //log
        case LOG_OPER:
            result = evalLog(node->opList);
            break;
        //sqrt
        case SQRT_OPER:
            result = evalSqrt(node->opList);
            break;
        //cbrt
        case CBRT_OPER:
            result = evalCbrt(node->opList);
            break;
        //hypot
        case HYPOT_OPER:
            result = evalHypot(node->opList);
            break;
        //max
        case MAX_OPER:
            result = evalMax(node->opList);
            break;
        //min
        case MIN_OPER:
            result = evalMin(node->opList);
            break;

        default:

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
            evalNumNode(node);
            break;
        case FUNC_NODE_TYPE:
            evalFuncNode(node);
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
            printf("Integer: %lf", val.value);
            break;
        case 1:
            printf("Double: %lf", val.value);
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

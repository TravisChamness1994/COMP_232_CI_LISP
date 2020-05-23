#include "ciLisp.h"
#include <ctype.h>
//declerations for helper funcs
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
    RET_VAL valueHold;
    if(currNode->next == NULL)
    {
        valueHold = eval(currNode);
        result->value += valueHold.value;
        if(valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
        return;
    }

    valueHold = eval(currNode);
    result->value += valueHold.value;
    if(valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
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
        RET_VAL valueHold;
        if(currNode->next != NULL)
            printWarning("sub called with extra (ignored) operands");

        valueHold = eval(currNode);
        result->value -= valueHold.value;
        if (valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
        }
}

void evalMult(AST_NODE *currNode, RET_VAL *result)
{
    //Base Cases
    RET_VAL valueHold;
    if(currNode == NULL)
    {
        printWarning(" mult call with no operands, 1 returned!.");
        return;
    }
    if(currNode->next == NULL)
    {
        valueHold = eval(currNode);
        result->value *= valueHold.value;
        if(valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
        return;
    }
    //perform Multiplication
    valueHold = eval(currNode);
    result->value *= valueHold.value;
    if(valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
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

        RET_VAL valueHold;
        valueHold = eval(node);
        result->value /= valueHold.value;
        if (valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
            result->type = DOUBLE_TYPE;
    }
}

void evalRem(AST_NODE *node, RET_VAL *result)
{
    if(node == NULL)
    {
        printf("\nERROR: rem called with only one arg!\n");
        *result = DEFAULT_RET_VAL;
    }
    else
    {
        if(node->next != NULL)
            printWarning("rem called with extra (ignored) operands.");

        RET_VAL valueHold;
        valueHold = eval(node);
        result->value = remainder(result->value, valueHold.value);
        if(result->value < 0)
            result->value += valueHold.value;
        if (valueHold.type == DOUBLE_TYPE && result->type == INT_TYPE)
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

    RET_VAL valueHold;
    valueHold = eval(currNode);
    if(valueHold.value > result->value)
        *result = valueHold;
    evalMax(currNode->next, result);
}
void evalMin(AST_NODE *currNode, RET_VAL *result)
{
    if(currNode == NULL)
    {
        return;
    }

    RET_VAL valueHold;
    valueHold = eval(currNode);
    if(valueHold.value < result->value)
        *result = valueHold;
    evalMin(currNode->next, result);
}

//Task 4 Print
RET_VAL evalPrint(AST_NODE* node)
{
    RET_VAL result = DEFAULT_RET_VAL;
    if(node == NULL)
    {
        printWarning("print called without operands!");
        return DEFAULT_RET_VAL;
    }
    else {
        AST_NODE *currNode = node;
        while (currNode->next != NULL) {
            printRetVal(eval(currNode));
            currNode = currNode->next;
        }
        result = eval(currNode);
    }
    printRetVal(result);
    return result;
}

//Task 4 - Rand
void evalRand(RET_VAL* result)
{
    result->type = DOUBLE_TYPE;
    result->value = ((double)(rand())/(double)(RAND_MAX));
}

//Task 4 - Equal
void evalEqual(AST_NODE* node,RET_VAL* result)
{
    if(node == NULL)
    {
        printWarning("equal called with no operands!");
        return;
    }
    if(node->next == NULL)
    {
        printWarning("equal called with only one operand!");
        return;
    }
    RET_VAL op1 = eval(node);
    RET_VAL op2 = eval(node->next);

    if(op1.value == op2.value)
        result->value = 1;
    else
        result->value = 0;
}
//Task 4 - less
void evalLess(AST_NODE* node, RET_VAL* result)
{
    if(node == NULL)
    {
        printWarning("less called with no operands!");
        return;
    }
    if(node->next == NULL)
    {
        printWarning("less called with only one operand!");
        return;
    }

    RET_VAL op1 = eval(node);
    RET_VAL op2 = eval(node->next);

    if(op1.value < op2.value)
        result->value = 1;
    else
        result->value = 0;
}

//Task 4 - greater
void evalGreater(AST_NODE* node, RET_VAL* result)
{
    if(node == NULL)
    {
        printWarning("greater called with no operands!");
        return;
    }
    if(node->next == NULL)
    {
        printWarning("greater called with only one operand!");
        return;
    }

    RET_VAL op1 = eval(node);
    RET_VAL op2 = eval(node->next);

    if(op1.value > op2.value)
        result->value = 1;
    else
        result->value = 0;
}

//Task 4 - Read
//TODO - evalRead: evaluate with brute force
void evalRead(RET_VAL* result)
{
    char numberString[BUFF_SIZE] = {0};
    char currentChar;
    int count = 0;
    bool isValid = false;

    //while the input is not valid
    while(!isValid) {

        //print for a read prompt
        count = 0;
        printf("\nread:: ");
        currentChar = (char) getchar();
        //set is valid to true because start, set to false when evaluating characters and isnt correct
        isValid = true;
        //while the current char evaluated isnt new line and the value is valid
        while (currentChar != '\n') { //removed && isValid

            switch (currentChar) {
                case '+':
                case '-':
                    //if char is a sign and not in the first position, the number isnt valid and must be re-prompeted
                    if (count != 0) {
                        isValid = false;
                        currentChar = (char)getchar();
                        break;
                    }
                    //the new value in the number string will be the sign in the zero position
                    numberString[count] = currentChar;
                    //increment the character and count
                    currentChar = (char) getchar();
                    count++;
                    //while the current char incoming is a digit, add to the numberString
                    while (isdigit(currentChar)) {
                        numberString[count] = currentChar;
                        count++;
                        currentChar = (char) getchar();
                    }
                    //when that ends, and if the next char is a dot, the result is a double type
                    if (currentChar == '.') {
                        result->type = DOUBLE_TYPE;
                        //type is now updated to double
                        //increment
                        numberString[count] = currentChar;
                        currentChar = (char) getchar();
                        count++;
                        //while the current value is a digit, add to the numberString
                        while (isdigit(currentChar)) {
                            numberString[count] = currentChar;
                            count++;
                            currentChar = (char) getchar();
                        }
                        //the value is not a digit or a dot and can no longer be a number
                    }
                    else if(currentChar == '\n')
                        break;
                    else
                        {
                        //therefor the string is invalid
                            isValid = false;
                            currentChar = (char)getchar();
                        }
                    break;
                case '0'...'9':
                    //while the character is a digit, append to numberString
                    while (isdigit(currentChar)) {
                        numberString[count] = currentChar;
                        count++;
                        currentChar = (char) getchar();
                    }
                    if (currentChar == '.') {
                        result->type = DOUBLE_TYPE;
                        numberString[count] = currentChar;
                        currentChar = (char) getchar();
                        count++;
                        while (isdigit(currentChar)) {
                            numberString[count] = currentChar;
                            count++;
                            currentChar = (char) getchar();
                        }
                    }
                    else if(currentChar == '\n')
                        break;
                    else
                        {
                            isValid = false;
                            currentChar = (char)getchar();
                        }
                    break;
                default:
                    isValid = false;
                    currentChar = (char)getchar();
                    break;
            }
        }
        if(isValid)
        {
            result->value = strtod(numberString, NULL);
            if(result->type == INT_TYPE)
                result->value = floor(result->value);
            printRetVal(*result);
        }
        else
        {
            printf("Invalid entry. Not a integer or double.\n");
            memset(numberString, 0, sizeof(numberString));
        }
    }
}

int getArgTblSize(AST_NODE* funcNode)
{
    int size = 0;
    SYMBOL_TABLE_NODE* argTblPtr = funcNode->customFuncTable->value->argTable;
    while(argTblPtr != NULL)
    {
        size++;
        argTblPtr = argTblPtr->next;
    }
    return size;
}

int getArgsSize(AST_NODE* node)
{
    int size = 0;
    AST_NODE* argsPtr = node->data.function.opList;
    while(argsPtr != NULL)
    {
        size++;
        argsPtr = argsPtr->next;
    }
    return size;
}
//Task 5 Helper
STACK_NODE* getStack(AST_NODE* node)
{
    STACK_NODE* stack = NULL;
    AST_NODE* argsPtr = node->data.function.opList;

    while(argsPtr != NULL)
    {
        STACK_NODE *newHead = calloc(sizeof(STACK_NODE), 1);
        newHead->stack_node = eval(argsPtr);
        argsPtr = argsPtr->next;

        if(stack == NULL)
        {
            stack = newHead;
        } else
            {
                stack->prev = newHead;
                newHead->next = stack;
                stack = newHead;
            }
    }

    return stack;
}
//adds the vales from stack onto the stacks of the arg table arguments
void addStackToArgs(STACK_NODE* stackPtr, SYMBOL_TABLE_NODE* argPtr)
{
    //Only perform for the correct amount of args passed in the
    while (stackPtr->next != NULL)
        stackPtr = stackPtr->next;
    while (argPtr->next != NULL)
        argPtr = argPtr->next;

    while(argPtr != NULL)
    {
        if(argPtr->stack == NULL)
            argPtr->stack = stackPtr;
        else
            {
            stackPtr->next = argPtr->stack;
            argPtr->stack = stackPtr;
            }
        argPtr = argPtr->prev;
        stackPtr = stackPtr->prev;
    }
}

AST_NODE* getCustFuncNode(AST_NODE* node)
{
    char* id = node->data.function.ident;
    while(node != NULL) {
        if (node->customFuncTable != NULL && strcmp(node->customFuncTable->id, id) == 0) {
            return node;
        }
        node = node->parent;
    }
    yyerror("Custom function not found!");

}
//Task 5 - Eval Custom Func
RET_VAL evalCust(AST_NODE* node)
{
//TODO - Task 5: EvalCust
//Get linked stack of evaluated custom nodes
    RET_VAL result = DEFAULT_RET_VAL;
    AST_NODE* funcNode = getCustFuncNode(node);
    int argTblSize = getArgTblSize(funcNode);
    int argsSize = getArgsSize(node);


    //The program fails if too few arguments are passed
    if(argTblSize > argsSize)
    {
        printWarning("\nCustom function called with too few arguments!");
        return result;
    }
    //if too many arguments are called, recursively dive to the bottom of the
    if(argTblSize < argsSize)
    {
        printWarning("Custom function called with too many arguments!");
    }
    //stack is the pointer to the top of the stack of evaluated arguments
    STACK_NODE* stack = getStack(node);

    //sets stack of args on node's value's argtable's arg's stacks's (whew)
    //Stack behavior, both starting with the last element at the top of the stack.
    //with this stack, if there are too many elements passed by the
    STACK_NODE* stackPtr = stack;
    SYMBOL_TABLE_NODE* argPtr = funcNode->customFuncTable->value->argTable;
    addStackToArgs(stackPtr, argPtr);

    //evaluate the function at node->value
    result = eval(funcNode->customFuncTable->value);

    //pop stack values on argtable
    argPtr = funcNode->customFuncTable->value->argTable;
    while(argPtr != NULL)
    {
        argPtr->stack = argPtr->stack->next;
        argPtr = argPtr->next;
    }

    switch(funcNode->customFuncTable->type)
    {
        case INT_TYPE:
            //if result is of double and currNode is of int, there will be a precision loss
            if(result.type == DOUBLE_TYPE)
                printf("\nWARNING: precision loss on int case from %f to %.0f for function %s\n",result.value, floor(result.value), funcNode->customFuncTable->id);
            result.type = funcNode->customFuncTable->type;
            result.value = floor(result.value);
            break;
            //if CurrNode is a double and the result is of type int, the result type needs to be changed to currNodes type.
        case DOUBLE_TYPE:
            if(result.type == INT_TYPE)
            {
                result.type = funcNode->customFuncTable->type;
            }
        default:
            //no mutations need to happen because NO type allows for whatever result of eval(val) to be the official value representing the id.
            break;
    }

    free(argPtr);
    free(stackPtr);
    return result;
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
    //Task 5 - Default symbol type is var_type and will be
    symbolNode->symbolType = SYM_TYPE;
    //End
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
    if (funcName == NULL) {
        node->data.function.oper = (OPER_TYPE) NULL;
    }
    else {
        node->data.function.oper = resolveFunc(funcName);
        node->data.function.ident = NULL;
    }
        //If oper is a custom_oper, set the ident to funcName
    if (node->data.function.oper == CUSTOM_OPER)
        node->data.function.ident = funcName;
    else
        free(funcName);
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
    return node;
}
SYMBOL_TABLE_NODE* createCustomFunction(char* type, char* id, SYMBOL_TABLE_NODE* argList, AST_NODE* value)
{
    //TODO - Task 5: No Freeing Implemented yet in createCustomFunction
    SYMBOL_TABLE_NODE* node;
    size_t size;

    size = sizeof(SYMBOL_TABLE_NODE);
    if((node = calloc(size, 1)) == NULL)
        yyerror("Memory allocation failed!");

    //Type casting.
    if (type == NULL) {
        node->type = NO_TYPE;
    }
    else
        node->type = resolveNum(type);


    //Assign Symbol type of node
    node->symbolType = LAMBDA_TYPE;
    //Assign the function name to the ID
    node->id = id;
    //Value points to new value AST_NODE
    node->value = value;
    //point value's argtable to the arglist
    node->value->argTable = argList;

    //Set argList nodes type to argType
    SYMBOL_TABLE_NODE* currentNode = node->value->argTable;
    while(currentNode != NULL)
    {
        currentNode->symbolType = ARG_TYPE;
        currentNode = currentNode->next;
    }
    free(type);
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

//Task 4
AST_NODE* createCondNode(AST_NODE* boolean, AST_NODE* tru, AST_NODE* fls)
{
    //allocate space for one AST_NODE of Condition type
    AST_NODE *node;
    size_t size;

    size = sizeof(AST_NODE);
    if((node = calloc(size, 1)) == NULL)
        yyerror("Memory allocation failed!");

    node->type = COND_NODE_TYPE;

    //TODO - TASK 4: Ask Ryan if it would be a preffered to perform eval here. His instructions say to use AST_NODE pointers to bool, tru, and false. Why?
    //if eval is performed - the bool condition and true false paths would be accounted and prepared for action on assessing
    //      would need to change the boolean, true, and false nodes to ret_val typyes.
    //if eval is not performed - the bool would need to be evaluated when called in inside of the evalCond method.
    node->data.cond.booleanNode = boolean;
    node->data.cond.booleanNode->parent = node;

    node->data.cond.truNode = tru;
    node->data.cond.truNode->parent = node;

    node->data.cond.flsNode = fls;
    node->data.cond.flsNode->parent = node;

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

AST_NODE *listAstLink(SYMBOL_TABLE_NODE* table, AST_NODE* node)
{
    //TODO - SymbolTreeAstLink - Updated to listAstLink in Task 5
    //links symbol table to AST_NODE and associates the symolTable value parents to node
    SYMBOL_TABLE_NODE* currNode = table;
    SYMBOL_TABLE_NODE* symTblPtr = NULL;
    SYMBOL_TABLE_NODE* custTblPtr = NULL;
    while(currNode!=NULL) {
        switch (currNode->symbolType) {
            case LAMBDA_TYPE:
                //If labda type(reference createCustomFunction), set type
                if(custTblPtr == NULL)
                    custTblPtr = currNode;
                else
                {
                    custTblPtr->next = currNode;
                    custTblPtr = custTblPtr->next;
                }
                break;
            default:
                //if not lambda, then it is a symbol type node
                if(symTblPtr == NULL)
                    symTblPtr = currNode;
                else
                {
                    symTblPtr->next = currNode;
                    symTblPtr = symTblPtr->next;
                }
        }
        currNode = currNode->next;
    }

    //CurrSymbol starts us at the beginning of the given table
    node->customFuncTable = custTblPtr;
    node->symbolTable = symTblPtr;
    SYMBOL_TABLE_NODE* currSymbol = table;
    //valuePtr will be used to update the value pointer of currSymbol.
    AST_NODE* valuePtr;
    while(currSymbol != NULL)
    {
        //set valuePtr to the value pointer
        valuePtr = currSymbol->value;
        //while there are values, update parent to node
        while(valuePtr != NULL)
        {
            valuePtr->parent = node;
            valuePtr = valuePtr->next;
        }
        //iterate currentSymbol to next while there are more symbols
        currSymbol = currSymbol->next;
    }
    //free the ptr
    free(currSymbol);
    free(currNode);
    return node;
}

SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *list, SYMBOL_TABLE_NODE *newHead)
{
    //TODO - AddSymbolToList
    //creates new head to symbolTree
    //Task 2
    //List parents are associated to the owner of the symbol table
    list->prev = newHead;
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
                printf("\nERROR: hypot called with no operands, 0.0 returned!\n");
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
            //print
        case PRINT_OPER:
            //If print is called without operands, the initial result is set to 0 so it will display Integer : 0
            funcResInitializer(&result);
            result = evalPrint(node->data.function.opList);
            break;
            //rand
        case RAND_OPER:
            evalRand(&result);
            break;
            //equal
        case EQUAL_OPER:
            evalEqual(node->data.function.opList,&result);
            break;
            //less
        case LESS_OPER:
            evalLess(node->data.function.opList, &result);
            break;
            //greater
        case GREATER_OPER:
            evalGreater(node->data.function.opList, &result);
            break;
            //read
        case READ_OPER:
            evalRead(&result);
            break;
        default:
            result = evalCust(node);
            break;
    }

    return result;
}

//Task 2
//Evaluates the symbol node by referencing the parent node's symbol table.
//Task 5
//Eval symbol node now needs to do the same job in the case that the table in an argTable where the elements lie.
void evalSymbolNode(AST_NODE* node, char* id, RET_VAL *result)
{
   //Task 2
    //Basecases
    if (!node)
        return;

    //search through immediate node for id in SYMBOL TABLE
    if(node->symbolTable != NULL)
    {
        SYMBOL_TABLE_NODE *currNode = node->symbolTable;
        while(currNode != NULL)
        {
            if(strcmp(currNode->id, id) == 0){
                *result = eval(currNode->value);
            //Task 2 - Frees the evaluated value node and replaces it with the num_node so there is no longer a need to re-evaluate each visit to the symbol.
                if(currNode->type != NUM_NODE_TYPE)
                {
                    freeNode(currNode->value);
                    currNode->value = createNumberNode(result->value,result->type);
                }

                //if the result and the Symbol Node type do not agree, then handle the differences
                //TODO (DONE) task 3 Change to accomodate ALWAYS. Result if Int and curr if Int should still floor the result value. EX: ((int)3/2) * 4 = 6 when it should equal 4
                switch(currNode->type)
                {
                    case INT_TYPE:
                        //if result is of double and currNode is of int, there will be a precision loss
                         if(result->type == DOUBLE_TYPE)
                            printf("\nWARNING: precision loss on int case from %f to %.0f for variable %s\n",result->value, floor(result->value), currNode->id);
                        result->type = currNode->type;
                        result->value = floor(result->value);
                        break;
                        //if CurrNode is a double and the result is of type int, the result type needs to be changed to currNodes type.
                    case DOUBLE_TYPE:
                        if(result->type == INT_TYPE)
                        {
                            result->type = currNode->type;
                        }
                    default:
                        //no mutations need to happen because NO type allows for whatever result of eval(val) to be the official value representing the id.
                        break;
                }
                return;
            }
            currNode = currNode->next;
        }
    }

    //search through immediate node for id in SYMBOL TABLE
    else if(node->argTable != NULL)
    {
        SYMBOL_TABLE_NODE *currNode = node->argTable;
        while(currNode != NULL)
        {
            if(strcmp(currNode->id, id) == 0){
                *result = currNode->stack->stack_node;
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

//TODO - Task 4: EvalCondNode. Could store in node->data.number, but this would not be explicitly known unless a flag was produced or test for NULL on cond node.
void evalCondNode(AST_NODE *node, RET_VAL *result)
{
    RET_VAL boolVal;

    //uncomment when finalized on todo for this task
    boolVal = eval(node->data.cond.booleanNode);
    //free(node->data.cond.booleanNode);
    //node->data.number = boolVal;

    if(boolVal.value > 0)
    {
       *result = eval(node->data.cond.truNode);
       //freeNode(node->data.cond.truNode);
       //node->data.number = result;

    }else {
       *result = eval(node->data.cond.flsNode);
       //freeNode(node->data.cond.flsNode);
       //node->data.number = result;
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
        case COND_NODE_TYPE:
            evalCondNode(node, &result);
            break;
        default:
            yyerror("Invalid AST_NODE_TYPE, probably invalid writes somewhere!");
    }

    return result;
}


// prints the type and value of a RET_VAL
void printRetVal(RET_VAL val)
{
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

void freeStack(STACK_NODE* stack)
{
    if(!stack)
        return;
    freeStack(stack->next);
    free(stack);
}
//helper method in freeNode method for freeing any symbol tables
void freeTable(SYMBOL_TABLE_NODE* node)
{
    if(!node)
        return;
    freeNode(node->value);
    free(node->id);
    freeStack(node->stack);
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
            if(node->data.function.ident != NULL)
                free(node->data.function.ident);
            break;
        case SYM_NODE_TYPE:
            free(node->data.symbol.id);
            break;
        case COND_NODE_TYPE:
            freeNode(node->data.cond.flsNode);
            freeNode(node->data.cond.truNode);
            freeNode(node->data.cond.booleanNode);
        default:
            break;

    }

    freeTable(node->symbolTable);
    freeTable(node->customFuncTable);
    freeTable(node->argTable);

    free(node);
}


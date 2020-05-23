#CiLisp Documentation
###CiLisp Summary:
CiLisp is an interpreter with limited mathematical functionality that can accomodate for user defined recursive functions, conditional statements, an boolean logic. CiLisp is created in multiple tasks beginning with the first task implementing the skeletal functions such as the basic math and AST. The second task implements variable binding with symbols and variable scope and task three implements type casting for those symbols. Task four is the implementation of special logical functions and special functions such as print and read. The logical functions include greater, less, equal, and a cond type AST node that acts as turnary. The print has an unusual feature that prints all values passed to it and returns the final value in it list. This allows print to act effectively in mathematical expressions as the number that it returns. Read pulls a string from the standard input and only accepts doubles or integers as defined in the language. Task five implements the custom function ability for CiLisp granting users the ability to create custom defined functions with resursive ability and type casting.

##Task 1
- Description: 
Task 1 of CiLisp contains the preliminary sequences in order to create the working parser, which is the backbone of CiLisp. This includes creating a Bison and Flex file to create, pass, and link together the decorated parse tree that represents our given language. Once completed, the focus is turned to the ciLisp.c file where we evaluate the parse tree. Using recursive generic methods, nodes are passed one after the other to the evaluating methods in order to return the number values of what may be complex nested processes. Upon return, these numeric values can then be evaluated with the specific operation necessary. At the final stage of our return, we output the pertanant information of the evaluated numberic data being the data type and its value. 
***Task One will have significant testing documentation because of the weight of necessary use throughout CiLisp.***
Implementation for these functions was very simple and straight forward. Each function itself has a definition implemented in C. The main work here is to accomodate the proper amount of arguments given.
- Known Bugs: A bug exists in the pow function. View the Pow test for further explaination.
###Neg
```
input:

(neg)
(neg -1)
(neg 1)
(neg (abs -5.5))
(neg 1 2)

output:

ERROR: neg called with no operands.

Integer: nan

Integer: 1

Integer: -1

Double: -5.500000

WARNING: neg called with extra(ignored) operands.

Integer: -1

Process finished with exit code 0
```

###Abs
```
input:
(abs)
(abs 1)
(abs -1)
(abs -1.4)
(abs -1 2)

output:

ERROR: abs called with no operands.

Integer: nan

Integer: 1

Integer: 1

Double: 1.400000

WARNING: abs called with extra(ignored) operands.

Integer: 1
```

###Add
```
input:
(add)
(add 1)
(add 1.0)
(add 1 2 3 4 5)
(add 1 -1.0)

output:

WARNING: add call with no operands, 0 returned

Integer: 0

Integer: 1

Double: 1.000000

Integer: 15

Double: 0.000000

```

###Sub
```
input:
(sub)
(sub 1)
(sub 1.0)
(sub 1 2)
(sub 2 1)
(sub 2 -1)
(sub 2.0 1)
(sub 2.0 -1)
(sub 1 2 3)

output:

ERROR: sub called with no operators!

Integer: nan

ERROR: sub called with only one arg!

Integer: nan

ERROR: sub called with only one arg!

Integer: nan

Integer: -1

Integer: 1

Integer: 3

Double: 1.000000

Double: 3.000000

WARNING: sub called with extra (ignored) operands

Integer: -1

```

###Mult
```
input:
(mult)
(mult 1)
(mult 1.0)
(mult -1)
(mult -1 -1.0)
(mult 1 -2 3 -4 5)
(mult -1 2 -3 4 -5)

output:

WARNING:  mult call with no operands, 1 returned!.

Integer: 1

Integer: 1

Double: 1.000000

Integer: -1

Double: 1.000000

Integer: 120

Integer: -120

```

###Div
```
input:
(div)
(div 1)
(div 1.0)
(div 1 2)
(div 1.0 2)
(div 2 1)
(div 1.0 2)
(div 2 1)
(div 2.0 1)
(div 5 2.0)
(div -20.0 4)
(div 1 2 3 4)
(div (mult 1 2) 1)

output:

ERROR: div called with no operands!

Integer: nan

ERROR: div called with only one arg!

Integer: nan

ERROR: div called with only one arg!

Integer: nan

Integer: 0

Double: 0.500000

Integer: 2

Double: 0.500000

Integer: 2

Double: 2.000000

Double: 2.500000

Double: -5.000000

WARNING: div called with extra (ignored) operands

Integer: 0

Integer: 2
```

###Rem
```
input:
(rem)
(rem 1)
(rem 0)
(rem -1.0)
(rem 1 2)
(rem 2 1)
(rem 2.5 1)
(rem 2 3)
(rem -6 10)
(rem -6.0 10.0)
(rem -6.0 -10.0)
(rem 1 2 3)
(rem 23 (div 21 3))

output:

ERROR: rem called with no operands!

Integer: nan

ERROR: rem called with only one arg!

Integer: nan

ERROR: rem called with only one arg!

Integer: nan

ERROR: rem called with only one arg!

Integer: nan

Integer: 1

Integer: 0

Double: 0.500000

Integer: 2

Integer: 4

Double: 4.000000

Double: 4.000000

WARNING: rem called with extra (ignored) operands.

Integer: 1

Integer: 2
```

###Exp
```
inputs:
(exp)
(exp 1)
(exp (log 1))
(exp -1)
(exp 5.0)
(exp -2.0)
(exp 1 2)

output:

ERROR: exp called with no operands!

Integer: nan

Double: 2.718282

Double: 1.000000

Double: 0.367879

Double: 148.413159

Double: 0.135335

WARNING: exp called with extra(ignored) operands.

Double: 2.718282

```

###Exp2
```
input:
(exp2)
(exp2 1)
(exp2 1.0)
(exp2 0)
(exp2 0.0)
(exp2 0.5)
(exp2 (div 1 2.00))
(exp2 -2)
(exp2 -2.0)
(exp2 1 2)

output:

ERROR: exp2 called with no operands!

Integer: nan

Integer: 2

Double: 2.000000

Integer: 1

Double: 1.000000

Double: 1.414214

Double: 1.414214

Double: 0.250000

Double: 0.250000

WARNING: exp2 called with extra(ignored) operands.

Integer: 2
```

###Pow
```
input:
(pow)
(pow 1)
(pow 1.0)
(pow 1 1)
(pow 1 1.0)
(pow 2 1)
(pow 2.1 1)
(pow -2 0.5)
(pow 3 3)
(pow 3.0 3)
(pow 27 (div 1 3.0))
(pow 1 2 3)

output:
ERROR: pow called with no operands!

Integer: nan

ERROR: pow called with only one arg!

Integer: nan

ERROR: pow called with only one arg!

Integer: nan

Integer: 1

Double: 1.000000

Integer: 2

Double: 2.100000

Double: -nan

Integer: 27

Double: 27.000000

Double: 3.000000

Process finished with exit code 139 (interrupted by signal 11: SIGSEGV)
```
***Bug Note***
In pow expression, (pow 27 (div 1 3.0)), when freeing the abs function AST node, the free function portion also attempts to free the function node id value. While this task is accomplished in every single previous AST node, this instance crashes with a segmentation fault. On investigation, I have not been able to find where the id portion gets any information in my AST creation. In tracking down, I implemented a check to ensure that if the case of function type being used where id should be NULL that it is set to NULL, but this did not fix it. 

###Log
```
input:

(log)
(log 1)
(log 0)
(log -1)
(log (exp 1))
(div (log 27) (log 3))
(log 1 2)

output:

ERROR: log called with no operands!

Integer: nan

Double: 0.000000

Double: -inf

Double: nan

Double: 1.000000

Double: 3.000000

WARNING: exp called with extra(ignored) operands.

Double: 0.000000
```

###Sqrt
```
input:
(sqrt)
(sqrt 1)
(sqrt 1.0)
(sqrt 0)
(sqrt 0.0)
(sqrt -1)
(sqrt -1.0)
(sqrt 4)
(sqrt 4.0)
(sqrt 2)
(sqrt 1 2)

output:

ERROR: sqrt called with no operands!

Integer: nan

Integer: 1

Double: 1.000000

Integer: 0

Double: 0.000000

Double: -nan

Double: -nan

Integer: 2

Double: 2.000000

Double: 1.414214

WARNING: exp called with extra(ignored) operands.

Integer: 1

```

###Cbrt
```
input:
(cbrt)
(cbrt 0)
(cbrt 0.0)
(cbrt -1)
(cbrt -1.0)
(cbrt 1)
(cbrt 1.0)
(cbrt 27)
(cbrt 4)
(cbrt (pow 3 3))
(cbrt (pow 3 6))
(cbrt 1 2)

output:

ERROR: cbrt called with no operands!

Integer: nan

Integer: 0

Double: 0.000000

Integer: -1

Double: -1.000000

Integer: 1

Double: 1.000000

Double: 3.000000

Double: 1.587401

Double: 3.000000

Double: 9.000000

WARNING: cbrt called with extra(ignored) operands.

Integer: 1

```

###Hypot
```
input:
(hypot)
(hypot 1)
(hypot 1.0)
(hypot 3 4)
(hypot -3 4)
(hypot -30 -40.0)
(hypot 4 4 7)
(hypot 7.0 4 4.0)
(hypot 12 13 14)
(hypot 5 5 5)
(hypot -5 -5.0 (sqrt 25))
(hypot 0 0 0.0 -3 0 0 0 0 4 0.0 -0.0 12)

output:

ERROR: hypot called with no operands, 0.0 returned!

Double: 0.000000

Double: 1.000000

Double: 1.000000

Double: 5.000000

Double: 5.000000

Double: 50.000000

Double: 9.000000

Double: 9.000000

Double: 22.561028

Double: 8.660254

Double: 8.660254

Double: 13.000000

```

###Max
```
input:
(max)
(max 1)
(max -1)
(max 1.0)
(max 232311.121)
(max 1 2 3 4 5 6 7 8.0 9)
(max 1 2 25.0 -26.0 12)

output:

ERROR: max called with no operands!

Integer: nan

Integer: 1

Integer: -1

Double: 1.000000

Double: 232311.121000

Integer: 9

Double: 25.000000
```

###Min
```
input:
(min)
(min 1)
(min 0.0)
(min 0)
(min -1 2 -3 4 -5 6)
(min -1.0 -12.0 12)

output:

ERROR: min called with no operands!

Integer: nan

Integer: 1

Double: 0.000000

Integer: 0

Integer: -5

Double: -12.000000
```

###Composition Test
```
input:
(log (exp(log(exp 1))))
(sub (mult 1 2 3 4) (add 1 2 3 4))
(sub (mult 1 2 3 -4.0) (add -1 -2 -3 -4))
(hypot (sqrt (div 100 7.0)) (mult 6 (sqrt (div 100.0 7))))
(hypot (sqrt (div 100 7.0)) (sqrt (mult 6 (div 100.0 7))))
(add 1 (add 2 (add 3 (add 4 ( add 5 (add 6 (add 7)))))))
(add 1 (add 2 (add 3 (add 4 ( add 5 (add 6 (sub 0 -7.0)))))))

output:

Double: 1.000000

Integer: 14

Double: -14.000000

Double: 22.990681

Double: 10.000000

Integer: 28

Double: 28.000000
```
##Task 2

- Description:
Task 2 of CiLisp elaborates upon the ability of the parser to recognize more variables to perform arithmetic with. The variables added as part of task 2 are symbol nodes. Symbold nodes contain ID's, values, and a next symbol pointer. By expanding the definition of an AST_NODE, which is the primary inclusive node type, to contain a Symbol Table List. By including this symbol table list and symbol type AST_NODEs, we are able to assign and read data of otherwise non-numeric type nodes. An example of a symbol assignment is as follows: (let (abc 1))This is called the let_list and when paired with an s_expr creates the symbol table list of the AST_NODE. 
- Known Issues: No known bugs

*For an indepth look at the relationship, view graphic at bottom of [pdf.](https://drive.google.com/file/d/1XnRFWccKS8pZmxoPVN2ZnOag8EPhdlAb/view?usp=sharing)*

###Test 1
```
input:
((let(x 1))x)
((let (abc (cbrt 8))) (add (sub 3 abc) 4))
((let (abc 1)) (add 2 abc))

output:

Integer: 1

Integer: 5

Integer: 3

```
Test one is a simple evaluation which tests the basic tree construction for symbols. This is the most basic and uncomprehensive test.

###Test 2
```
input:
(mult ((let (a 1) (b 2)) (add a b)) (sqrt 2))
((let (abc 1) (de 1)) (sub (add abc de) abc))
(add ((let (a 1)) (sub 2 a)) a)

output:

ERROR: Undefined Symbol!
Double: nan

ERROR: Undefined Symbol!
Integer: nan

ERROR: Undefined Symbol!
Integer: nan
```
Test 2 ensures the scope of the language is enforced. These runs show that successfully the ((Let_list) (S_expr)) relationship is intact. It was important to have my tree parent traversal was upwards and not lateral or down.

###Test 3
```
input:
(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))

output:

Double: 5.398346

```
Test 3 further tests the given tree to ensure depth does not break the functionality.

##Task 3
- Description: In task 3 of CiLisp, we implement type casting functionality for symbol table nodes to cast over their value. In doing this, the symbol table node was expanded to accomodate a number type variable. When casting occurs, if there is a discrepancy in precision, we alert the user of the truncation if occuring. Finally, the error for invalid character prompt is added which can be seen in test 1.

###Test
```
input:

((let(int x 1.00))x)
((let (int a 5)) (add a 5))
((let (int a 5)) (add a 5.0))
((let (int a 5.5)) (add a 5))
(mult ((let (int a (div 3 2))) a) 4)
(mult ((let (double a (div 3 2))) a) 4)
((let (int a 3)) (hypot a 4))

output:

WARNING: precision loss on int case from 1.000000 to 1 for variable x

Integer: 1

Integer: 10

Double: 10.000000

WARNING: precision loss on int case from 5.500000 to 5 for variable a

Integer: 10

Integer: 4

Double: 6.000000

Double: 5.000000

```
The test for this is simply checking the ability for the programs ability to recognize the type cast in the symbol table node. Once that is tested, it is important to make certain that precision loss errors are noted and accomodated properly including a notification to the user.

##Task 4
- Description: Task 4 of CiLisp adds to the CiLisp by slightly expanding the language to include conditional statements giving the user turnary expression ability via the keyword "cond". Task 4 also allows the user to print, read, rand, and boolean test via greater, lesser, or equal functions. Print prints a list of evaluated s-expressions and returns the final value in the list. Rand returns a random value from 0 to 1 exclusive. Read reads user input in the form of doubles or integers and loops continuously until a valid input is give. 
- Known Issues:  No known issues in task 4.

###Conditional Test
```
input:
(cond 0 5 6)
(cond 1 5 6)
(cond 0 5 (mult ((let (double a (div 3 2))) a) 4))
((let (int a (rand)))(cond a (div 10 2) (pow 2 3)))

output:

Integer: 6

Integer: 5

Double: 6.000000

WARNING: precision loss on int case from 0.840188 to 0 for variable a

Integer: 8
```
Conditional test checks the ability for the conditional statement to execute. I used separate nodes that were not connected inside of the COND type AST node to describe the Bool True False logic rather than a connected string. My reason being readability on revisiting would be higher as the descriptive ability was greater.


###Boolean Logic Cond
```
input: 
(equal 0 0)
(equal 0 1)
(equal 1 1)
(equal 0.0 0)
(equal 1.0 0.0 )
(equal 1 1.0)
(equal 1 5)
((let (a 1))(equal a 1))
(equal)
(equal 1)
(less 0 0)
(less 0 1)
(less 1 1)
(less 0.0 0)
(less 1.0 0.0 )
(less 1 -1.0)
(less -1 -5)
(less -5 -1)
((let (a 1))(less 0 a))
(less)
(less 1)
(greater 0 0)
(greater 0 1)
(greater 1 1)
(greater 0.0 0)
(greater 1.0 0.0 )
(greater 1 -1.0)
(greater -1 -5)
(greater -5 -1)
((let (a 1))(greater a 0))
(greater)
(greater 1)

output:

Integer: 1

Integer: 0

Integer: 1

Integer: 1

Integer: 0

Integer: 1

Integer: 0

Integer: 1

WARNING: equal called with no operands!

Integer: nan

WARNING: equal called with only one operand!

Integer: nan

Integer: 0

Integer: 1

Integer: 0

Integer: 0

Integer: 0

Integer: 0

Integer: 0

Integer: 1

Integer: 1

WARNING: less called with no operands!

Integer: nan

WARNING: less called with only one operand!

Integer: nan

Integer: 0

Integer: 0

Integer: 0

Integer: 0

Integer: 1

Integer: 1

Integer: 1

Integer: 0

Integer: 1

WARNING: greater called with no operands!

Integer: nan

WARNING: greater called with only one operand!

Integer: nan
```
The boolean logic test checks the ability for the equal, less, and greater to accurately portray these qualities. The qualities here are very simple and again could be created using very basic C operators. 

###Print Test
```
input:
(print 1)
(print 1 2 3)
(add 1 (print 2 3 4))
(print (mult 12 2))


output:

Integer: 1

Integer: 1

Integer: 1

Integer: 2

Integer: 3

Integer: 3

Integer: 2

Integer: 3

Integer: 4

Integer: 5

Integer: 24

Integer: 24
```
Print is functionally a list which is iterated through and returned with the final value in the list. 

###Read Test
```
> (add 4 (read))

read:: 4

Integer: 4

Integer: 8
> (add 4 (read))

read:: 4.0

Double: 4.000000

Double: 8.000000
```
Simple test checking that type casting on a simple read call works and appropriately changes types.
```
((let (int a (read)))(mult a 2))

read:: 3.5

Double: 3.500000

WARNING: precision loss on int case from 3.500000 to 3 for variable a

Integer: 6
> ((let (double a (read)))(mult a 2))

read:: 3.5

Double: 3.500000

Double: 7.000000
```
((let (int a (read)))(mult a 2))
read:: 3.5
Checks to ensure that the type conversion is truly successful and not a cosmetic feature. If 3.5 is read in this math expression, the result should be 7 if the type cast is a double and should be 6 if the type cast is for an integer. If we type cast from a double to an int, it is important to notify the user of the percision loss.
```
>((let (double a (read)))(mult a 2))

read:: -f15.+.5448df
Invalid entry. Not a integer or double.

read:: -0138.86+f.5
Invalid entry. Not a integer or double.

read:: dsf4836.a69jf++.5
Invalid entry. Not a integer or double.

read:: 2.5

Double: 2.500000

Double: 5.000000
```
The test above is trying its very best to break the read function. Read is a set of switch cases based on the FSM describing the regex for both integer and double. With proper checks, the read function acts very well in detecting errors and clearing itself of previous input.
##Task 
- Description: Task 5 implements user created custom recursive capable functions with adapted SYMBOL_TABLE_NODEs, AST_NODEs, and STACK_NODEs which accomodate a tree structure that can communicate and store evaluated arguments for the user defined functions and calls. Task 5 has multiple table types in AST_NODEs which allow the AST_NODE to store multiple table types that distinguish the contents and their uses.
- Known Issues: No known Bugs

###Test 1
```
input: 

((let (int myFunc lambda (x y) (add x y))) (myFunc 1.0 2))

output:

WARNING: precision loss on int case from 3.000000 to 3 for function myFunc

Integer: 3
```
This test was very simple and did not include depth of tree meaning an embedded call to the created function further than the immediate call of the root AST node.

###Test 2
```
input:

((let (double myFunc lambda (x y) (mult (add x 5) (sub y 2)))) (sub (myFunc 3 5) 2))

output:

Double: 22.000000

```
Test 2 provided an adequate look into function calls at depth within the modified task 5 tree with new functions and nodes.

***Test 3***
```
input:

((let (gcd lambda (x y) (cond (greater y x) (gcd y x) (cond (equal y 0) x (gcd y (rem x y)))))) (gcd 95 55))

output:

Integer: 5

```
Test 3 GCD custom function was a significant look at the new tree's ability to handle recurion within the tree. 

###Test 4
```
Input:

((let (int a 1)(f lambda (x y) (add x y)))(f 2 (f a 3)))

output:

Integer: 6

``` 
Test 4 looks at the ability to distinguish between symbols and custom function tables. Initially my tree did not properly handle the distinguished trees. With an edit to the tree construction in listAstLink function, the problem was quickly solved by stepping through the stack placing the appropriate stack node to its table.

###Test 5
```
input :

((let (f lambda (x y) (add x y)))(f (sub 5 2) (mult 2 3)))
((let (f lambda (x y) (add x y)))(f (sub 5 2) (mult 2 3) (div 5 2)))
((let (f lambda (x y) (add x y)))(f (sub 5 2)))

output:

Integer: 9

WARNING: Custom function called with too many arguments!

Integer: 9

WARNING: 
Custom function called with too few arguments!

Integer: nan

```
Test 5 determines whether the function calls can accurately be processed with varying levels of appropriate arguments. The challenge was ensuring that the stack of arguments that are processed in the calling function would be associated correctly with the argument in the argument table of the function value. To do this I updated both **SYMBOL_TABLE_NODE** and **STACK_NODE** to have a previous pointer and dove to the bottom of each stack to assign the stack values to the argument table.
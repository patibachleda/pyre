# Pyre

##### Pyre is a language that is focused on simplifying process flows in a language. The language executes a chain of processes.
##### The language gets it's name from this "chain reaction" just like fire would burn a pyre from the ground up through a series of chain reactions.

#### Installation
1. Set the repo to your path variable so you can use the 'pyre' shortcut
2. Run pyre \<filename.pyre\> 

#### Example programs
There are 4 example programs that I created in the examples folder. Some of the features are shown in each program. 
The program starts execution in the main() function. From there, it follows the sequence of processes that gets executed
in the order you specify through through an arrow notation. 

Each process has a return type and can take in arguments. Each process has it's own "main()" function, named func(). The func()
of each process is where the execution of logic happens. Each process may have 0 or more helper methods that can be used inside the func() function.

The helper functions can "emit" a value. This value 's local_scope will inside the func() function.
The func() function can "emit" a value as well. However, This value's local_scope will inside the main() function.
You can define the name of the emitted value by following the syntax emit \<name\>(\<value\>);

#### Grammar
```text
<Program> ::= <Process>* <Main>

<Process> ::= "process" IDENTIFIER "(" <ArgList> ")" "{" <Function> <Helper>* "}"

<Func> ::=  <TYPE> "func" "(" ")" "{" <Stmt>* "}"

<Helper> ::= "helper" <Type> IDENTIFIER "(" <ArgList> ")" "{" <Stmt>* "}"

<Main> ::= "main" "(" ")" "{" <Stmt>* "}"



<ProcessCall> ::= IDENTIFIER "(" <ArgList>  ")" ("->" IDENTIFIER "(" <ArgList>  ")")*

<HelperCall> ::= IDENTIFIER "(" <ArgList>  ")"


<Stmt> ::= <Emit>
         | <IfElse>
         | <Expr>
         | <Call>
         | <VarDec>
         | <ProcessCall>
         | <HelperCall>

<Emit> ::= "emit" IDENTIFIER "(" <Expr> ")"

<VarDec> ::= <Type> IDENTIFIER "=" IDENTIFER

<Conditional> ::= "is" "(" <BooleanExpr> ")" "{" "yes" "{" <Stmt>* "}" "no" "{" <Stmt>* "}" "}"
               | "is" "(" <BooleanExpr> ")" "{" <Stmt>* "}"


<BooleanExpr> ::= <BooleanTerm> ("or" <BooleanTerm>)*

<BooleanTerm> ::= <BooleanFactor> ("and" <BooleanFactor>)*

<BooleanFactor> ::= "not" <BooleanFactor>
                 | "(" <BooleanExpr> ")"
                 | <Comparison>

<Comparison> ::= IDENTIFIER <ComparisonOp> IDENTIFIER
               | IDENTIFIER <ComparisonOp> NUMBER
               | NUMBER <ComparisonOp> IDENTIFIER
               | NUMBER <ComparisonOp> NUMBER
               | <BooleanLiteral>

<ComparisonOp> ::= "<" | "<=" | ">" | ">=" | "==" | "!="

<BooleanLiteral> ::= "true" | "false"

<Binary> ::= <Expr> ( ("+" | "-" | "*" | "/" ) <Expr> )*


<Expr> ::= IDENTIFIER
           | NUMBER
           | "(" <Expr> ")"
           | <FuncCall>
           | "-" <Factor>  // Unary negation
           | <BooleanExpr> // Boolean expressions as part of an expression

<ArgList> ::= <Arg> ("," <Arg>)* | ε

<NamedArg> ::= IDENTIFIER ":" IDENTIFIER

<Type> ::= "int" | "double" |  "boolean" | "string" |  "char" | IDENTIFIER
```

# pyre

### pyre is a language that is focused on simplifying process flows in a language. 

#### Grammar
```text
<Program> ::= <Process>* <Main>

<Process> ::= "process" IDENTIFIER "(" <ArgList> ")" "{" <Function> <Helper>* "}"

<Main> ::= "main" "(" ")" "{" <Stmt>* "}"

<ProcessCall> ::= IDENTIFIER "(" <ArgList>  ")" ("->" IDENTIFIER "(" <ArgList>  ")")*

<HelperCall> ::= IDENTIFIER "(" <ArgList>  ")"

<Function> ::=  TYPE "func" "(" ")" "{" <Stmt>* "}"

<Helper> ::= "helper" <Type> IDENTIFIER "(" <ArgList> ")" "{" <Stmt>* "}"

<Stmt> ::= <Emit>
         | <IfElse>
         | <Expr>
         | <Call>
         | <VarDec>
         | <ProcessCall>
         | <HelperCall>

<Emit> ::= "emit" IDENTIFIER "(" <Expr> ")"

<VarDec> ::= <Type> IDENTIFIER "=" IDENTIFER

<IfElse> ::= "is" "(" <BooleanExpr> ")" "{" "yes" "{" <Stmt>* "}" "no" "{" <Stmt>* "}" "}"
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

<Expr> ::= <Term> ( ("+" | "-") <Term> )*

<Term> ::= <Factor> ( ("*" | "/") <Factor> )*

<Factor> ::= IDENTIFIER
           | NUMBER
           | "(" <Expr> ")"
           | <FuncCall>
           | "-" <Factor>  // Unary negation
           | <BooleanExpr> // Boolean expressions as part of an expression

<ArgList> ::= <Arg> ("," <Arg>)* | ε

<NamedArg> ::= IDENTIFIER ":" IDENTIFIER

<Type> ::= "int" | "double" |  "boolean" | "string" |  "char" | IDENTIFIER
```

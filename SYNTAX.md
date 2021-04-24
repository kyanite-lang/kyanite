# Kyanite Syntax

## Specification
The root of the program is `program`, from which all syntax branches.
```
program                = [ root statement, { newline, root statement } ] ;

root statement         = function definition | object definition | event definition | statement ;
statement              = flow | ( assignment, [ inline if ] ) | ( function call, [ inline if ] ) | ( event invoke, [ inline if ] ) | event function ;

block                  = assignment | function call | break | return | multi line block ;
multi line block       = newline, statement, { newline, statement } ;

function definition    = "function", function name, parameter list, block, end ;
function name          = identifier | ( identifier, ".", identifier ) | function override ;
function override      = identifier, ":", ( binary operator | identifier ) ;
parameter list         = "(", [ identifier, { ",", identifier } ], ")" ;

object definition      = "object", identifier, variable list, [ "!" ] ;
variable list          = "{", identifier, { ",", identifier }, "}" ;

event definition       = "event", identifier, ".", identifier ;
event invoke           = "invoke", full identifier, ".", identifier, [ value list ] ;
event function         = "on", full identifier, ".", identifier, event block ;
event block            = ( "call", full identifier ) | ( [ "as", identifier ], "do", block, end ) ;

flow                   = if block | for block | switch block | typeswitch block ;

if block               = ( "if" | "unless" ), [ assignment, "," ], condition, then block ;
then block             = "then", block, { elseif block }, [ "else", block ], end ;
elseif block           = "else if", condition, "then", block ;

inline if              = ( "if" | "unless" ), condition ;

for block              = "for", ( foreach expression | foruntil expression | for expression ), "do", block, end ;
foreach expression     = identifier, [ ",", identifier ], "in", iterable ;
iterable               = full identifier | list literal | string literal
foruntil expression    = ( identifier | assignment ), "to", expression, [ "by", expression ] ;
for expression         = [ ( identifier | assignment ), "," ], condition, [ ",", ( assignment | function call ) ] ;

switch block           = "switch", expression, switch case, { switch case }, [ default case ], end ;
switch case            = "case", literal, block ;
default case           = "default", block ;

typeswitch block       = "typeswitch", expression, typeswitch case, { typeswitch case }, [ default case ], end ;
typeswitch case        = "case", ( type literal | identifier ), block ;

assignment             = equal assignment | unary assignment | binary assignment ;
equal assignment       = full identifier, "=", expression ;
unary assignment       = ( full identifier, ( "++" | "--" ) ) | ( ( "++" | "--" ), full identifier ) ;
binary assignment      = full identifier, assignment operator, expression ;

condition              = function call | full identifier | type check | conditional expression | unary condition | ( "(", condition, ")" ) ;
conditional expression = expression, conditional operator, expression ;
unary condition        = unary operator , condition ;

expression             = function call | arithmetic | value | condition | ( unary operator, expression ) | ( "(", expression, ")" ) ;
arithmetic             = expression, binary operator, expression ;
type check             = expression, "is", ( type literal | identifier ) ;

function call          = full identifier, value list ;
value list             = "(", [ expression, { ",", expression } ], ")" ;

value                  = conditional value | literal | list | full identifier ;
conditional value      = "if", condition, "then", expression, "else", expression ;

list                   = literal list | ( "[", [ expression, { ",", expression } ] "]" ) ;
literal list           = "[", [ literal, { "," literal } ] "]" ;

literal                = string literal | integer literal | float literal | boolean literal | "nil" ;
string literal         = '"', { character }, '"' ;
integer literal        = 0 | ( [ "-" ], natural number, { number } ) ;
float literal          = [ "-" ], ( 0 | natural number, { number } ), ".", number, { number } ;
boolean literal        = "true" | "false" ;

type literal           = "__string" | "__int" | "__float" | "__bool" | "__list" | "nil" ;

break                  = "break" ;
return                 = "return", [ expression ] ;

full identifier        = { identifier, "." }, identifier ;
identifier             = alpha, { special alphanumeric }, [ "?" ] ;

binary operator        = "+" | "-" | "/" | "*" | "%" | "<<" | ">>" | "^" ;
unary operator         = "not" | "-" | "+" | "~" ;
conditional operator   = "==" | "<", ">", "<=", ">=", "!=" ;
assignment operator    = "+=" | "-=" | "/=" | "*=" | "%=" | "<<=" | ">>=" | "^=" ;

alpha                  = ? [A-Za-z] ? ;
number                 = ? [0-9] ? ;
natural number         = ? [1-9] ? ;
character              = ? any character (*) ?
alphanumeric           = alpha | number ;
special alphanumeric   = alphanumeric | "_" ;
end                    = "end" ;
newline                = "\n", { "\n" } ;
```

### Useful links
* [Extended Backus-Naur Form](https://en.wikipedia.org/wiki/Extended_Backus-Naur_form)

## Examples
### Basic assignment
```
i = 0
s = "Hello!"
o = new Object()
m = myFunction()
n = i + 48
n++
n += 33
# note! this is a comment
```

### Basic flow control
```
ready? = true
if ready? then
    print("Ready!")
else
    print("Not ready!")
end

print("Ready!") if ready?

unless ready? then print("Not ready!") end

if i = incrementByOne(4), i == 5 then
    print("4 plus one does equal 5!")
end
```

#### Switch
```
fruit = getFavouriteFruit()
switch fruit
case "banana" do
    print("Monkies love bananas!")
case "apple" do
    print("Donkies love apples!")
default do
    print("I've not heard of that...")
end
```

### Type-checking
Kyanite is dynamically typed so any value can be stored in any variable. The value of a variable's value or a literal can be checked with the type-checking operator `is` and one of the possible types: `__list` or `__string` or `__int` or `__float` or `__bool` (or `nil`).
```
function doSomething(var)
    if var is __list then
        print("It's a list! The values are:")
        for v in var do print(v) end
    else
        print("Not a list! The value is", var)
    end
end

doSomething(123)
doSomething([1, 2, 3])
```

#### Type-switch
There's a special `typeswitch` keyword that will act as a switch that checks types.
```
object Person { name }
thing = someFunction()
typeswitch thing
case __string do
    print("The string says", thing)
case Person do
    print("A person! Hello,", thing.name)
default do
    print("It's something else...")
end
```

#### Object type checking
You can also check if something is a specific object in a similar way (see about objects below).
```
object Person { name }!
brenda = new Person("Brenda")

if brenda is Person then
    print("You're a person,", brenda)
else
    print("Beep boop! Are you a robot?")
end
```

### Defining a function
```
function saySomething(from, to)
    print(from, "has said something mysterious to", to)
end

saySomething("Lars", "Steven")
# prints: Lars has said something mysterious to Steven
```

### Defining an object
Objects are defined with a special syntax (including a list of variable names enclosed in `{}`) and instances can be created with the `new` operator.
```
object Person { name, age }
bob = new Person
bob.name = "Bob"
bob.age = 34
```

#### Implicit constructor
You can include a `!` after the object's variable list to imply a constructor with all the possible variables. The constructor then works like a function, with the parameter list coming after the object name within the `new` syntax. You can also manually write the constructor by overriding `object:new` (see the section on operator overriding below).
```
object Person { name, age }!
bob = new Person("Bob", 34)
```

### Object functions
Functions that work on instances of objects are defined by the object name followed by `.` followed by the function name.
```
object Person { name }!

function Person.speak(words)
    print(name, "says", words)
end

denise = new Person("Denise")
denise.speak("hello, stranger")
# Prints: Denise says hello, stranger
```

#### Operator overriding for custom objects
Built-ins can be overriden by using `:` instead of `.` between the object and function names. The specific numbers of parameters etc. will depend on which function is being overridden.
```
object Person { name, height }

function Person:+(person1, person2)
    return new Person("mutant", person1.height + person2.height)
end

sarah = new Person("Sarah", 58.8)
craig = new Person("Craig", 61.4)
mutant = sarah + craig
print("What have I created? It must be a", mutant.height, "meters tall", mutant.name)
# Prints: What have I created? It must be a 120.2 meters tall mutant
```

### Basic looping
```
for true do
    print("Eternal looping!")
end

for i = 0, i < 10, i++ do
    print("This prints 10 times!")
end

t = new Iteration()
for t.hasNext?, t = next() do
    print("This loops through the iteration!")
end

for i = 0 to 10 do
    print("This also prints 10 times!")
end

for i = 0 to 10 by 2 do
    print("This only prints 5 times but", i, "will be even!")
end
```

### Basic iteration
```
names = ["Adora", "Bow", "Glimmer"]

for name in names do
    print("Hello," name)
end

for i, name in names do
    print("Hello,", name, "- you are number", i + 1)
end
```

### Object iteration
```
object MyData { name, width, height, weight }!
data = new MyData("Jenny", 20.5, 58.4, 62.6)

for key, value in data do
    print(key, "is", value)
end
```

### Events
Event management is easily built into the language. Events can be defined like object definitions, with a simple `event` keyword. Events can be subscribed to with anonymous or pre-defined functions which are called when the event is invoked. Invoking an event can take optional parameters which are received by the anonymous or pre-defined function as a single parameter (nil if no parameters given). In the case of anonymous functions, the name of the parameter is given with the `as` keyword. When multiple parameters are given when invoking an event, these are received as a list in the listening functions.
```
object Account { balance }!
event Account.transaction

function Account.withdraw(amount)
    this.balance -= amount
    invoke this.transaction(-amount)
end

account = new Account(100)

# You can write this:
function onTransaction(tx)
    print("Balance has changed by", tx)
end
on account.transaction call onTransaction

# or this:
on account.transaction as tx do
    print("Balance has changed by", tx)
end

account.withdraw(20)
# Prints: Balance has changed by -20
```
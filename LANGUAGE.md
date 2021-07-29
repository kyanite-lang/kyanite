# 💎 Kyanite
Kyanite is a compiled language designed for simplicity and power. It aims to bridge the gap between simple design and more complex concepts, such as events and inversion of control.

The language’s design is inspired by other readable languages such as [Lua](https://www.lua.org/about.html) and [Crystal](https://crystal-lang.org/), whilst retaining elements of more traditional object-oriented languages. It’s strongly typed with powerful compile-time type inference.

# Basic examples

The [most basic](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) Kyanite program can be written in only one line.

```
print "Hello, world!"
```

Let’s see how the classic [fibonacci](https://en.wikipedia.org/wiki/Fibonacci_number) algorithm looks.

```
function fib(n int)
  return n if n < 2 else fib(n-1) + fib(n-2)
end
```

# Defining objects

Kyanite is object-oriented, working with objects known as **types**. These can contain instance variables, prefixed with `@` to differentiate them from local variables which start with a lowercase letter.

Functions can be defined within these objects which then have access to the instance variables.

```
type Person
  @name string
  @age int

  function speak
    print $"Hello! My name is {@name} and I am {@age}."
  end
end
```

These can then be instantiated without needing to define a specific constructor, just by passing in explicit parameters.

```
jenny = Person name: "Jenny", age: 27
jenny.speak # prints: "Hello my name is Jenny and I am 27.
```

## Dependency injection

Kyanite makes it very easy to make an application with dependency injection as this is baked into the language.

Instance variables in a class can be marked as injected and at run-time Kyanite will inject the necessary object into the variable when it is instantiated. There is a special `with`…`do` syntax that can be used to define what types should be injected and how.

```
type Worker
  injected @database Database

  function howOldIsMartina
    @database.findPersonByName("martina")?.age
  end
end

with
  Database
  Worker as worker
do
  print worker.howOldIsMartina or "Couldn't find Martina!"
end
```

This snippet instructs Kyanite that Database and Worker are the two types that it should be able to make. The Worker class is given a local variable name, so one  is instantiated immediately to be used within the `do` block (usually used to define the entry point into the application). Because Worker relies on Database being injected, a Database instance is also instantiated.

## Events

Events are a special kind of instance variable which resemble a function signature. These can be invoked, and listeners can be registered such that other parts of the code can react to an invoked event.

```
type Person
  @age int
  @birthday event(int)

  function haveABirthday
    @age = @age + 1
    invoke @birthday @age
  end
end

julia = Person age: 26
on julia.haveABirthday as age do
  print $"Happy birthday, Julia! You are {age} now!"
end

julia.haveABirthday # prints: "Happy birthday, Julia! You are 27 now!"
```

# Type system

Kyanite is strongly-typed with powerful type inference, meaning you often don’t have to declare the type of local variables or functions.

```
a = "Hello!" # inferred to be a string
a = 123 # compile-time error, cannot assign int to string


function howManyEggsInTheBasket # inferred to return an int
  # do some calculations
  return 12
end

foo int = howManyEggsInTheBasket # ok!
bar string = howManyEggsInTheBasket # compile-time error, cannot assign int to string
```

## Nullable types

By default, a variable cannot be null unless stated explicitly (or inferred) with the `?` operator.

```
foo string? = "Hello"
foo = null # ok!

bar string = "Hello"
bar = null # compile time error
```

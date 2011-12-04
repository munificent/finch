^title Expressions

<span class="nav">[Welcome](index.html) | Expressions</span>

Expressions are the heart of any programming language&mdash; they're the building blocks for programs. Finch, like many functional languages but unlike most C-derived languages, does not have statements, only expressions. We'll go over the different kinds of expressions Finch supports, starting from the bottom up.

## Comments

OK, technically comments aren't expressions, but here is as good a place as any to describe them. Comments start with a single quote and end at the end of the line:

    :::finch
    ' this is a comment

## Literals

Finch currently supports two atomic types: numbers and strings. Numbers are double-precision floating point values, and strings are text. Finch doesn't support a lot of fancy formats for them yet, just the basic:

    :::finch
    0
    1234
    -432.1
    "a string"
    "another string"
    "supported escapes: \" \n \\"

## Variables

Variable names in Finch are more flexible than in most other languages. They must contain a letter or an underscore somewhere in the name, but pretty much everything else is fair game. All of these are valid variable names:

    :::finch
    foo
    Bar
    best-friends4eva
    _under_score_
    I<3punctuation!!!!!1

Finch has two kinds of variables. Normal variables are lexically scoped like
other languages. Variables that start with an underscore (`_`) are *fields*.
These are looked up on the object that the current method is being called on.
In other words, when you see `_name`, it's a bit like `this.name` in other
languages.

## Messages

What other languages refer to as "calling a function" or "invoking a method", Finch calls "sending a message." Sending a message to an object (and remember, everything is an object) invokes a *method* on that object. There are three flavors of message sends in Finch: unary, operator, and keyword.

### Unary Messages

An *unary* message has a name but no arguments. You can send an unary message to an object by following the object with the name of the message:

    :::finch
    ' send the 'length' message to the string "hi there"
    "hi there" length

### Operators

One or more punctuation characters defines an *operator*. You can define whatever operators you like, but don't go too crazy. The goal here is not to make your code look like comic strip profanity:

    ' valid punctuation characters
    + - ! @ # $ % ^ & * = < > / ? ~

    ' can also be combined
    -- ?! ---@ <=/=> @*#%&#$@&!

All operators are *infix*&mdash; they have operands on both sides. Using an operator sends a message to the left-hand operand with the right-hand one as an argument.

    :::finch
    a + b

The above expression means "send a `+` message to `a`, passing in `b` as an argument." Finch doesn't have any built-in operators: they're all just message sends.

Because of this, all operators have the same precedence and associativity (left to right). This is unlike most other languages with hard-coded precedence levels. Parentheses are your friends here.

    1 + 2 * 3   ' evaluates to 9 in Finch
    1 + (2 * 3) ' evaluates to 7

Because there are no built-in operators, there are no unary operators in Finch. Instead, it uses unary message for what would be an unary operator in another language:

    ' C/JS/etc.  Finch
    -value       value neg
    !condition   condition not

### Keyword Messages

We've covered messages that take zero arguments (unary) and one (operators). To pass more than one argument, you use *keyword messages*. A keyword is a name followed by a colon (`:`), or just a colon by itself. A keyword message is formed by alternating keywords and arguments. An example will help here:

    :::finch
    dictionary add-key: "some key" value: "the value"

This sends the `add-key:value:` message to the `dictionary` object, passing in "some key" and "the value" as arguments. You can chain as many keywords as you want in a single message (within reason):

    chef cook-soup: tomato appetizer: calimari entree: veal dessert: cake

That sends a *single* `cook-soup:appetizer:entree:dessert:` message to `chef` with four arguments.

Like other messages, keyword messages usually follow a receiver (`dictionary` and `chef` in the above examples). However, you can also omit the receiver. In that case, it will implicitly be sent to a special `Ether` object:

    :::finch
    ' this:
    write: "hi"
    ' is equivalent to:
    Ether write: "hi"

Most of Finch's control flow operations like `if:then:` and `while:do` are defined as methods on Ether.

## Sequences

Multiple expressions can be sequenced together into a single expression by separating them with commas.

    :::finch
    write: "hi", write: "bye"

This code forms a single expression that writes "hi" and then "bye". A sequence just evaluates each of its expressions in order, and returns the result of the last one.

To make things a little cleaner, Finch also treats newlines as commas in places where that makes sense. In other words, we could write the above just as:

    :::finch
    write: "hi"
    write: "bye"

This doesn't mean *all* newlines will be treated as commas. If the end of a line is obviously not the end of an expression, a newline will be ignored. For example:

    :::finch
    write: 1 +
      2

Since a `+` can't end an expression, the newline after is ignored and it continues onto the next line.

## Blocks

Finch looks like a lot of other languages in that curly braces define local blocks:

    :::finch
    {
      write: "inside a block"
    }

However, these blocks aren't what you think they are. When you enclose an expression in curly braces, you're actually creating a block *object*. A block object is essentially a closure or a local function. It's an object that contains a chunk of code and a lexical scope (i.e. its own set of local variables).

When you create a block, the code inside it isn't immediately executed. It's just packaged up into the block object. The above example won't actually print anything. To invoke the code inside a block, you send the block object a `call` message:

    :::finch
    { write: "inside a block" } call

Now "inside a block" will be printed.

### Blocks are Objects

It's important to realize that blocks really are just objects. They can be stored in variables, passed to methods, etc. In fact, Finch doesn't have any built-in flow control structures. Consider:

    :::finch
    if: a < b then: {
      write: "less"
    } else: {
      write: "greater"
    }

That looks like some built-in `if/then` construct. It isn't. What you're looking at is an `if:then:else:` keyword message being sent to `Ether`. The two blocks are arguments that are sent with the message. The implementation of that method looks at the first condition argument, and decides which of the two blocks to `call` based on that.

### Block Arguments

Because blocks are basically functions, you can also pass arguments to them. If a block has arguments, they appear after the opening curly brace, surrounded by pipes (`|`):

    :::finch
    {|a b| a + b }

The above code creates a block that takes two arguments. When called, it returns the sum of the arguments. You pass arguments to a block by using one of the keyword versions of `call`. For example:

    :::finch
    {|a| write: a } call: "arg" ' one argument
    {|a b| write: a + b } call: "one" : "two" ' two args
    {|a b c| write: a + b + c } call: "one" : "two" : "three" ' you get the idea

If you pass too many arguments to a block, the extra ones will be ignored. If you don't pass enough, it will assign the special value `Nil` to the missing ones.

## Assignment

Variables are declared and given values using assignment expressions. An assignment expression is simply a name followed by an assignment arrow, followed by some expression for the value to assign to the variable. For example:

    :::finch
    a <- "some value"

Variables do not have to be explicitly declared&mdash; assigning it a value will create it if it doesn't already exist. The value returned by an assignment expression is the assigned value. For example:

    :::finch
    write: (a <- "hi")

This creates a variable `a`, assigns "hi" to it, then prints "hi".

### Short and Long Assignment

There are actually *two* kinds of assignment in Finch: short and long. The `<-` operator is short assignment and is what you'll use most of the time. It assigns to a variable in the current scope. If you want to avoid creating a *new* variable in the current scope and instead assign to an existing variable defined in an outer scope, you use the long assignment arrow: `<--`.

An example will clarify:

    :::finch
    a <- "outside"
    do: {
      a <- "inside"
    }
    write: a

This chunk of code will display "outside". The assignment inside the `do:` block creates a new `a` variable local to that block. When the block ends, that `a` is discarded and the outer one remains. Let's change it to use long assignment:

    :::finch
    a <- "outside"
    do: {
      a <-- "inside"
    }
    write: a

Now this code will print "inside". The `a <-- "inside"` line now means "walk up the scope chain looking for an *existing* variable named `a` and assign to it." (If it fails to find a previously declared variable to assign to, it just does nothing.)

One way you can think of this is that short assignment always means "declare a new variable in the current scope" and long assignment means "assign to an existing one."

## Arrays

Finch has built-in support for resizable arrays. Most of the things you can do with arrays use normal message syntax, but there's also a little special sauce for creating arrays. If you surround a series of expressions with square brackets, it creates an array with an element for the value of each expression. Elements are separated with newlines or semi-colons like in a regular sequence. Enough talk:

    :::finch
    []             ' creates an empty array
    [1, 2, 3]      ' a three-element array
    [123, "text"]  ' arrays can have elements of different types
    [1 + 2, 3 neg] ' expressions are fine too

    ' newlines can separate elements too
    ["first"
     "second"
     "third"]

Arrays are objects like everything else, so they can be stored in variables, passed to methods, etc.

## Method Binding

One thing you commonly do in Finch is bind methods to objects. Finch doesn't have "class declarations" or anything formal like that. To make an object that does stuff, you basically make an object and then slap a bunch of methods on it.

You do that using the bind operator: `::`. If we have some object `fred`, we can add a method to it like this:

    :::finch
    fred :: dance { writeLine: "Sorry, I'm too sexy." }
    fred dance ' Sorry, I'm too sexy.

This also works for operators and keyword messages:

    :::finch
    fred :: ? right {
        writeLine: "What do I do with a " + right + "?"
    }

    fred :: give: gift to: who {
        writeLine: "Here, " + who + ", have a " + gift + "."
    }

    fred ? "plunger"
    fred give: "plunger" to: "Bill"

### Multibinds

It's common to want to define a number of methods on an object all at once. To make that easier, you can also use parentheses after `::` and define a group of methods, like so:

    :::finch
    fred :: (
        dance { writeLine: "Sorry, I'm too sexy." }

        ? right { writeLine: "What do I do with a " + right + "?" }

        give: gift to: who {
            writeLine: "Here, " + who + ", have a " + gift + "."
        }
    )

In addition to methods, you can use bind expressions to define object variables:

### Variable Binding

    :::finch
    fred :: (
        _name <- "Fred"
        sayName { writeLine: _name }
    }
    fred sayName ' Fred

If the name is an object variable name like `_name` here, it just defines that
variable on the object. If you use a name without a leading underscore, then it will define an object variable with that name and automatically add an accessor message. In other words, this:

    :::finch
    fred :: (
        band <- "Right Said Fred"
    )

Is exactly the same as doing:

    :::finch
    fred :: (
        _band <- "Right Said Fred"
        band { _band }
    )

## Combining Expressions: Precedence and Associativity

OK, so we've got the building blocks. Now let's talk about how they interact. The two keys parts are precedence and associativity. Precedence determines which expression binds "tighter" when different expression types are mixed together. From lowest (loosest) to highest, we have: binds, sequences, assignment, keyword messages, operators, then unary messages. For example, given an expression like this:

    :::finch
    a <- 8 + 2 neg mod: 4 - 2
    write: a

Finch will parse that like this:

    :::finch
    ((a <- ((8 + (2 neg)) mod: (4 - 2))); (write: a))

Associativity controls how a series of the same type of expression is interpreted. Unary and operator messages associate to the left. A series of keywords will be parsed into a single keyword. Assignment is right-associative. Or, by example:

    :::finch
    3 neg abs square            ' is parsed as (((3 neg) abs) square)
    1 + 2 * 3 - 4               ' is parsed as (((1 + 2) * 3) - 4)
    dict addKey: 123 value: "v" ' is parsed as a single "addKey:value:" message
    a <- b <- c <- 4            ' is parsed as (a <- (b <- (c <- 4)))

That's pretty much the entirety of Finch's syntax. It's different, but I think it's a good bit simpler than most languages out there. (I'm looking at you C++.)
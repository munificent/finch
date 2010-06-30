^title Expressions

Expressions are the heart of any programming language&mdash; they're the building blocks for programs. Finch, like many functional languages but unlike most C-derived languages, does have statements, only expressions. We'll go over the different kinds of expressions Finch supports, starting from the bottom up.

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

Variable names in Finch are more flexible than in most other languages. They must start with a letter or an underscore, but pretty much everything is fair game after that. All of these are valid variable names:

    :::finch
    foo
    Bar
    this2
    andPunctuation@#$%^&*-_=+<>/?!
    
There are three kinds of variables in Finch: global, local, and object. Global variables start with a capital letter. Local variables start with a lowercase letter, and object variables start with an underscore:

    :::finch
    Global
    local
    _object

## Messages

What other languages refer to as "calling a function" or "invoking a method", Finch calls "sending a message." Sending a message to an object (and remember, everything is an object) invokes a method on that object. There are three flavors of message send in Finch: unary, operator, and keyword.

### Unary Messages

An unary message has a name, but no arguments beyond the object receiving the message. You can send an unary message to an object by following the object with the name of the message:

    :::finch
    ' send the 'length' message to the string "hi there"
    "hi there" length

### Operators

Any sequence of punctuation characters can be used to define an operator, but don't go too crazy. The goal here is not to make your code look like comic strip profanity:

    ' valid punctuation characters
    + - ! @ # $ % ^ & * = < > / ? ~

    ' can also be combined
    -- ?! @*#%&#$@&!

Finch has no built-in operators. An expression like:

    :::finch
    a + b
    
Means "send a `+` message to `a`, passing in `b` as an argument." All operators have the same precedence and associate from left to right. This is unlike most other languages with hard-coded precedence levels. Parentheses are your friends here.

    1 + 2 * 3   ' evaluates to 9 in Finch
    1 + (2 * 3) ' evaluates to 6

Because there are no built-in operators, there are no unary operators in Finch. To negate an expression, instead of an unary `-` operator, send it the unary `neg` message:

    ' NOT valid
    - someVar
    
    ' OK!
    someVar neg
    
### Keyword Messages

We've covered messages that take zero arguments (unary) and one (operators). To pass more than one argument, you use keyword messages. A keyword is a name followed by a colon (`:`), or just a colon by itself. A keyword message is formed by alternating keywords and arguments. An example will help here:

    :::finch
    dictionary addKey: "some key" value: "the value"
    
This sends the `addKey:value:` message to the `dictionary` object, passing in "some key" and "the value" as arguments. You can chain as many keywords as you want in a single message (within reason):

    chef prepareSoup: tomato appetizer: calimari entree: veal dessert: cake

## Sequences

Multiple expressions can be sequenced together into a single expression by separating them with newlines. This code:

    :::finch
    write: "hi"
    write: "bye"
    
forms a single expression that writes two things. A sequence returns the value of the last expression. The returns of the other expressions are discarded.

If you want to sequence multiple expressions in a single line, you can also use a semicolon (`;`) as a separator.
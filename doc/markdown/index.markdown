^title Welcome

<span class="nav">Welcome | [Expressions](expressions.html)</span>

Welcome and thanks for coming. You're here because, for some reason, you're interested in Finch, a little programming language of mine. Finch is still very early in development so the only people likely to be interested in it are language nerds. Thus, the docs here will tend to assume you're one of those.

## What is Finch?

[Finch](http://bitbucket.org/munificent/finch/) is a simple [bytecode](http://en.wikipedia.org/wiki/Bytecode) [interpreted](http://en.wikipedia.org/wiki/Interpreted_language), purely [object-oriented](http://en.wikipedia.org/wiki/Object-oriented_programming), [prototype-based](http://en.wikipedia.org/wiki/Prototype-based_programming), [dynamically-typed](http://en.wikipedia.org/wiki/Dynamic_programming_language) programming language. It's mostly inspired by [Smalltalk](http://www.smalltalk.org/main/), [Self](http://selflanguage.org/), and [Javascript](https://developer.mozilla.org/en/About_JavaScript).

It is written in C++ with a hand-written [lexer](http://bitbucket.org/munificent/finch/src/tip/src/Syntax/Lexer.h) and [parser](http://bitbucket.org/munificent/finch/src/tip/src/Syntax/FinchParser.h). It has minimal dependencies. I want Finch to be:

*   Syntactically expressive yet minimal. Your code should look beautiful and
    do what you want.

*   An example of a small, clean C++ codebase for an interpreter. If you can 
    read C++ and want to learn more about programming languages, I hope Finch's
    code will be a good primer.

*   A language in the Smalltalk family that's friendly to people coming from a
    text file and curly brace background.

*   A minimal prototype-based dynamic language. I think prototypes are a really
    cool paradigm, but Self is too complex (and mostly dead), and Javascript
    is... well... Javascript.

*   Easily embeddable in other applications. I don't know if Finch ever will
    have real use, but if it does, it will likely be as a configuration or
    scripting language within a larger application, much like Lua.

## A Taste of the Language

Here's a little example to get you going. This little program doesn't draw, but it will tell you what turns to make to draw a [dragon curve](http://en.wikipedia.org/wiki/Dragon_curve):

    :::finch
    ' creates a single global Dragon object with a single "trace:" method for
    ' outputting the series of left and right turns needed to draw a dragon
    ' curve.
    Dragon <- Object copyWith: {
      self addMethod: "trace:" body: {|turns|
        ' a local function
        dragon <- {|n turn|
          if: n > 0 then: {
            dragon call: n - 1 : "R"
            write: turn
            dragon call: n - 1 : "L"
          }
        }
    
        dragon call: turns : "R"
        writeLine: "" ' end the line
      }
    }
    
    ' now lets try it
    Dragon trace: 5

This doesn't really give you the OOP flavor of the language, but it gives you the idea of the general look.

## Getting Started

Finch lives on bitbucket here: [http://bitbucket.org/munificent/finch](http://bitbucket.org/munificent/finch)

To play around with it, sync it down. There is both an XCode project and a makefile. Use whichever you prefer. I don't have anything set up for Windows yet, but I'm keen if someone wants to throw a VS solution at me.

### Building on Mac OS X with XCode

I mostly work on it in XCode. If you like that, here's how to get going:

1. Open the XCode project at: `src/finch.xcodeproject`
2. Build and run. Ta-da!

### Building with make

Thanks to [Steve Folta](http://www.folta.net/steve/), there is also a makefile for *nix users:

1. `cd` to the root `finch` directory.
2. `make`
3. `./finch`

### Running Finch

Once you've got it built and running, you'll be at the main interpreter prompt. Finch is a command-line app. If you run it without any arguments, it drops you into a [REPL](http://en.wikipedia.org/wiki/REPL), an interactive session. You can type in chunks of code and it will interpret them immediately. (If you run it with a single argument, it expects that to be a path to a .fin script, and it will load and run that script.)

Once you're in the REPL, you can load and execute a script using <code>load:</code>. The path must be relative to where the executable is. If you've built from XCode, you can run the tests like this:

    :::finch
    >>> load: "../../test/test.fin"

If you built from a makefile, it's:

    :::finch
    >>> load: "test/test.fin"

### Where to Go from Here

You should be good to start hacking some Finch code now. The other docs here can help get you started. The [sample scripts](http://bitbucket.org/munificent/finch/src/tip/sample/) and the [tests](http://bitbucket.org/munificent/finch/src/tip/test/) will give you some ideas, along with the Finch-related posts on [my blog](http://journal.stuffwithstuff.com/category/finch/).

If you have any questions or comments, holler at me.
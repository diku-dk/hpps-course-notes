# Compiled and Interpreted Languages


A computer can directly execute only machine code, consisting of raw
numeric data.  Machine code can be written by humans, but we usually
use symbolic *assembly languages* to make it more approachable.
However, even when using an assembly language, this form of
programming is very tedious.  This is because assembly languages are
(almost) a transparent text-based syntax on top of the raw binary
machine code, and the machine code has been designed to be efficient
to *execute*, not to be a pleasant programming experience.
Specifically, we are programming at a very low level of abstraction
when we use assembly languages, and with no good ability to build new
abstractions.  In practice, almost all progamming is conducted in
*high-level languages*.

## Low-level and High-Level Languages

For the purpose of this chapter, a high-level programming language is
a language that is designed not to directly represent the capabilities
and details of some machine, but rather to *abstract* the
mechanical details, in order to make programming simpler.  However, we
should note that "high-level" is a spectrum.  In general, the meaning
of the term "high-level programming language" depends on the speaker
and the context.  The pioneering computer scientist Alan Perlis said:
"A programming language is low-level when its programs require
attention to the irrelevant".  During the course you will gain
familiarity with the programming language C, which *definitely*
requires you to pay attention to things that are often considered
irrelevant, which makes it low-level in Perlis's eyes.  On the flip
side, we will see that the control offered by C provides some
capabilities, mostly the ability to *tune* our code for high
performance, that are for some problems *not* irrelevant.  The term
*mid-level programming language* might be a good description of C, as
it fills a niche between low-level assembly languages, and high-level
languages such as Python and F#.

Generally speaking, low-level languages tend to be more difficult to
program in, while offering greater potential performance (i.e. they
are faster).  Higher-level languages are much easier to program in,
but run slower and require more machine resources (e.g. memory).
Given the speed of modern computers, this is a price we are often
willing to pay - especially in the common case where the slowest part
of our program is waiting for information from disk or network.  Do
not make the mistake of assuming that a program written in a low-level
language is *always* faster than one written in a high-level language.
Choice of algorithm is often more important than choice of language.
Further, some high-level languages are designed specifically to
execute very quickly.  But there is no free lunch: these languages
tend to make tradeoffs in other areas.  There is no objective measure
of where a language lies on the scale of "level-ness", so while a
statement such as "Python is more high-level than C" is unlikely to
raise any objections, it is usually pointless to try to rank very
similar languages on this spectrum.

## Compilers and Interpreters

As the computer natively understands only its machine code, other
languages must be *translated* to machine code in order to run.  For
historical reasons, this is called *compilation*.  We say that a
compiler takes as input a file with a *source program*, and produces a
file containing an executable *machine program* that can be run
directly.  This is a very simplified model, for the following reasons:

1. Strictly speaking, a compiler does not have to produce machine
   code.  A compiler can also produce code in a different high level
   languag.  For example, with the rise of the web, it has become
   common to write compilers that produce JavaScript code.

2. The machine program normally cannot be *directly* executed,
   as modern systems have many layers of abstraction on top of the
   processor.  While the compiler does produce machine code, it is
   usually stored in a special file format that is understood by the
   *operating system*, which is responsible for making the
   machine code available to the processor.

3. The actual compiler contains many internal steps.  Further, large
   programs are typically not compiled all at once, but rather in
   chunks.  Typically, each *source file* is compiled to one *object
   file*, which are finally *linked* to form an executable program.


While compilers are a fascinating subject in their own right, we will
discuss them only at a practical level.  For a more in-depth
treatment, you are encouraged to read a book such as Torben Mogensen's
[Basics of Compiler
Design](http://hjemmesider.diku.dk/~torbenm/Basics/).

In contrast to compilers, an *interpreter* is a program that executes
code directly, without first translating it.  The interpreter can be a
compiled program, or itself be interpreted.  At the bottom level, we
always have a CPU executing machine code, but there is no fundamental
limit to how many layers of interpreters we can build on top.
However, the most common case is that the interpreter is a machine
code program, typically produced by a compiler.  For example, Python
is an interpreted language, and the `python` interpreter program used
by most people, is itself written in C, and compiled to machine code.

Interpreters are generally easier to construct than compilers,
especially for very dynamic languages, such as Python.  The downside
is that code that is interpreted generally runs much slower than
machine code.  This is called the *interpretive overhead*.  When a C
compiler encounters an integer expression `x + y`, then this can
likely be translated to a single machine code instruction (possibly
preceded by instructions to read `x` and `y` from memory.  In
contrast, whenever an interpreter encounters this expression, it has
to analyse it and figure out what is supposed to happen (integer
addition in this case), and then dispatch to an implementation of that
operation.  This is usually at least an order of magnitude slower than
actually doing the work.  This means that interpreted languages are
usually slower at computation than compiled languages.  However, many
programs spend most of their time waiting for user input, for a
network request, or for data from the file system.  Such programs are
not greatly affected by interpretive overhead.

As an example of interpretive overhead, let us try writing programs
for investigating the [Collatz
conjecture](https://en.wikipedia.org/wiki/Collatz_conjecture).  The
Collatz conjecture states that if we repeatedly apply the function

\\[ f(n) =
  \begin{cases}
    \frac{n}{2} & \text{if}~n~\text{is even} \\\\
    3n+1 & \text{if}~n~\text{is odd}
  \end{cases}
\\]

to some initial number greater than 1, then we will eventually
reach 1.  To investigate this function, we write the following Python
program `collatz.py`:

```Python
{{#include code/collatz.py}}
```

The program accepts an initial \\(k\\) from the user, then for every
\\(1\leq n<k\\) prints out \\(n\\) followed by the number of iterations of the
function it takes to reach \\(1\\).


In a Unix shell we can time the program for \\(k=100000\\) as follows,
where we explicitly ignore the normal output of the program[^naive]:

```
$ time python3 ./collatz.py 100000 >/dev/null

real    0m1.368s
user    0m1.361s
sys     0m0.007s
```

The `real` measurement tells us that the program took a little more
than 1.3s to run in the real world.  We'll talk about the meaning of
`user` and `sys` later in the course.

[^naive]: Note that this is a very naive way of timing programs - it's
adequate for programs that run for a relatively long time, but later
we will have to discuss better ways to measure performance.  In
particular, it ignores the overhead of starting up the Python
interpreter, and it is sensitive to noise, because we only do a single
run.

Now let us consider the same program, but written in C, which we call
`collatz.c`:

```C
{{#include code/collatz.c}}
```

C is a compiled language, so we have to run a compiler on `collatz.c`:

```
$ gcc collatz.c -o collatz
```

And then we can run it:

```
$ time ./collatz 100000 >/dev/null

real    0m0.032s
user    0m0.030s
sys     0m0.002s
```

Only 0.032s!  This means that our C program is

\\[
  \frac{1.368}{0.032} = 42.75
\\]

times faster than the Python program[^speedup].  This is not
unexpected.  The ease of use of interpreted languages comes at a
significant overhead.

[^speedup]: This is the *speedup in latency* - a concept we will
return to in a later chapter.

### Advantages of interpreters

People implement interpreters because they are easy to construct,
especially for advanced or dynamic languages, and because they are
easier to work with.  For example, when we are compiling a program to
machine code, then the compiler throws away a lot of information,
which makes it difficult to relate the generated machine code with the
code originally written by the compiler.  This makes debugging harder,
because the connection between what the machine physically *does*, and
what the programmer *wrote*, is more complicated.  In contrast, an
interpreter more or less executes the program as written by the
programmer, so when things go wrong, it is easier to explain where in
the source code the problem occurs.

In practice, to help with debugging, good compilers can generate
significant amounts of extra information in order to let special
*debugger* programs map the generated machine code to the
original source code.  However, this does tend to affect the
performance of the generated code.

Another typical advantage of interpreters is that they are
straightforwardly *portable*.  When writing a compiler that
generates machine code, we must explicitly write a code generator
every CPU architecture we wish to target.  An interpreter can be
written once in a portable programming language (say, C), and then
compiled to any architecture for which we have a C compiler (which is
essentially all of them).

As a rule of thumb, very high-level languages tend to be interpreted,
and low-level languages are almost always compiled.  Unfortunately,
things are not always so clear cut in practice, and *any* language can
in principle be compiled---it may just be very difficult for some
languages.

### Blurring the lines

Very few production languages are *pure* interpreters, in the sense
that they do no processing of the source program before executing it.
Even Python, which is our main example of an interpreted language,
does in fact compile Python source code to Python *bytecode*, which is
a kind of invented machine code that is then interpreted by the Python
*virtual machine*, which is an interpreter written in C.  We can in
fact ask Python to show us the bytecode corresponding to a function:

```
>>> import dis
>>> def add(a,b,c):
...     return a + b + c
...
>>> dis.dis(add)
  2           0 LOAD_FAST                0 (a)
              2 LOAD_FAST                1 (b)
              4 BINARY_ADD
              6 LOAD_FAST                2 (c)
              8 BINARY_ADD
             10 RETURN_VALUE
```

This is not machine code for any processor that has ever been
physically constructed, but rather an invented machine code that is
interpreted by Python's bytecode interpreter.  This is a common design
because it is faster than interpreting raw Python source code, but it
is still much slower than true machine code.

#### Just-In-Time Compilation

An even more advanced implementation technique is *just-in-time* (JIT)
compilation, which is notably used for languages such as C#, F# and
JavaScript.  Here, the source program is first compiled to some kind
of intermediary bytecode, but this bytecode is then further compiled
*at run-time* to actual machine code.  The technique is called
just-in-time compilation because the final compilation typically
occurs on the user's own machine, immediately prior to the program
running.

The main advantage of JIT compilation is that programs run much faster
than when interpreting bytecode, because we ultimately do end up
executing a machine code version of the program.  Because JIT
compilation takes place while the program is running, it is also able
to inspect the actual run-time behaviour of the program and tailor the
code generation to the actual data encountered in use.  This is useful
for highly dynamic languages, where traditional *ahead-of-time*
(AOT) compilers have difficulty generating good code.  In theory, a
JIT compiler can always be *at least as good* as an AOT
compiler, but in practice, AOT compilers tend to generate better code,
as they can afford to spend more time on compilation.  In practice,
JIT compilers are only used to compute those parts of the program that
are ``hot'' (where a lot of time is spent), and an interpreter is used
for the rest.  This tends to works well in practice, due to the maxim
that 80% of the run-time is spent in 20% of the code.  An AOT
compiler will not know which 20% of the code is actually hot, and so
must dedicate equal effort to every part, while a JIT compiler can
measure the run-time behaviour of the program, and see where it is
worth putting in extra effort.

The main downside of JIT compilation is that it is difficult to
implement.  It has been claimed that AOT compilers are 10 times as
difficult to write as interpreters, and JIT compilers are 10 times as
difficult to write as AOT compilers.

## Tombstone Diagrams

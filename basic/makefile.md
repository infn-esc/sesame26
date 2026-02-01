---
title: How to automate the compilation
layout: main
section: basic
---

A simple way to automate the compilation steps is to write a `Makefile`.

Here is a simple `Makefile` for the `pi_time` example:
```make
pi_time: pi_time.cpp
	g++ pi_time.cpp -std=c++20 -O2 -Wall -Wextra -o pi_time
```

Save it with the name `Makefile` in the same directory as `pi_time.cpp`, and run `make`:
```bash
$ make
g++ pi_time.cpp -std=c++20 -O2 -Wall -Wextra -o pi_time
```

### What is a `Makefile`

A `Makefile` is a text file with a list of **RULES** that describe how to build
a **TARGET** (in this case, `pi_time`) from some **DEPENDENCIES** (in this case,
`pi_time.cpp`) running various commands (`g++ ...`):
```make
TARGET: DEPENDENCIES
	COMMAND
	COMMAND
	...
```

Note that the lines with the COMMANDS must start with a `<tab>` character, not
spaces!


### What is it for ?

The main reasons for writing and using a `Makefile` are to simplify the
compilation, and to build only what is needed.

By writing down the compilation rules, you don't need to remember all the
compiler commands and flags: just write them down once, and run `make` any time
you want to rebuild the code.
This makes it easier to always use the same compilation steps, but also to set
up some alternatives, as we will see below.

Another advantage is that `make` will build a **TARGET** only if it does not
exist, or if it is older than any of its **DEPENDENCIES**.
If you re-run `make` you should see a message like
```bash
$ make
make: 'pi_time' is up to date.
```


### Dependencies

When you run `make`, it will check if the target exists or not.
If if does, it will compare its timestamp with that of the dependencies, and
rebuild the target only if any of the dependencies is newer that it is.

If we make any changes to the sources, or just "touch" them to update their
timestamp, `make` will happily rebuild the target when we ask it:
```bash
$ touch pi_time.cpp
$ make
g++ pi_time.cpp -std=c++20 -O2 -Wall -Wextra -o pi_time
```

### Variables

Once a `Makefile` gorws in complexity - for example if you have multiple source
files to build and link together - it becomes helpful to use variables to store
commands and flags.

For example:
```make
CXX := g++
CXXFLAGS := -std=c++20 -Wall -march=native

pi_time: pi_time.cpp Makefile
	$(CXX) pi_time.cpp $(CXXFLAGS) -o pi_time
```

Note that We added the `Makefile` itself to the list of dependencies, so `make`
will rebuild the target when the rules are updated.

Variables are written in uppercase by convention, but almost any word will work.

To _set_ a variable use the `:=` operator (`=` also works, but with a different
behaviour):
```make
CXXFLAGS := -std=c++20 -Wall -march=native
```

To _expand_ a variable, write it between `$(` and `)`
```make
... $(CXXFLAGS) ...
```

Make also provides some "automatic variables":

```make
CXX := g++
CXXFLAGS := -std=c++20 -Wall -march=native

pi_time: pi_time.cpp Makefile
	$(CXX) $< $(CXXFLAGS) -o $@
```

`$@` expands to the current **TARGET**, in this case `pi_time`.

`$<` expands to the _first_ dependency, in this case `pi_time.cpp`.


### Multiple targets

Now that we have variables, we can write a `Makefile` to build multiple targets
using similar commands.

For example, we can add
```make
deb/pi_time: pi_time.cpp Makefile
        mkdir -p deb
        $(CXX) $< $(CXXFLAGS) -Og -g -o $@
```
to build a _debug_ version of the example.

But if we run `make`, it will build only the regular `pi_time`... why ?

By default, `make` will build only the first target.
So, a commond approach is to add a first "phony" or "fake" target that lists as
dependnecies all the targets that we want to build:
```
.PHONY: all

all: release debug

release: pi_time

debug: deb/pi_time

...
```

Now `make` will build both `release` and `debug` targets by default, or we can
ask explicitly to build only one of them:
```bash
$ make debug
mkdir -p deb
g++ pi_time.cpp -std=c++20 -Wall -march=native -Og -g -o deb/pi_time
```


### Cleaning up

Another common "phony" target is `clean`, used to tell `make` to delete the
files it may have built:
```make
.PHONY: clean

clean:
	rm -f pi_time deb/pi_time
```

Now we can run `make clean` to delete the binaries we have built:
```
$ make clean
rm -f pi_time deb/pi_time
```

Note that, since `clean` does not have any dependencies, `make` will run its
commands every time we invoke it:
```
$ make clean
rm -f pi_time deb/pi_time
$ make clean
rm -f pi_time deb/pi_time
```

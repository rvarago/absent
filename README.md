# absent

This is a small header-only library meant to simplify the composition of nullable types in a declarative style for some C++ types constructors.

[![Build Status](https://travis-ci.org/rvarago/absent.svg?branch=master)](https://travis-ci.org/rvarago/absent)

## A word of caution

_absent_ provides an alternative way to solve a very specific problem of enabling some kinds of compositions for some kinds of nullable types. Of course, there are several ways to do achieve pretty much the same goal.

_absent_ represents a tiny contribution that aims to leverage composition via immutable operations that don't mutate the argument, instead, they create and return brand new instances.

It may be a pro, because by restricting mutation the likelihood of introducing a class of bugs may be reduced as well.

However, it may also be a con, because for each operation a new instance is created and then destroyed. Hopefully, few copies would be optimized away by the compiler, but there's no guarantee. So it's important to keep this in mind, maybe via objective measurements that prove that it's a real problem for the specific project in which _absent_ is planned to be used.

## Description

Handling nullable types has always been forcing us to add a significant amount of boilerplate in our code, in such a
way that it sometimes even hides the business logic which we are trying to express in our code.

Consider an API that makes use of nullable types, such as `std::optional<A>`. We may have the following functions:

```
std::optional<person> find_person() const;
std::optional<address> find_address(person const&) const;
zip zip_code(address const&) const;
```

A fairly common pattern in C++ would then be:

```
auto const maybe_person = find_person();
if (!maybe_person) return;

auto const maybe_address = find_address(maybe_person.value());
if (!maybe_address) return;

auto const zip_code = zip_code(maybe_address.value());
```

We have mixed business logic with error handling, it'd be nice to have these two concerns apart from each other. It's a lot
of boilerplate to achieve such a simple requirement as obtaining the zip code of a given person.

Furthermore, we have to make several calls to `std::optional<T>` accessor member function, `value()`, and for each call, we have to make sure we’ve checked that it's not empty before accessing its value.
Otherwise, it would trigger a `bad_optional_access`. Thus, it’d be nice to minimize the direct calls to `value()` by wrapping intermediary ones inside a function that does the checking and then accesses the value.
And only make the direct call to `value()` from our code at the very end of the composition.

**Note:**  We could have used the dereference operator `*` rather than `value()`. It would invoke undefined behaviour instead of
throwing an exception in case of accessing an empty `std::optional<T>`.

Now, compare it against the code that does not make use of nullable types whatsoever. And of course, it expects
that nothing can fail:

```
auto const zip_code = zip_code(find_address(find_person()));
```

Which is hopefully simpler to read and therefore to understand.

Furthermore, with the unwrapped types, we can do function composition to reduce the pipeline of function applications:

```
(void -> person) andThen (person -> address) andThen (address -> zip)
```

Where _andThen_ means the usual function composition, that evaluates the first function and then feed the return value into the second function:

```
f: A -> B, g: B -> C => (f andThen g): A -> C = g(f(x)), forall x in A
```

Since the types compose, we can reduce the pipeline of functions into a function composition:

```
(void -> zip)
```

And that's _functionally_ equivalent.

However, for nullable types we have:

```
(void -> std::optional<person>) andThen (person -> std::optional<address>) andThen (address -> zip)
```

That can't be composed, because the types don't match anymore, and so _andThen_ can't be used. We can't feed an `std::optional<person>` into a function that expects a `person`.

So, in its essence, the problem lies in the observation that introducing nullable types breaks our ability to compose the code by chaining elementary operations.

We have to find a way to combine both: type-safety brought by nullable types and the
expressiveness achieved by composing simple functions as we can do for non-nullable types.

### Enters _absent_

_absent_ provides simple abstractions based on functional programming to help us composing operations. It's inspired by
Haskell and so uses a similar vocabulary. It does **NOT** attempt to solve all problems when it comes to the complex problem of error-handling,
far away from it.

However, it abstracts away some details of an "error-as-value" API by encapsulating common patterns into functions, giving labels to repetitive pieces
of logic. Therefore, it reduces the syntactic noise of some forms of composition of nullable types and increases safety.

Furthermore, _absent_ does **NOT** provide any implementation of nullable types, instead, it aims to be generic regarding
the concrete nullable type.

Hence, an interesting caveat of _absent_ is that:

> Up to some extent, _absent_ is agnostic regarding the concrete implementation of a nullable type that one may use,
as long as it adheres to the **concept** of a nullable type expected by the library.

The main example of a nullable type that models this concept is: `std::optional<T>`, which might get a [monadic interface](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0798r3.html) soon.

Meanwhile, _absent_ may be used to fill that "monadic-gap".

And even after, it could still be interesting, since it's generic regarding the concrete nullable type implementation,
also working for optional-like types other than `std::optional<T>`. For instance, you might want to return more information to explain why a function has failed to produce a value, maybe by returning not an `std::optional<A>`, but an `types::either<A, E>`.

`types::either<A, E>` is an alias for `std::variant<A, E>`, where, by convention, `E` represents an error.
_absent_ provides combinators for `types::either<A, E>` as well.

### Getting started

_absent_ is packaged as a header-only library and, once installed, to get started with it you simply have to include the
necessary headers, for instance, `absent/absent.h` includes the main combinators that are inside the namespace `rvarago::absent`.

Since the qualified names may be too verbose, adding an alias to your project might be helpful.

### Rewriting the person/address/zip_code example using absent

Using a prefix notation, we can rewrite the above example using _absent_ as:

```
auto const maybe_zip_code = transform(and_then(find_person(), find_address), zip_code);
````

Which solves the initial problem for the lack of composition for nullable types, now we express the pipeline as:

```
(void -> std::optional<person>) and_then (person -> std::optional<address>) transform (address -> zip)
```

And that's _functionally_ equivalent to:

```
(void -> std::optional<zip>)
```

An alternative syntax is also available via infix notation based on overloaded operators:

```
auto const maybe_zip_code = find_person() >> find_address | zip_code;
````

Which is very similar to the notation used to express the pipeline:

```
(void -> std::optional<person>) >> (person -> std::optional<address>) | (address -> zip)
```

Almost as easy to read as the version without using nullable types (maybe even more?) and with the expressiveness and type-safety
brought by them.

In the case where `find_address` and `zip_code` are "getter" member functions, such as:

```
std::optional<address> person::find_address() const;
zip address::zip_code() const;
```

It's possible to wrap them inside lambdas and use `transform` and `and_then` as we did before.

However, overloads that accept getter member functions are also provided to simplify the caller code even more.

So, using the infix notation, we can do:

```
auto const maybe_zip_code = find_person() >> &person::find_address | &address::zip_code;
if (maybe_zip_code) {
    // You just have to check at the end before making the "final" use of the outcome yielded by the composition chain
}
````

To understand the above snippets, here it follows a brief explanation of the combinators `transform` and `and_then`.

#### transform (|)

One of the most basic and useful operations to allow the composition of nullable types is `transform`. Which roughly
speaking turns a nullable type containing a value of type _A_ into a functor.
 
Given a nullable _N[A]_ and a function _f: A -> B_, `transform` uses _f_ to map over _N[A]_, yielding another nullable
_N[B]_.

Furthermore, if the input nullable is empty, `transform` does nothing, and simply returns a brand new empty nullable _N[B]_.

Example:

```
auto int_to_string = [](auto const& a){ return std::to_string(a); };
std::optional<int> const one{1};
std::optional<std::string> const one_as_string = transform(one, int_to_string); // contains "1"
std::optional<int> const none{};
std::optional<std::string> const empty_as_string = transform(none, int_to_string); // contains nothing
```

To simplify the act of chaining multiple operations, an infix notation of `transform` is provided via overloading `operator|`:

```
auto const string2int = [](auto const& a){ return std::stoi(a); };
auto const int2string = [](auto const& a){ return std::to_string(a); };
std::optional<std::string> const some_zero_as_string{"0"};
std::optional<std::string> const mapped_some_of_zero_as_string = some_zero_as_string
                                                                    | string2int
                                                                    | int2string; // contains "0"
```

There's also an overload for `transform` that accepts a member function that has to be **const** and **parameterless** getter function.
So you can do this:

```
struct person {
    int id() const{ return 1; }
};
auto const maybe_id = std::optional{person{}} | &person::id; // contains 1
```

Which calls `id()` if the `std::optional<person>` contains a `person` and wraps it inside a new `std::optional<int>`.
Otherwise, in case the `std::optional<person>` does not contain a `person` it simply returns an empty `std::optional<int>`.

It's also possible to use the non-member overload for `transform`, but at the call site, the user has to wrap the member
function inside a lambda, which adds a little bit of noise to the caller code.

#### and_then (>>)

Another useful combinator is `and_then` which allows the composition of functions which by themselves also return values
wrapped in a nullable. Roughly speaking, it's modelling a monad. Please, note that you don't have to know about monads
to benefit from the practical applications of _absent_.

Given a nullable _N[A]_ and a function _f: A -> N[B]_, `and_then` uses _f_ to map over _N[A]_, yielding another nullable
_N[B]_.

The main difference if compared to `transform` is that if you apply _f_ using `transform` you end up with _N[N[B]]_.
Whereas `and_then` knows how it should flatten _N[N[B]]_ into _N[B]_ after applying the mapping function.

Suppose a scenario where you invoke a function that might fail, so you use a nullable to represent such failure.
And then you use the value inside the nullable to invoke another function that might fail as well. So and so forth.
That's precisely a good use-case to make for `and_then`.

Example:

```
auto const maybe_int_to_string = [](auto const& a){ return std::optional{std::to_string(a)}; };
std::optional<int> const one{1};
std::optional<std::string> const one_as_string = and_then(one, maybe_int_to_string); // contains "1"
std::optional<int> const none{};
std::optional<std::string> const none_as_string = and_then(none, maybe_int_to_string); // contains nothing
```

To simplify the act of chaining multiple operations, an infix notation of `and_then` is provided via overloading `operator>>`:

```
auto const maybe_string2int = [](auto const& a){ return std::optional{std::stoi(a)}; };
auto const maybe_int2string = [](auto const& a){ return std::optional{std::to_string(a)}; };
std::optional<std::string> const some_zero_as_string{"0"};
std::optional<std::string> const mapped_some_of_zero_as_string = some_zero_as_string
                                                                    >> maybe_string2int
                                                                    >> maybe_int2string; // contains "0"
```

Similarly to `transform`, there's also an overload for `and_then` that accepts a member function that has to be **const** and
**parameterless** getter function. So you can do this:

```
struct person {
     std::optional<int> id() const{ return 1; }
};
auto const maybe_id = std::optional{person{}} >> &person::id;
```

Which calls `id()` if the `std::optional<person>` contains a `person` already wrapped in an `std::optional<int>`.
Otherwise, in case the `std::optional<person>` does not contain a `person` it simply returns an empty `std::optional<int>`.

It's also possible to use the non-member overload for `and_then`, but at the call site, the user has to wrap the member
function inside a lambda, which adds a little bit of noise to the caller code.

##### Multiple error handling

Another common use-case for `and_then` is for multiple error handling.

One way to do multiple error handling if by threading a sequence of
computations that return `std::optional<T>` to represent success or fail,
and the computations shall stop as soon as the first one returns an empty
`std::optional`, meaning it has failed.

_absent_ can help here too, for instance:

```
std::optional<blank> first();
std::optional<blank> second();

auto const ok = first() >> sink(second);

if (ok) {
    // handle success
}
else {
    // handle failure
}
```

Where:

* `rvarago::absent::support::blank` is a type that conveys the idea
of a _unit_, i.e. it contains only one value or state, which is
`blank{}`, that is also exported by the helpful constant `rvarago::absent::support::unit`.
* `rvarago::absent::support::sink` wraps a callable that should receive
parameters in another callable that discards the referred parameters.

Please note that, if you had to provide parameters to `second`,
then you would have to wrap it inside a lambda that captures all required
objects, and then forwards them to `second`, for instance:

```
std::optional<blank> second(std::string, int);

std::string x = "123"s;
auto const success = first() >> support::sink([&second, &x] { return second(x, 42); });
```

It's also possible raise the level of abstraction even more by using the header `absent/support/execution_status.h`.
It, conveniently, exports the alias `execution_status` for `std::optional<blank>`, as well as the compile-time constants of type `execution_status`:

* `success` for an `execution_status` filled with a `unit`.
* `failure` for an `execution_status` filled with a `std::nullopt`.

For example:

```
execution_status first() {
    // ...
    return success;
}

execution_status second() {
    // ...
    return failure;
}

auto const ok = first() >> sink(second);

if (ok) {
    // handle success
}
else {
    // handle failure
}
```

#### for_each

Another combinator is `for_each` which allows running a function that does not return any value, so only executing an 
action with the wrapped value as a parameter. Since the action does not return anything, it's only executed because of
its side-effect, like logging a message to the console, saving an entity in the database, etc.

Given a nullable _N[A]_ and a function _f: A -> void_, `for_each` executes _f_  providing _A_ from _N[A]_ as the argument to _f_.
If _N[A]_ is empty, then `for_each` does nothing.

One use-case for `for_each` is where you would like to log the wrapped value if any. Otherwise, in case of empty
nullable, you don't want to do anything:

```
for_each(std::optional{person{}}, log);
```

Where `log` may be:

```
void log(person const&) const;
```

#### eval

Another combinator is `eval` which returns the wrapped value inside nullable if present or evaluates the
fallback function in case the nullable is empty. Therefore, providing a simulated "call-by-need" version of `std::optional<T>::value_or`.

Here, call-by-need roughly means that the evaluation of the fallback is deferred to point when it must happen,
which is: inside `eval` only when the nullable is empty.

Therefore, it avoids wasting computations as it happens with `std::optional<T>::value_or`, because in this case, the function's argument is evaluated *before* reaching `std::optional<T>::value_or` even if the nullable is not empty, in which case the value is then discarded. So, any computation that was already performed becomes useless and, maybe even more critical, it might've triggered potential side-effects that would only make sense when the nullable is empty.

Given a nullable _N[A]_ and a function _f: void -> A_, `eval` returns the un-wrapped _A_ inside _N[A]_ if it's not empty,
or evaluates _f_ that returns a fallback, or default, instance for _A_.

One use-case for `eval` is where you happen to have a default value for the nullable, but its computation is
expensive or it has some side-effect that only makes sense to be executed in case of an empty nullable. For instance:

```
eval(std::optional{person{}}, make_fallback_person);
```

Where `make_fallback_person` may be:

```
person make_fallback_person();
```

#### attempt

Sometimes we have to interface code that throws exceptions with nullable types, e.g wrap the exception into a nullable,
which shall be empty if the exception was thrown or shall contain the returned value if no exception was thrown. For
this sole purpose, _absent_ provides a combinator called `attempt`:

```
auto const result = attempt<std::optional, std::logic_error>(may_throw_an_exception);
```

Where `may_throw_an_exception` has the following signature:

```
int may_throw_an_exception();
```

So it returns either value of type `int`, in which case `result` will be an `std::optional<int>` that wraps the returned value.
Or it throws an exception derived from `std::logic_error`, in which case `result` will be an empty `std::optional<int>`.

## Requirements

### Mandatory

* C++17

### Optional

* CMake (_only if you need to build from sources_)
* Make (_only if you want to use it to orchestrate task execution_)
* Conan (_only if you want generate a package or build the tests using conan as a provider for the test framework_)
* Docker (_only if you want build from inside a docker container_)

## Build

The _Makefile_ wraps the commands to download dependencies (Conan), generate the build configuration, build, run the
unit tests, and clear the build folder.

* Compile:

```
make
```

By default, it also builds the unit tests, you can disable the behavior by:

```
make BUILD_TESTS=OFF
```

The build always assumes that the default profile (*profiles/common*) applies to your build. If that's not, then you
can specify your profile by setting _PROFILE_ as:
 
```
make PROFILE=<path_to_your_profile>
```

And to build with Release mode (by default it builds with Debug mode enabled):

``
make BUILD_TYPE=Release
``

* To run the unit tests:

```
make test
```

* To clean the _build_ folder:

```
make clean
```

### Run unit tests inside a Docker container

Optionally, it's also possible to run the unit tests inside a Docker container by executing:

```
make env-test
```

## Installing on the system

To install _absent_:

```
make install
```

This will install _absent_ into _${CMAKE_INSTALL_PREFIX}/include/absent_ and make it available into CMake local package
repository.

Then, it's possible to import _absent_ into external CMake projects, say in a target _myExample_, by simply adding the
following commands to its _CMakeLists.txt_:

```
find_package(absent REQUIRED)
target_link_libraries(myExample rvarago::absent)
```

## Package managers

To simplify the integration, _absent_ can also be provided by the following package managers:

1. [Conan](https://bintray.com/conan/conan-center/absent%3A_)

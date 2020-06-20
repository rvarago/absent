# absent

[![Build Status](https://travis-ci.org/rvarago/absent.svg?branch=master)](https://travis-ci.org/rvarago/absent)

[![Build Status](https://dev.azure.com/rvarago/absent/_apis/build/status/rvarago.absent?branchName=master)](https://dev.azure.com/rvarago/absent/_build/latest?definitionId=1&branchName=master)

_absent_ is a C++17 small header-only library meant to simplify the functional composition of operations on nullable (i.e. optional-like) types
used to represent computations that may fail.

## Description

Handling nullable types has always been forcing us to write a significant amount of boilerplate and sometimes it even obfuscates
the business logic that we are trying to express in our code.

Consider the following API that uses `std::optional<A>` as a nullable type to represent computations that may fail:

```Cpp
std::optional<person> find_person() const;
std::optional<address> find_address(person const&) const;
zip_code get_zip_code(address const&) const;
```

A fairly common pattern in C++ would then be:

```Cpp
std::optional<person> person_opt = find_person();
if (!person_opt) return;

std::optional<address> address_opt = find_address(person_opt.value());
if (!address_opt) return;

zip_code code = get_zip_code(address_opt.value());
```

We have mixed business logic with error-handling, and it'd be nice to have these two concerns more clearly separated from each other.

Furthermore, we had to make several calls to `std::optional<T>` accessor `value()`. And for each call,
we had to make sure we’d checked that the `std::optional<T>` at hand was not empty before accessing its value.
Otherwise, it would've triggered a `bad_optional_access`.

Thus, it’d be better to minimize the number of direct calls to `value()` by
wrapping intermediary calls inside a function that checks for emptiness and then accesses the value.
Hence, we would only make a direct call to `value()` from our application at the very end of the chain of operations.

Now, compare that against the code that does not make use of nullable types at all:

```Cpp
zip_code code = get_zip_code(find_address(find_person()));
```

That is possibly simpler to read and therefore to understand.

Furthermore, we can leverage function composition to reduce the pipeline of function applications:

```
(void -> person) compose (person -> address) compose (address -> zip_code)
```

Where _compose_ means the usual function composition, which applies the first function and then feeds its result into the second function:

```
f: A -> B, g: B -> C => (f compose g): A -> C = g(f(x)), forall x in A
```

Since the types compose (source and target types match), we can reduce the pipeline of functions into a function composition:

```
(void -> zip_code)
```

However, for nullable types we can't do the same:

```
(void -> std::optional<person>) compose (person -> std::optional<address>) compose (address -> zip_code)
```

This can't be composed, because the types don't match anymore, so _compose_ isn't powerful enough to be used here. We can't
simply feed an `std::optional<person>` into a function that expects a `person`.

So, in essence, the problem lies in the observation that nullable types break our ability to compose functions using the
usual function composition operator.

We want to have a way to combine both:

* Type-safety brought by nullable types.
* Expressiveness achieved by composing simple functions as we can do for non-nullable types.

### Composition with _absent_

Inspired by Haskell, _absent_ provides building-blocks based on functional programming to help us to compose computations that may fail.

It abstracts away some details of an "error-as-value" API by encapsulating common patterns into a small set of
higher-order functions that encapsulates repetitive pieces of logic. Therefore, it aims to reduce the syntactic noise that arises from the composition of nullable types and increase safety.

It worth mentioning that _absent_ does **NOT** provide any implementation of nullable types.
It rather tries to be generic and leverage existing implementations:

> Up to some extent, _absent_ is agnostic regarding the concrete implementation of a nullable type that one may use,
as long as it adheres to the **concept** of a nullable type expected by the library.

The main example of a nullable type that models this concept is: `std::optional<T>`, which may get a [monadic interface](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0798r3.html) in the future.

Meanwhile, _absent_ may be used to fill the gap. And even after, since it brings different utilities and it's also generic regarding the concrete nullable type implementation,
also working for optional-like types other than `std::optional<T>`.

For instance, a function may fail due to several reasons and you might want to provide more information to explain why a
particular function call has failed. Perhaps by returning not an `std::optional<A>`, but rather a `types::either<A, E>`. Where `types::either<A, E>` is an alias for `std::variant<A, E>`,
and, by convention, `E` represents an error. `types::either<A, E>` is provided by _absent_ and it supports a whole set
of combinators.

### Getting started

_absent_ is packaged as a header-only library and, once installed, to get started with it you simply have to include the
relevant headers.

### Rewriting the person/address/zip_code example using absent

Using a prefix notation, we can rewrite the *zip_code* example using _absent_ as:

```Cpp
std::optional<zip_code> code_opt = transform(and_then(find_person(), find_address), get_zip_code);
````

And that solves the initial problem of lack of compositionality for nullable types.

Now we express the pipeline as:

```
(void -> std::optional<person>) and_then (person -> std::optional<address>) transform (address -> zip_code)
```

And that's _functionally_ equivalent to:

```
(void -> std::optional<zip_code>)
```

For convenience, an alternative infix notation based on operator overloading is also available:

```Cpp
std::optional<zip_code> code_opt = find_person() >> find_address | get_zip_code;
````

Which is closer to the notation used to express the pipeline:

```
(void -> std::optional<person>) >> (person -> std::optional<address>) | (address -> zip_code)
```

Hopefully, it's almost as easy to read as the version without using nullable types and with the expressiveness and type-safety
that we wanted to achieve.

## Combinators

* [`transform`](#transform)
* [`and_then`](#and_then)
* [`eval`](#eval)
* [`attempt`](#attempt)
* [`for_each`](#for_each)
* [`from_variant`](#from_variant)

### <A name="transform"/>`transform`

`transform` is used when we want to apply a function to a value that is wrapped in a nullable type if such nullable
isn't empty.

>  Given a nullable _N&lt;A&gt;_ and a function _f: A -> B_, `transform` uses _f_ to map over _N&lt;A&gt;_, yielding another nullable
_N&lt;B&gt;_. If the input nullable is empty, `transform` does nothing, and simply returns a brand new empty nullable _N&lt;B&gt;_.

Example:

```Cpp
auto int2str = [](auto x){ return std::to_string(x); };

std::optional<int> one{1};
std::optional<std::string> one_str = transform(one, int2str); // std::optional{"1"}

std::optional<int> none = std::nullopt;
std::optional<std::string> none_str = transform(none, int2str); // std::nullopt
```

To simplify the act of chaining multiple operations, an infix notation of `transform` is provided by `operator|`:

```Cpp
auto int2str = [](auto x){ return std::to_string(x); };

std::optional<int> one{1};
std::optional<std::string> one_str = one | int2str; // std::optional{"1"}
```

### <A name="and_then"/>`and_then`

`and_then` allows the application of functions that themselves return nullable types.

> Given a nullable _N&lt;A&gt;_ and a function _f: A -> N&lt;B&gt;_, `and_then` uses _f_ to map over _N&lt;A&gt;_, yielding another nullable
_N&lt;B&gt;_.

The main difference if compared to `transform` is that if you apply _f_ using `transform` you end up with _N&lt;N&lt;B&gt;&gt;_
that would need to be flattened.
Whereas `and_then` knows how to flatten _N&lt;N&lt;B&gt;>_ into _N&lt;B&gt;_ after the function _f_ has been applied.

Suppose a scenario where you invoke a function that may fail and you use an empty nullable type to represent such failure.
And then you use the value inside the obtained nullable as the input of another function that itself may fail with an empty nullable.
That's where `and_then` comes in handy.

Example:

```Cpp
auto int2str_opt = [](auto x){ return std::optional{std::to_string(x)}; };

std::optional<int> one{1};
std::optional<std::string> one_str = and_then(one, int2str_opt); // std::optional{"1"}

std::optional<int> none = std::nullopt;
std::optional<std::string> none_str = and_then(none, int2str_opt); // std::nullopt
```

To simplify the act of chaining multiple operations, an infix notation of `and_then` is provided by `operator>>`:

```Cpp
auto int2str_opt = [](auto x){ return std::optional{std::to_string(x)}; };

std::optional<int> one{1};
std::optional<std::string> one_str = one >> int2str_opt; // std::optional{"1"}
```

### <A name="eval"/>`eval`

`eval` returns the wrapped value inside a nullable if present or evaluates the
fallback function and returns its result in case the nullable is empty. Thus, it provides a "lazy variant" of `std::optional<T>::value_or`.

> Given a nullable _N&lt;A&gt;_ and a function _f: void -> A_, `eval` returns the un-wrapped _A_ inside _N&lt;A&gt;_ if it's not empty,
or evaluates _f_ that returns a fallback, or default, instance for _A_.

Here, lazy roughly means that the evaluation of the fallback is deferred to point when it must happen,
which is: inside `eval` when the nullable is, in fact, empty.

Therefore, it avoids wasting computations as it happens with `std::optional<T>::value_or`, where, the function
argument is evaluated *before* reaching `std::optional<T>::value_or`, even if the nullable is not empty, in which case
the value is simply discarded.

Maybe even more seriously case is when the fallback triggers side-effects that would only make sense when the nullable is indeed empty.

Example:

```Cpp
person get_default_person();

std::optional<person> person_opt = find_person();
person p = eval(person_opt, get_default_person);
```

### <A name="attempt"/>`attempt`

Sometimes we have to interface nullable types with code that throws exceptions, for instance, by wrapping exceptions into empty nullable
types. This can be done with `attempt`.

Example:

```Cpp
int may_throw_an_exception();

std::optional<int> result = attempt<std::optional, std::logic_error>(may_throw_an_exception);
```

`may_throw_an_exception` returns either a value of type `int`, and then `result` will be an `std::optional<int>`
that wraps the returned value, or it throws an exception derived from `std::logic_error`, and then `result` will be an empty `std::optional<int>`.

### <A name="for_each"/>`for_each`

`for_each` allows running a function that does not return any value, but only executes an 
action when supplied with a value, where such value is wrapped in a nullable type.

Since the action does not return anything meaningful, it's only executed because of
its side-effect, e.g. logging a message to the console, saving an entity in the database, etc.

> Given a nullable _N&lt;A&gt;_ and a function _f: A -> void_, `for_each` executes _f_  providing _A_ from _N&lt;A&gt;_ as the argument to _f_.
If _N&lt;A&gt;_ is empty, then `for_each` does nothing.

Example:

```Cpp
void log(person const&) const;

std::optional<person> person_opt = find_person();
for_each(person_opt, log);
```

### <A name="from_variant"/>`from_variant`

`from_variant` allows us to go from an `std::variant<As...>` to a "simpler" nullable type, such as `std::optional<A>`, holding a value
of type `A` if the variant holds a value of such type, or empty otherwise.

```Cpp
std::variant<int, std::string> int_or_str = 1;
std::optional<int> one_opt = from_variant<int>(int_or_str); // std::optional{1}

int_or_str = std::string{"42"}
std::optional<int> none_opt = from_variant<int>(int_or_str); // std::nullopt
```

## Multiple error-handling

One way to do multiple error-handling is by threading a sequence of
computations that return `std::optional<T>` to represent success or failure,
and the chain of computations should stop as soon as the first one returns an empty
`std::optional`, meaning that it failed. For instance:

```Cpp
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

* `support::blank` is a type that conveys the idea of a _unit_, i.e. it can have only one possible value.
* `support::sink` wraps a callable that should receive parameters in another callable, but discards the whatever arguments
it receives.

Note that, if you had to provide parameters to `second`, then you would need to wrap it inside a lambda that captures
all the parameters, and then forwards them to `second`, for instance:

```Cpp
std::optional<blank> second(std::string, int);

std::string x = "123"s;
auto const success = first() >> support::sink([&second, &x] { return second(x, 42); });
```

It's also possible to raise the level of abstraction even more by using the alias `support::execution_status` for
`std::optional<blank>`, as well as the compile-time constants of type `execution_status`:

* `success` for an `execution_status` filled with a `unit`.
* `failure` for an `execution_status` filled with a `std::nullopt`.

For example:

```Cpp
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

## Requirements

### Mandatory

* C++17

### Optional

* CMake
* Make
* Conan
* Docker

## Build

The _Makefile_ conveniently wraps the commands to fetch the dependencies need to compile the tests using Conan, invoke
CMake to build, execute the tests, etc.

* Compile:

```
make # BUILD_TESTS=OFF to skip tests
```

* To run the tests:

```
make test
```

### Build inside a Docker container

Optionally, it's also possible to build and run the tests inside a Docker container by executing:

```
make env-test
```

## Installing on the system

To install _absent_:

```
make install
```

This will install _absent_ into _${CMAKE_INSTALL_PREFIX}/include/absent_ and make it available into your CMake local package
repository.

Then, it's possible to import _absent_ into external CMake projects, say in a target _myExample_, by adding the
following commands into the _CMakeLists.txt_:

```
find_package(absent REQUIRED)
target_link_libraries(myExample rvarago::absent)
```

## Package managers

_absent_ is also integrated into the following package managers:

1. [Conan](https://github.com/conan-io/conan-center-index/tree/master/recipes/absent)
2. [Vcpkg](https://github.com/microsoft/vcpkg/tree/master/ports/absent)

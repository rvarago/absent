# absent

[![Build Status](https://travis-ci.org/rvarago/absent.svg?branch=master)](https://travis-ci.org/rvarago/absent)

[![Build Status](https://dev.azure.com/rvarago/absent/_apis/build/status/rvarago.absent?branchName=master)](https://dev.azure.com/rvarago/absent/_build/latest?definitionId=1&branchName=master)

_absent_ is a small header-only library meant to simplify the functional composition of operations on nullable (i.e. optional-like) types
that represent computations that may fail.

## Description

Handling nullable types has always been forcing us to write a significant amount of boilerplate. Sometimes it even obfuscates
the business logic that we are trying to express in our code.

Consider the following API that uses `std::optional<A>` as a nullable type to represent computations that may fail:

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

We have mixed business logic with error-handling, it'd be nice to have these two concerns separated from each other. It's a lot
of boilerplate to achieve such a simple requirement as obtaining the zip code of a given person.

Furthermore, we had to make several calls to `std::optional<T>` accessor member function, `value()`. And for each call,
we had to make sure we’d checked that the `std::optional<T>` was not empty before accessing its value.
Otherwise, it would've triggered a `bad_optional_access`.

Thus, it’d be better to minimize the number of direct calls to `value()` by
wrapping intermediary calls inside a function that checks and then accesses the value.
Hence, we would only make a direct call to `value()` from our application at the very end of the composition.

**Note:**  We could have used the dereference operator `*` rather than `value()`. It would've invoked undefined behaviour,
instead of throwing an exception in the case of accessing an empty `std::optional<T>`.

Now, compare it against the code that does not make use of nullable types at all:

```
auto const zip_code = zip_code(find_address(find_person()));
```

Which is likely simpler to read and therefore to understand.

Furthermore, we can now do function composition to reduce the pipeline of function applications:

```
(void -> person) andThen (person -> address) andThen (address -> zip)
```

Where _andThen_ means the usual function composition, which applies the first function and then feeds its result into the second function:

```
f: A -> B, g: B -> C => (f andThen g): A -> C = g(f(x)), forall x in A
```

Since the types compose, we can reduce the pipeline of functions into a function composition that's _functionally_ equivalent:

```
(void -> zip)
```

However, for nullable types we can't do the same:

```
(void -> std::optional<person>) andThen (person -> std::optional<address>) andThen (address -> zip)
```

This can't be composed, because the types don't match anymore, so _andThen_ can't be used. We can't feed an `std::optional<person>` into a function that expects a `person`.

So, in its essence, the problem lies in the observation that nullable types break our ability to compose functions using the
usual function composition operator.

We have to find a way to combine both:

* Extra type-safety brought by nullable types.
* Expressiveness achieved by composing simple functions as we can do for non-nullable types.

### Composition with _absent_

Inspired by Haskell, _absent_ provides building-blocks based on functional programming to help us to compose operations that may fail.

It abstracts away some details of an "error-as-value" API by encapsulating common patterns into functions, giving labels to repetitive pieces
of logic. Therefore, it aims to reduce the syntactic noise that arises from the composition of nullable types and increase safety.

Furthermore, _absent_ does **NOT** provide any implementation of nullable types, it rather tries to be generic and leverage existing nullable type implementations:

> Up to some extent, _absent_ is agnostic regarding the concrete implementation of a nullable type that one may use,
as long as it adheres to the **concept** of a nullable type expected by the library.

The main example of a nullable type that models this concept is: `std::optional<T>`, which may get a [monadic interface](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0798r3.html) in the future.

Meanwhile, _absent_ may be used to fill the gap. And even after, since it brings different utilities and it's also generic regarding the concrete nullable type implementation,
also working for optional-like types other than `std::optional<T>`. For instance, you might want to return more information to explain why a function has failed, perhaps
by returning not an `std::optional<A>`, but rather a `types::either<A, E>`. Where `types::either<A, E>` is an alias for `std::variant<A, E>`,
and, by convention, `E` represents an error. `types::either<A, E>` is provided by _absent_ and it supports the whole set
of combinators.

### Getting started

_absent_ is packaged as a header-only library and, once installed, to get started with it you simply have to include the
relevant headers.

### Rewriting the person/address/zip_code example using absent

Using a prefix notation, we can rewrite the above example using _absent_ as:

```
auto const maybe_zip_code = transform(and_then(find_person(), find_address), zip_code);
````

And that solves the initial problem of lack of compositionality for nullable types, now we express the pipeline as:

```
(void -> std::optional<person>) and_then (person -> std::optional<address>) transform (address -> zip)
```

And that's _functionally_ equivalent to:

```
(void -> std::optional<zip>)
```

An alternative infix notation based on operator overloading is also available:

```
auto const maybe_zip_code = find_person() >> find_address | zip_code;
````

Which is closer to the notation used to express the pipeline:

```
(void -> std::optional<person>) >> (person -> std::optional<address>) | (address -> zip)
```

Hopefully, as easy to read as the version without using nullable types and with the expressiveness and type-safety
that we wanted.

#### transform (|)

`transform` fits the scenario where we want to apply a function that maps a value, but such value is wrapped in a nullable type.


>  Given a nullable _N&lt;A&gt;_ and a function _f: A -> B_, `transform` uses _f_ to map over _N&lt;A&gt;_, yielding another nullable
_N&lt;B&gt;_.

Furthermore, if the input nullable is empty, `transform` does nothing, and simply returns a brand new empty nullable _N&lt;B&gt;_.


Example:

```
auto int_to_string = [](auto const& a){ return std::to_string(a); };
std::optional<int> const one{1};
std::optional<std::string> const one_as_string = transform(one, int_to_string); // std::optional{"1"}
std::optional<int> const none = std::nullopt;
std::optional<std::string> const empty_as_string = transform(none, int_to_string); // std::nullopt
```

To simplify the act of chaining multiple operations, an infix notation of `transform` is provided by `operator|`:

```
auto const string2int = [](auto const& a){ return std::stoi(a); };
auto const int2string = [](auto const& a){ return std::to_string(a); };
std::optional<std::string> const some_zero_as_string{"0"};
std::optional<std::string> const mapped_some_of_zero_as_string = some_zero_as_string
                                                                    | string2int
                                                                    | int2string; // contains "0"
```

#### and_then (>>)

`and_then` allows the composition of functions that themselves return nullables.

> Given a nullable _N&lt;A&gt;_ and a function _f: A -> N&lt;B&gt;_, `and_then` uses _f_ to map over _N&lt;A&gt;_, yielding another nullable
_N&lt;B&gt;_.

The main difference if compared to `transform` is that if you apply _f_ using `transform` you end up with _N&lt;N&lt;B&gt;&gt;_
that would need to be flattened.
Whereas `and_then` knows how to flatten _N&lt;N&lt;B&gt;>_ into _N&lt;B&gt;_ after the function has been applied.

Suppose a scenario where you invoke a function that might fail and you use a nullable type to represent such failure.
And then you use the value inside the nullable to invoke another function that itself might fail, and so and so forth.
That's precisely a good use-case for `and_then`.

Example:

```
auto const maybe_int_to_string = [](auto const& a){ return std::optional{std::to_string(a)}; };
std::optional<int> const one{1};
std::optional<std::string> const one_as_string = and_then(one, maybe_int_to_string); // std::optional{"1"}
std::optional<int> const none = std::nullopt;
std::optional<std::string> const none_as_string = and_then(none, maybe_int_to_string); // std::nullopt
```

To simplify the act of chaining multiple operations, an infix notation of `and_then` is provided by `operator>>`:

```
auto const maybe_string2int = [](auto const& a){ return std::optional{std::stoi(a)}; };
auto const maybe_int2string = [](auto const& a){ return std::optional{std::to_string(a)}; };
std::optional<std::string> const some_zero_as_string{"0"};
std::optional<std::string> const mapped_some_of_zero_as_string = some_zero_as_string
                                                                    >> maybe_string2int
                                                                    >> maybe_int2string; // contains "0"
```

##### Multiple error-handling

One way to do multiple error-handling is by threading a sequence of
computations that return `std::optional<T>` to represent success or failure,
and the chain of computations should stop as soon as the first one returns an empty
`std::optional`, meaning that it failed. For instance:

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

* `support::blank` is a type that conveys the idea
of _unit_, i.e. it can have only one value.
* `support::sink` wraps a callable that should receive
parameters in another callable, but discards the referred parameters.

Note that, if you had to provide parameters to `second`,
then you would have to wrap it inside a lambda that captures all the
parameters, and then forwards them to `second`, for instance:

```
std::optional<blank> second(std::string, int);

std::string x = "123"s;
auto const success = first() >> support::sink([&second, &x] { return second(x, 42); });
```

It's also possible to raise the level of abstraction even more by using the alias `support::execution_status` for `std::optional<blank>`, as well as the compile-time constants of type `execution_status`:

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

`for_each` allows running a function that does not return any value, but only executes an 
action with the nullable type as a parameter. Since the action does not return anything, it's only executed because of
its side-effect, like logging a message to the console, saving an entity in the database, etc.

> Given a nullable _N&lt;A&gt;_ and a function _f: A -> void_, `for_each` executes _f_  providing _A_ from _N&lt;A&gt;_ as the argument to _f_.
If _N&lt;A&gt;_ is empty, then `for_each` does nothing.

A use-case for `for_each` is where you would like to log the wrapped value if any. Otherwise, in case of empty
nullable, you don't want to do anything:

```
for_each(std::optional{person{}}, log);
```

Where `log` may be:

```
void log(person const&) const;
```

#### eval

`eval` returns the wrapped value inside a nullable if present or evaluates the
fallback function and returns its result in case the nullable is empty. Thus, it provides a "lazy variant" of `std::optional<T>::value_or`.

Here, lazy roughly means that the evaluation of the fallback is deferred to point when it must happen,
which is: inside `eval` when the nullable is empty.

Therefore, it avoids wasting computations as it happens with `std::optional<T>::value_or`, where, the function
argument is evaluated *before* reaching `std::optional<T>::value_or`, even if the nullable is not empty, in which case the value is simply discarded.
So, any computation that was already performed was useless. Maybe even more seriously in case the fallback triggers side-effects that would only make sense when the nullable is indeed empty.

> Given a nullable _N&lt;A&gt;_ and a function _f: void -> A_, `eval` returns the un-wrapped _A_ inside _N&lt;A&gt;_ if it's not empty,
or evaluates _f_ that returns a fallback, or default, instance for _A_.

An use-case for `eval` is where you have a default value, but its computation is
expensive or it has some side-effect that only makes sense to be executed in case of an empty nullable. For instance:

```
eval(std::optional{person{}}, make_fallback_person);
```

Where `make_fallback_person` may be:

```
person make_fallback_person();
```

#### attempt

Sometimes we have to interface nullable types with code that throws exceptions, e.g wrap the exception into a nullable type,
which shall be empty if the exception was thrown or contain the returned value if no exception was thrown. 

This can be done with `attempt`. For instance:

```
auto const result = attempt<std::optional, std::logic_error>(may_throw_an_exception);
```

Where `may_throw_an_exception` has the following signature:

```
int may_throw_an_exception();
```

So it returns either a value of type `int`, where `result` will be an `std::optional<int>` that wraps the returned value.
Or it throws an exception derived from `std::logic_error`, where `result` will be an empty `std::optional<int>`.

#### from_variant

`from_variant` allows us to go from an `std::variant<As...>` to a "simpler" nullable type, such as `std::optional<A>`, holding a value
of type `A` if the variant holds a value of such type, or empty otherwise.

```
std::variant<int, std::string> v = 10;
std::optional<int> opt = from_variant<int>(v);
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

The _Makefile_ conveniently wraps the commands to fetch the dependencies using Conan, invoke CMake to build, run the tests, etc.

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

This will install _absent_ into _${CMAKE_INSTALL_PREFIX}/include/absent_ and make it available into CMake local package
repository.

Then, it's possible to import _absent_ into external CMake projects, say in a target _myExample_, by adding the
following commands into the _CMakeLists.txt_:

```
find_package(absent REQUIRED)
target_link_libraries(myExample rvarago::absent)
```

## Package managers

_absent_ is also integrated into the following package managers:

1. [Conan](https://bintray.com/conan/conan-center/absent%3A_)

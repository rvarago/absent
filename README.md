# absent

A simple library to compose nullable types in a declarative style for the modern C++ programmer

[![Build Status](https://travis-ci.org/rvarago/absent.svg?branch=master)](https://travis-ci.org/rvarago/absent)

## Description

Handling nullable types has always been forcing us to add a significant amount of boilerplate in our code, in such a
way that it sometimes even hides the business logic which we are trying to express in our code.

Consider an API that returns nullable types, such as std::optional<A>, for each operation that may fail. A fairly
common pattern in C++ would then be:

```
auto const maybe_person = find_person();
if (maybe_person) {
    auto const maybe_address = find_address(*maybe_person);
    if (maybe_address) {
        auto const zip_code = zip_code(*maybe_address);
        // You have to check many times against empty. Moreover, you have to do it in the middle of your logic
    }
}
```

We have mixed business logic with error handling, it'd be nice to have these two concerns apart from each other.

Obviously, it's possible to break the snippet up into separated and well-named functions. But we still
have a lot of code to accomplish such a simple task as obtaining a person's zip code.

Now, compare it against the code without employing nullable types whatsoever, and of course, expecting that nothing can
fail:

```
auto const zip_code = zip_code(find_address(find_person()));
```

A little simpler, isn't it?

The problem here is that the introduction of nullable types breaks our ability to compose the code by chaining
elementary operations. We have to find a way to combine the type-safety of nullable types together with the
expressiveness achieved by composing simple functions as we do for the non-nullable types.

### Enters _absent_

_absent_ provides simple abstractions based on functional programming to help us composing operations. It's inspired by
Haskell and so uses a similar vocabulary. It does **NOT** attempt to solve all problems, far away from it.
However, it abstracts away some details by encapsulating common patterns into functions, giving labels to repetitive pieces
of logic. Therefore, it reduces the syntactic noise of some forms of composition of nullable types.

It's important to say: _absent_ does **NOT** provide any implementation of nullable types, but it attempts to wrap the
ones that you're using.

Hence, an interesting caveat of _absent_ is that:

> _absent_ is agnostic regarding the concrete implementation of a nullable type that you're using, 
as long as it adheres to the concept of a nullable type expected by _absent_.

Mainly:

* It has to be convertible to bool in order to check the absence of a value.
* It has to support the de-reference operation to extract the contained value.

(See ```absent/nullable.h``` for more details.)

One example of a nullable type that models this concept would then be: _std::optional_, which, by the way, is going to
have a nice [monadic interface](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0798r3.html) soon.

Meanwhile, _absent_ may be used to fill the gap, and even after the introduction of a monadic interface for
_std::optional_ it could still be interesting, since it's agnostic regarding the concrete nullable types, also working
for types other than _std::optional_ as long as they adhere to the expected concept of a nullable type used by _absent_.

#### Rewriting the person/address/zip_code example using absent

Using the postfix notation, we can rewrite the above example using _absent_ as:

```
auto const maybe_zip_code = fmap(bind(find_person(), find_address), zip_code);
````

Or using the infix notation based on overloaded symbols:
```
auto const maybe_zip_code = find_person() >> find_address | zip_code;
````

Almost as simple as the version without using nullable types at all, but with the type-safety brought by them.

In case _find_address_ and _zip_code_ are member functions, it's possible to wrap them inside lambdas and use _fmap_
and _bind_. However, overloads are provided to simplify the caller code. Using the infix notation, we can do:

```
auto const maybe_zip_code = find_person() >> &person::find_address | &address::zip_code;
if (maybe_zip_code) {
    // You just have to check at the end before making the "final" use of the outcome yielded by the composition chain
}
````

To understand the above snippets, here it follows a brief explanation of the combinators _fmap_ and _bind_.

#### fmap (|)

One of the most basic and useful operations to allow the composition of nullable types is _fmap_. Which roughly
speaking turns a nullable type containing a value of type _A_ into a functor.
 
Given a nullable _N[A]_ and a function _f: A -> B_, _fmap_ uses _f_ to map over _N[A]_, yielding another nullable
_N[B]_.

Furthermore, if the input nullable is empty, _fmap_ does nothing, and simply returns a brand new empty nullable _N[B]_.

Example:

```
auto int_to_string = [](auto const& a){ return std::to_string(a); };
std::optional<int> const one{1};
std::optional<std::string> const one_as_string = fmap(one, int_to_string); // contains "1"
std::optional<int> const empty{};
std::optional<std::string> const empty_as_string = fmap(empty, int_to_string); // contains nothing
```

To simplify the act of chaining multiple operations, an infix notation of _fmap_ is provided via overloading _operator|_:

```
auto const string2int = [](auto const& a){ return std::stoi(a); };
auto const int2string = [](auto const& a){ return std::to_string(a); };
std::optional<std::string> const some_zero_as_string{"0"};
std::optional<std::string> const mapped_some_of_zero_as_string = some_zero_as_string | string2int | int2string; // contains "0"
```

There's an overload for _fmap_ that accepts a member function that has to be **const** and **parameterless** getter function.
So you can do this:

```
struct person {
    int id() const{ return 1; }
};
auto const maybe_id = std::optional{person{}} | &person::id; // contains 1
```

Which calls _id()_ if the std::optional contains a _person_ and wraps it inside a new _std::optional_. Otherwise, in
case the _std::optional_ does not contain a _person_ it simply returns an empty _std::optional_.

It's also possible to use the non-member overload for _fmap_, but at the call site the user has to wrap the member
function inside a lambda, which adds a little bit of noise in the caller code.

#### bind (>>)

Another useful combinator is _bind_ which allows the composition of functions which by themselves also return values
wrapped in a nullable, **partially** modelling a monad.

Given a nullable _N[A]_ and a function _f: A -> N[B]_, _bind_ uses _f_ to map over _N[A]_, yielding another nullable
_N[B]_. The main difference if compared to _fmap_ is that if you apply _f_ using _fmap_ you end up with _N[N[B]]_.
Whereas _bind_ knows how it should flatten _N[N[B]]_ into _N[B]_. Think of a scenario where you invoke a function
that might fail, and use a nullable to represent such failure, and then you use the value inside the nullable to invoke
another function that might also fail, and so and so forth. That's a good use case to make use of _bind_.

Example:

```
auto const maybe_int_to_string = [](auto const& a){ return std::optional{std::to_string(a)}; };
std::optional<int> const one{1};
std::optional<std::string> const one_as_string = bind(one, maybe_int_to_string); // contains "1"
std::optional<int> const empty{};
std::optional<std::string> const empty_as_string = bind(empty, maybe_int_to_string); // contains nothing
```

To simplify the act of chaining multiple operations, an infix notation of _bind_ is provided via overloading _operator>>_:

```
auto const maybe_string2int = [](auto const& a){ return std::optional{std::stoi(a)}; };
auto const maybe_int2string = [](auto const& a){ return std::optional{std::to_string(a)}; };
std::optional<std::string> const some_zero_as_string{"0"};
std::optional<std::string> const mapped_some_of_zero_as_string = some_zero_as_string >> maybe_string2int >> maybe_int2string; // contains "0"
```

Similar to _fmap_, there's an overload for _bind_ that accepts a member function that has to be **const** and
**parameterless** getter function. So you can do this:

```
struct person {
     std::optional<int> id() const{ return 1; }
};
auto const maybe_id = std::optional{person{}} >> &person::id;
```

Which calls _id()_ if the std::optional contains a _person_ already wrapped in an _std::optional_. Otherwise, in
case the _std::optional_ does not contain a _person_ it simply returns an empty _std::optional_.

It's also possible to use the non-member overload for _bind_, but at the call site the user has to wrap the member
function inside a lambda, which adds a little bit of noise in the caller code.

#### foreach

Another combinator is _foreach_ which allows running a function that does not return any value, so only executing an 
action with the wrapped value as a parameter. Since the action does not return anything, it's only executed because of
its side-effect, like logging a message to the console, saving an entity in the database, etc.

Given a nullable _N[A]_ and a function _f: A -> void_, _foreach_ executes _f_  providing _A_ from _N[A]_ as a parameter.
If _N[A]_ is empty, _foreach_ does nothing.

One use-case for _foreach_ is where you would like to log the wrapped value if any. Otherwise, in case of empty
nullable, you don't want to do anything:

```
foreach(std::optional{person{}}, log_person);
```

## Build

The _Makefile_ wraps the commands to download dependencies (Conan), generate the build configuration, build, run the
unit tests, and clear the build folder.

* Compile (by default, it also compiles the unit tests):

```
make
```

It assumes that the default profile (*profiles/common*) applies to your build, if not, then you can specify your
profile by appending _PROFILE_ as:
 
```
make PROFILE=<path_to_your_profile>
```

* Run the unit tests:

```
make test
```

* Clear the _build_ folder:

```
make clean
```

## Run tests inside a Docker container

Optionally, it's possible to run the tests inside a Docker container by running:

```
make env-test
```

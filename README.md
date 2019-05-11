# absent

A simple library to compose nullable types in a declarative style for the modern C++ programmer

[![Build Status](https://travis-ci.org/rvarago/absent.svg?branch=master)](https://travis-ci.org/rvarago/absent)

## Description

Handling nullable types has always been enforcing us to add a significant amount of boilerplate in our code, in such a way that it
sometimes even hides the business logic which we are trying to code it up.

Consider an API that returns nullable types, such as std::optional<A>, for each operation that may fail. A fairly
common pattern in C++ would then be:

```
auto const maybe_person = find_person();
if (maybe_person) {
    auto const maybe_address = (*maybe_person).address();
    if (maybe_address) {
        auto const zip_code = (*maybe_adress).zip_code();
    }
}
```

Obviously, it's possible to break the snippet up into separated and well named functions. But we
still have a lot of code to accomplish such a simple task as obtaining a person's zip code, anyways.

Now, compare it against the code without employing nullable types whatsoever, and of course, expecting that nothing can
fail:

```
auto const zip_code = find_person().address().zip_code();
```

So much simper, isn't it?

The problem here is that the introduction of nullable type breaks our ability to compose the code by chaining
elementary operations. We have to find a way to combine the type safety of nullable types together with the
expressiveness of chaining operations as we do for the non-nullable types. Shall we?

### Enters _absent_

_absent_ provides simple abstractions based on functional programming to help us composing operations. It does **NOT**
solve all problems, far away from it. However, it abstracts away some details and so partially reduces the syntactic
noise of some forms of composition of nullable types.

It's important to say: _absent_ does **NOT** provide any implementation of nullable types, but it attempt to wrap the
one that you're using.

Hence, a interesting caveat of _absent_ is that: it's agnostic regarding the concrete implementation of nullable types
that you're using, as long as it adheres to concept of nullable type expected by _absent_. Mainly:

* It has to be convertible to bool in order to check the absence of a value.
* It has to support the de-reference operation to extract the contained value.

One example of nullable type that models this concept would then be: _std::optional_.

#### fmap

One the most basic and useful operations to allow composition of nullable types is _fmap_. Which roughly speaking turns
a nullable type containing a value of type _A_ into a functor.
 
Given a nullable _N[A]_ and a function _f: A -> B_, _fmap_ uses _f_ to map over _N[A]_, yielding another nullable
_N[B]_.

Furthermore, if the input nullable is empty, _fmap_ does nothing, and simply returns a brand new empty nullable _N[B]_.

Example:

```
auto int_to_string = [](auto& a){ return std::to_string(a); };
std::optional<int> one{1};
std::optional<std::string> one_as_string = fmap(one, int_to_string); // contains "1"
std::optional<int> empty{};
std::optional<std::string> empty_as_string = fmap(empty, int_to_string); // contains nothing
```

To simplify the act of chaining multiple operations, an infix notation of _fmap_ is provided via overloading _operator&_:

```
auto string2int = [](auto& a){ return std::stoi(a); };
auto int2string = [](auto& a){ return std::to_string(a); };
std::optional<std::string> some_zero_as_string{"0"};
std::optional<std::string> mapped_some_of_zero_as_string = some_zero_as_string & string2int & int2string; // contains "0"
```

#### bind

Another useful combinator is _bind_ which allows to compose functions which by themselves also return values wrapped in
a nullable, **partially** modelling a monad.

Given a nullable _N[A]_ and a function _f: A -> N[B]_, _bind_ uses _f_ to map over _N[A]_, yielding another nullable
_N[B]_. The main difference if compared to _fmap_ is that if you apply _f_ using _fmap_ you end up with _N[N[B]]_.
Whereas _bind_ knows how it should flatten _N[N[B]]_ into _N[B]_. Think of a scenario where you invoke a function
that might fail, and use a nullable to represent such failure, and then you use the value inside the nullable to invoke
another function that might also fail, and so and so forth. That's a good use case to make use of _bind_.

Example:

```
auto maybe_int_to_string = [](auto& a){ return std::optional{std::to_string(a)}; };
std::optional<int> one{1};
std::optional<std::string> one_as_string = bind(one, maybe_int_to_string); // contains "1"
std::optional<int> empty{};
std::optional<std::string> empty_as_string = bind(empty, maybe_int_to_string); // contains nothing
```

To simplify the act of chaining multiple operations, an infix notation of _bind_ is provided via overloading _operator>>_:

```
auto maybe_string2int = [](auto& a){ return std::optional{std::stoi(a)}; };
auto maybe_int2string = [](auto& a){ return std::optional{std::to_string(a)}; };
std::optional<std::string> some_zero_as_string{"0"};
std::optional<std::string> mapped_some_of_zero_as_string = some_zero_as_string >> maybe_string2int >> maybe_int2string; // contains "0"
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

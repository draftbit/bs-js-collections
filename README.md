# `bs-js-collections`

Bindings to primitive JavaScript `Set` and `Map` types for
BuckleScript/ReScript.

In addition to bindings to the native API, this library adds a number
of functions useful for pure functional programming, such as:

* Conversion to/from various data structures such as arrays, lists,
  dicts and `Belt` data structures
* `map`, `keep`, `reduce`, including `*WithKey` versions for the Map type
* Set operations such as `union`, `intersection` and `diff`

It is largely influenced by `Belt`, following similar naming
conventions and favoring fast-pipe argument order
(i.e. significant-data-first).

## Examples

### Sets

```reason
// Construct empty, with a single element, or from an array
let myEmptySet = JsSet.empty();
let mySetWithOneString = JsSet.singleton("hello!");
let myIntSet = [|1, 2, 3|]->JsSet.fromArray;

// Mutably add some values to the set
[|3, 4, 5|]->Belt.Array.forEach(myIntSet->JsSet.addMut);

// Mutably delete values from the set
myIntSet->JsSet.deleteMut(1);

// Map functions over the set
let myStringSet = myIntSet->JsString.map(string_of_int);

// Filter the set
Js.log(myIntSet->keep(i => i < 3)); // { 1, 2 }

// Filter and map the set
Js.log(myIntSet->keepMap(i => i == 2 ? None : Some(i->string_of_int))); // { '1', '3' }

Js.log(myIntSet->JsSet.size); // 2
```

### Maps

```reason
let myStringMap = JsMap.fromArray([|("x", 1), ("y", 2)|]);
myStringMap->JsMap.delete("x");
Js.log(myStringMap->JsMap.size); // 1
Js.log(myStringMap->JsMap.has("y")); // true
Js.log(myStringMap->Js.Map.get("y")); // 2
```

See the `.rei` files for more tails, and unit tests examples of usage.
In general we try to follow naming conventions similar to `Belt` and
a pipe-first style.

## Usable types

One of the advantages of `JsMap.t` over `Js.Dict.t` is to allow arbitrary
key types (e.g. numbers). Similarly for the values allowable
in `JsSet.t`. The compiler will set values and map keys to be any type,
but in practice only certain types will work. In particular, it must be
a type at runtime that JavaScript is able to handle.

The rule of thumb is that for map keys and set values, it must be a type
which, at runtime, takes the form of an int, a string, a boolean, etc. This
means that most enums are fine, as are opaque wrapper types.

**Do not** use records, lists, arrays, tuples, etc.

**Avoid** optional types in general or at the least provide extra testing
if it's unavoidable.

## Commands/scripts

```
yarn re:build
yarn re:clean
yarn re:watch
```

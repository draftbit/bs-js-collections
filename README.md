# `bs-js-collections`

Bindings to primitive JavaScript `Set` and `Map` types for BuckleScript/ReScript, plus
some useful bells and whistles.

## Examples

### Sets

```reason
let myIntSet = JsSet.empty();
[|1, 2, 3|]->Belt.Array.forEach(myIntSet->JsSet.add);
myIntSet->JsSet.delete(1);
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

See unit tests for more examples. In addition a number of higher-level
utility functions are included, such as `map`, `keep`, `keepMap`, etc.
In general we try to follow naming conventions similar to `Belt` and
a pipe-first style. Note that many of these functions still require tests,
so use with caution and your own testing.

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

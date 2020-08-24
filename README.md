# `bs-js-collections`

Bindings to primitive JavaScript `Set` and `Map` types for BuckleScript/ReScript.

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

See unit tests for more examples.

## Commands/scripts

```
yarn re:build
yarn re:clean
yarn re:watch
```

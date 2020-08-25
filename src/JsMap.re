// Interface to the primitive Map type in JS.
// Note that the type parameters 'k and 'v will not work arbitrarily:
//
// * 'k must be a hashable at runtime, such as a number or a string.
// * 'v should not be an `option` type due to the way lookup works. In particular,
//   this is a known failure case:
//
//   let map = fromArray([|("z", None)|]);
//   expect(map->get("z"))->toEqual(Some(None));
//
// So, use with caution!
type t('k, 'v);

[@bs.new] external empty: unit => t('k, 'v) = "Map";
[@bs.new] external fromArray: array(('k, 'v)) => t('k, 'v) = "Map";
external toArrayLike: t('k, 'v) => Js.Array.array_like(('k, 'v)) =
  "%identity";
[@bs.new]
external fromArrayLike: Js.Array.array_like(('k, 'v)) => t('k, 'v) = "Map";
[@bs.val] external toArray: t('k, 'v) => array(('k, 'v)) = "Array.from";

// Convert a map to a list
let toList: 'k 'a. t('k, 'a) => list(('k, 'a)) =
  m => m->toArray->Belt.List.fromArray;

// True if the key exists in the map
[@bs.send] external has: (t('k, 'v), 'k) => bool = "has";

// Look up a key in the map.
[@bs.send] [@bs.return nullable]
external get: (t('k, 'v), 'k) => option('v) = "get";

// How many key/value pairs in the map
[@bs.send] external size: t('k, 'a) => int = "size";

// Adding the `Mut` suffix to clarify that this is not a pure operation
[@bs.send] external setMut: (t('k, 'v), 'k, 'v) => t('k, 'v) = "set";

// `Mut` signifies that this is a mutable operation
[@bs.send] external deleteMut: (t('k, 'v), 'k) => bool = "delete";

// Iterate over *values* of the map
[@bs.send]
external forEach: (t('k, 'v), [@bs.uncurry] ('v => unit)) => unit = "forEach";

// Prefer `forEachWithKey` below but we need this binding
[@bs.send]
external forEachValueKeyU: (t('k, 'v), (. 'v, 'k) => unit) => unit =
  "forEach";

let forEachWithKey: (t('k, 'v), ('k, 'v) => unit) => unit =
  (map, f) => map->forEachValueKeyU((. v, k) => f(k, v));

[@bs.send]
external entries: t('k, 'a) => Js.Array.array_like(('k, 'a)) = "entries";

[@bs.send] external keys: t('k, 'a) => Js.Array.array_like('k) = "keys";
let keysArray: 'k 'a. t('k, 'a) => array('k) = m => m->keys->Js.Array.from;

let keysList: 'k 'a. t('k, 'a) => list('k) =
  m => m->keysArray->Belt.List.fromArray;

[@bs.send] external values: t('k, 'a) => Js.Array.array_like('a) = "values";

let valuesArray: 'k 'a. t('k, 'a) => array('a) =
  m => m->values->Js.Array.from;

let valuesList: 'k 'a. t('k, 'a) => list('a) =
  m => m->valuesArray->Belt.List.fromArray;

// Map a function over the values in a map.
let map: 'k 'a. (t('k, 'a), 'a => 'b) => t('k, 'b) =
  (m, f) => {
    let output = empty();
    m->forEachWithKey((k, v) => output->setMut(k, f(v))->ignore);
    output;
  };

// Map a function over the key/value pairs in a dict.
let mapWithKey: 'k 'a. (t('k, 'a), ('k, 'a) => 'b) => t('k, 'b) =
  (m, f) => {
    let output = empty();
    m->forEachWithKey((k, v) => output->setMut(k, f(k, v))->ignore);
    output;
  };

let keep: 'k 'a. (t('k, 'a), 'a => bool) => t('k, 'a) =
  (m, f) => {
    let output = empty();
    m->forEachWithKey((k, v) =>
      if (f(v)) {
        output->setMut(k, v)->ignore;
      }
    );
    output;
  };

let keepWithKey: 'k 'a. (t('k, 'a), ('k, 'a) => bool) => t('k, 'a) =
  (m, f) => {
    let output = empty();
    m->forEachWithKey((k, v) =>
      if (f(k, v)) {
        output->setMut(k, v)->ignore;
      }
    );
    output;
  };

let keepMap: 'k 'a 'b. (t('k, 'a), 'a => option('b)) => t('k, 'b) =
  (m, f) => {
    let output = empty();
    m->forEachWithKey((k, v) => {
      switch (f(v)) {
      | None => ()
      | Some(newV) => output->setMut(k, newV)->ignore
      }
    });
    output;
  };

let keepMapWithKey:
  'k 'a 'b.
  (t('k, 'a), ('k, 'a) => option('b)) => t('k, 'b)
 =
  (m, f) => {
    let output = empty();
    m->forEachWithKey((k, v) => {
      switch (f(k, v)) {
      | None => ()
      | Some(newV) => output->setMut(k, newV)->ignore
      }
    });
    output;
  };

// Create a copy of the map
let copy: 'k 'a. t('k, 'a) => t('k, 'a) = m => m->toArray->fromArray;

// Set a key in a dictionary, producing a new dictionary.
let setPure: 'k 'a. (t('k, 'a), 'k, 'a) => t('k, 'a) =
  (m, k, v) => m->copy->setMut(k, v);

// Create a map with a single key and value
let singleton: 'k 'a. ('k, 'a) => t('k, 'a) =
  (k, v) => fromArray([|(k, v)|]);

// Get or throw an exception if the key is not found. You can customize the
// exception with the missing key.
let getOrRaise: 'k 'a. (t('k, 'a), 'k, 'k => exn) => 'a =
  (m, k, toExn) =>
    switch (get(m, k)) {
    | None => raise(k->toExn)
    | Some(v) => v
    };

// Look up the key in the dictionary; if it's not in it, add the
// given default to the map and then return it.
let getOrSetDefaultMut: (t('k, 'a), 'k, 'a) => 'a =
  (m, k, default) =>
    switch (m->get(k)) {
    | None =>
      m->setMut(k, default)->ignore;
      default;
    | Some(v) => v
    };

// Create from a dictionary
let fromDict: Js.Dict.t('a) => t(string, 'a) =
  map => fromArray(Js.Dict.entries(map));

// Convert to a dictionary
let toDict: t(string, 'a) => Js.Dict.t('a) =
  m => Js.Dict.fromArray(toArray(m));

// Convert a Belt string map to a JS map
let fromStringBeltMap: Belt.Map.String.t('a) => t(string, 'a) =
  map => fromArray(Belt.Map.String.toArray(map));

// Convert to string map
let toStringBeltMap: t(string, 'a) => Belt.Map.String.t('a) =
  m => Belt.Map.String.fromArray(toArray(m));

// Convert a belt int map to a JS map
let fromIntBeltMap: Belt.Map.Int.t('a) => t(int, 'a) =
  map => fromArray(Belt.Map.Int.toArray(map));

// Convert belt map
let toIntBeltMap: t(int, 'a) => Belt.Map.Int.t('a) =
  m => Belt.Map.Int.fromArray(toArray(m));

// Serialize a Map to JSON. This uses significant-data-last ordering for
// mixing with encoders in `@glennsl/bs-json` (although it works standalone)
let toJson = (innerToJson, m): Js.Json.t =>
  m->map(innerToJson)->toDict->Js.Json.object_;

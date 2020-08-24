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
[@bs.new]
external fromArrayLike: Js.Array.array_like(('k, 'v)) => t('k, 'v) = "Map";
[@bs.val] external toArray: t('k, 'v) => array(('k, 'v)) = "Array.from";
let toList: 'k 'a. t('k, 'a) => list(('k, 'a)) =
  m => m->toArray->Belt.List.fromArray;
[@bs.send] external has: (t('k, 'v), 'k) => bool = "has";
[@bs.send] [@bs.return nullable]
external get: (t('k, 'v), 'k) => option('v) = "get";

// Adding the `Mut` suffix to clarify that this is not a pure operation
[@bs.send] external setMut: (t('k, 'v), 'k, 'v) => t('k, 'v) = "set";

// `Mut` signifies that this is a mutable operation
[@bs.send] external deleteMut: (t('k, 'v), 'k) => bool = "delete";

// The original function is `forEach` but this clarifies that only the values
// are iterated over. For anything else you can convert to an array(_like).
[@bs.send]
external forEachValueU: (t('k, 'v), (. 'v) => unit) => unit = "forEach";
let forEachValue = (map, f) => map->forEachValueU((. v) => f(v));

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

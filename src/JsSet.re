// Interface to the primitive Set type in JS. In practice, 'a
// must be a type that JavaScript is able to handle, so use carefully:
//
// * Primitive types (string, int, bool, undefined, null) work fine
// * Abstract versions of those types
// * Enums which have a primitive representation such as `Blue or `utf8
// * `option` versions of the above, although *NESTED OPTIONS WON'T WORK*
//
type t('a);

[@bs.new] external empty: unit => t('a) = "Set";
[@bs.new] external fromArray: array('a) => t('a) = "Set";
[@bs.val] external toArray: t('a) => array('a) = "Array.from";
[@bs.new] external fromArrayLike: Js.Array.array_like('a) => t('a) = "Set";
external toArrayLike: t('a) => Js.Array.array_like('a) = "%identity";
let toList: t('a) => list('a) = s => s->toArray->Belt.List.fromArray;
[@bs.send] external has: (t('a), 'a) => bool = "has";
[@bs.get] external size: t('a) => int = "size";

// This is a mutating operation
[@bs.send] external addMut: (t('a), 'a) => t('a) = "add";

// This is a mutating operation
[@bs.send] external deleteMut: (t('a), 'a) => bool = "delete";

[@bs.send]
external forEach: (t('a), [@bs.uncurry] ('a => unit)) => unit = "forEach";

let reduce: 'a 'b. (t('a), 'b, ('b, 'a) => 'b) => 'b =
  (set, start, f) => set->toArray->Belt.Array.reduce(start, f);

// Map a function over the set. Note that the output type must also be hashable,
// this is on the developer to get right :)
let map: (t('a), 'a => 'b) => t('b) =
  (s, f) => {
    let output = empty();
    s->forEach(v => output->addMut(f(v))->ignore);
    output;
  };

let keep: (t('a), 'a => bool) => t('a) =
  (s, f) => {
    let output = empty();
    s->forEach(v =>
      if (f(v)) {
        output->addMut(v)->ignore;
      }
    );
    output;
  };

let keepMap: (t('a), 'a => option('b)) => t('b) =
  (s, f) => {
    let output = empty();
    s->forEach(v => {
      switch (f(v)) {
      | None => ()
      | Some(newV) => output->addMut(newV)->ignore
      }
    });
    output;
  };

// Create a copy of the set
let copy: 'k 'a. t('a) => t('a) = s => s->toArray->fromArray;

// Set a key in a set, producing a new set.
let addPure: (t('a), 'a) => t('a) = (s, v) => s->copy->addMut(v);

let union: 'a. (t('a), t('a)) => t('a) =
  (set1, set2) => set2->reduce(set1->reduce(empty(), addMut), addMut);

let intersection: 'a. (t('a), t('a)) => t('a) =
  (set1, set2) => set1->keep(set2->has);

let diff: 'a. (t('a), t('a)) => t('a) =
  (set1, set2) => set1->keep(e => !set2->has(e));

// Create a set with a single item
let singleton: 'a => t('a) = v => fromArray([|v|]);

// Convert a Belt string set to a JS set
let fromStringBeltSet: Belt.Set.String.t => t(string) =
  set => fromArray(Belt.Set.String.toArray(set));

// Convert to string set
let toStringBeltSet: t(string) => Belt.Set.String.t =
  set => Belt.Set.String.fromArray(toArray(set));

// Convert a belt int set to a JS set
let fromIntBeltSet: Belt.Set.Int.t => t(int) =
  set => fromArray(Belt.Set.Int.toArray(set));

// Convert belt set
let toIntBeltSet: t(int) => Belt.Set.Int.t =
  set => Belt.Set.Int.fromArray(toArray(set));

// Convert to JSON
let toJson: ('a => Js.Json.t, t('a)) => Js.Json.t =
  (toJsonInner, set) =>
    set->toArray->Belt.Array.map(toJsonInner)->Js.Json.array;

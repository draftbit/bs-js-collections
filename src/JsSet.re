// Interface to the primitive Set type in JS. In practice, 'a
// must be a type that JavaScript is able to handle, so use carefully.
type t('a);

[@bs.new] external empty: unit => t('a) = "Set";
[@bs.new] external fromArray: array('a) => t('a) = "Set";
[@bs.val] external toArray: t('a) => array('a) = "Array.from";
let toList: t('a) => list('a) = s => s->toArray->Belt.List.fromArray;
[@bs.send] external has: (t('a), 'a) => bool = "has";

// This is a mutating operation
[@bs.send] external addMut: (t('a), 'a) => t('a) = "add";

// This is a mutating operation
[@bs.send] external deleteMut: (t('a), 'a) => bool = "delete";

[@bs.send] external forEachU: (t('a), (. 'a) => unit) => unit = "forEach";
let forEach = (set, f) => set->forEachU((. x) => f(x));

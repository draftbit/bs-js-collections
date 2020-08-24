// Some useful operations on array-like values
type t('a) = Js.Array.array_like('a);

external fromArray: array('a) => t('a) = "%identity";
let toArray: t('a) => array('a) = Js.Array.from;

// Helpful if you want to use an array operation on an array_like.
let onArray: 'a 'b. (t('a), array('a) => array('b)) => t('b) =
  (al, f) => al->toArray->f->fromArray;

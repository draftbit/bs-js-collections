open Jest;
open Expect;

module MapTests = {
  open JsMap;
  describe("Map", () => {
    test("string keys", () => {
      let map = fromArray([|("foo", "bar")|]);
      expect(map->get("foo"))->toEqual(Some("bar"));
      expect(map->has("foo"))->toBe(true);
      expect(map->get("qux"))->toEqual(None);
      expect(map->has("qux"))->toBe(false);
      expect(map->toArray)->toEqual([|("foo", "bar")|]);
      expect(map->toList)->toEqual([("foo", "bar")]);
      expect(map->keysArray)->toEqual([|"foo"|]);
      expect(map->keysList)->toEqual(["foo"]);
      expect(map->valuesArray)->toEqual([|"bar"|]);
      expect(map->valuesList)->toEqual(["bar"]);
    });
    test("int keys", () => {
      let map = fromArray([|(123, "bar")|]);
      expect(map->get(123))->toEqual(Some("bar"));
    });
    test("mutable operations", () => {
      let map = fromArray([|(123, "bar")|]);
      let _ = map->setMut(456, "qux");
      expect(map->get(456))->toEqual(Some("qux"));
      let deleted = map->deleteMut(123);
      expect(deleted)->toBe(true);
      expect(map)->toEqual(fromArray([|(456, "qux")|]));
    });
    test("option values (DO NOT USE)", () => {
      let map =
        fromArray([|
          ("x", Some(None)),
          ("y", Some(Some(123))),
          ("z", None),
        |]);
      expect(map->get("x"))->toEqual(Some(Some(None)));
      expect(map->get("y"))->toEqual(Some(Some(Some(123))));
      // NOTE this is a known fail and why option values shouldn't be used
      //    expect(map->get("z"))->toEqual(Some(None));
      expect(map->get("xxx"))->toEqual(None);
    });

    test("forEachValue", () => {
      let map = fromArray([|("a", 1), ("b", 2), ("c", 3)|]);
      let sum = ref(0);
      map->forEachValue(n => sum := sum^ + n);
      expect(sum^)->toEqual(6);
    });
  });
};

module SetTests = {
  open JsSet;

  describe("Set", () => {
    test("strings", () => {
      let set = fromArray([|"foo", "bar"|]);
      expect(set->has("foo"))->toBe(true);
      expect(set->has("qux"))->toBe(false);
      expect(set->toArray)->toEqual([|"foo", "bar"|]);
      expect(set->toList)->toEqual(["foo", "bar"]);
    });
    test("intss", () => {
      let set = fromArray([|123, 456|]);
      expect(set->has(123))->toEqual(true);
    });
    test("mutable operations", () => {
      let set = fromArray([|123|]);
      let _ = set->addMut(456);
      expect(set->has(456))->toEqual(true);
      let deleted = set->deleteMut(123);
      expect(deleted)->toBe(true);
      expect(set)->toEqual(fromArray([|456|]));
    });
    test("forEach", () => {
      let set = fromArray([|1, 2, 3|]);
      let sum = ref(0);
      set->forEach(n => sum := sum^ + n);
      expect(sum^)->toEqual(6);
    });
  });
};

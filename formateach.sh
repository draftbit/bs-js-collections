#!/usr/bin/env bash
for fileName in "$@"
do
  original="$(cat $fileName)"
  formatted="$(node_modules/.bin/bsc -format $fileName)"
  if [[ "$formatted" != "$original" ]]; then
    echo "$fileName changed after formatting"
    echo "$formatted" > $fileName
  fi
done

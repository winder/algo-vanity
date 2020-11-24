#!/usr/bin/env bash

if [ -z $1 ]; then
  echo "Usage: ./$0 <prefix>"
fi

PRE=${1^^}
echo "Searching for $PRE"

while true; do
  GEN=$(algokey generate)
  if [[ "${GEN}" == *"key: $PRE"* ]]; then
    echo "$GEN"
    exit
  fi
done

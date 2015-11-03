#!/bin/zsh

function die { echo 1>&2 $*; exit 1 }

((ARGC)) || die "Usage: $0 main.cpp other.cpp..."

main=$1; shift

g++ -g -O2 -fPIC -o $main:r $* $main

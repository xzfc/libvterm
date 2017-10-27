#!/usr/bin/env sh

usage() {
	echo "Usage: $1 <vterm_dir> <input_file>" 1>&2
}

[ "$#" -eq 2 ] || { usage "$0"; exit 1; }

LIBVTERM=$1

clang -I$LIBVTERM/include -L$LIBVTERM/.libs -Wl,-rpath=$LIBVTERM/.libs -lvterm go.c -o go || exit 1
./go    < $2
./go -b < $2

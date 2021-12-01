#!/bin/sh

BASEDIR=$(dirname $0)
OUTDIR="$BASEDIR/out"

cd "$BASEDIR" && ./yes-no.sh -N -m "Do you want to remove benchmark files ?" && rm -rv "$OUTDIR"

exit 0
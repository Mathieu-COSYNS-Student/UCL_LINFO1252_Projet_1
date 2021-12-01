#!/bin/sh

BASEDIR=$(dirname $0)
OUTDIR="$BASEDIR/out"

"$BASEDIR/yes-no.sh" -N -m "Do you want to remove benchmark files ?" && rm -rfv "$OUTDIR"

exit 0
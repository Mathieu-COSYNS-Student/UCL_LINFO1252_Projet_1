#!/bin/sh

echo "Benchmark running..."

BASEDIR=$(dirname $0)
OUTDIR="$BASEDIR/out"

[ ! -d "$OUTDIR" ] && mkdir "$OUTDIR"

DOUBLE_THREADS=$(expr "$(nproc) * 2" | bc)

for i in $(seq 1 3)
do
  $BASEDIR/test_time.sh --type bigest_first \
    "$BASEDIR/../bin/readers_writers_$i" > "$OUTDIR/readers_writers_$i.csv"
done

for i in $(seq 1 2)
do
  $BASEDIR/test_time.sh --max-threads $DOUBLE_THREADS \
  "$BASEDIR/../bin/lock_simple_test_$i" > "$OUTDIR/lock_simple_test_$i.csv"
done
#!/bin/sh

echo "Benchmark running..."

BASEDIR=$(dirname $0)
OUTDIR="$BASEDIR/out"

[ ! -d "$OUTDIR" ] && mkdir "$OUTDIR"

# Tache 1.1
for i in $(seq 1 3)
do
  "$BASEDIR/test_time.sh" --min-threads 2 \
    "$BASEDIR/../bin/philosophes_$i" > "$OUTDIR/philosophes_$i.csv"
done

# Tache 1.2
for i in $(seq 1 3)
do
  "$BASEDIR/test_time.sh" --min-threads 2 --type smallest_first \
    "$BASEDIR/../bin/producteurs_consommateurs_$i" > "$OUTDIR/producteurs_consommateurs_$i.csv"
done

# Tache 1.3
for i in $(seq 1 3)
do
  "$BASEDIR/test_time.sh" --type bigest_first \
    "$BASEDIR/../bin/readers_writers_$i" > "$OUTDIR/readers_writers_$i.csv"
done

# Tache 2.2
DOUBLE_THREADS=$(expr "$(nproc) * 2" | bc)

for i in $(seq 1 2)
do
  "$BASEDIR/test_time.sh" --max-threads $DOUBLE_THREADS \
  "$BASEDIR/../bin/lock_simple_test_$i" > "$OUTDIR/lock_simple_test_$i.csv"
done
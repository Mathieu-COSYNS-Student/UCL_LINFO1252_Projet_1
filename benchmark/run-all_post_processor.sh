#!/bin/sh

echo "Benchmark-post running..."

BASEDIR=$(dirname $0)
OUTDIR="$BASEDIR/out"

# Tache 1.1
"$BASEDIR/test_time_post_processor.py" \
  -t "Philosophes" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/philosophes_1.csv" -m 1 \
  -i "$OUTDIR/philosophes_2.csv" -m 2 \
  -i "$OUTDIR/philosophes_3.csv" -m 3

# Tache 1.2
"$BASEDIR/test_time_post_processor.py" \
  -t "Producteurs-consomateur" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/producteurs_consommateurs_1.csv" -m 1 \
  -i "$OUTDIR/producteurs_consommateurs_2.csv" -m 2 \
  -i "$OUTDIR/producteurs_consommateurs_3.csv" -m 3

# Tache 1.3
"$BASEDIR/test_time_post_processor.py" \
  -t "Lecteurs et érivains" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/readers_writers_1.csv" -m 1 \
  -i "$OUTDIR/readers_writers_2.csv" -m 2 \
  -i "$OUTDIR/readers_writers_3.csv" -m 3
  
# Tache 2.2
"$BASEDIR/test_time_post_processor.py" \
  -t "Attante active" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/lock_simple_test_1.csv" -m "test-and-set" \
  -i "$OUTDIR/lock_simple_test_2.csv" -m "test-and-test-and-set" \
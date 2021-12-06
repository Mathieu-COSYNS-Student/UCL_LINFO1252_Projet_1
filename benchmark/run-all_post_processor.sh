#!/bin/sh

echo "Benchmark-post running..."

BASEDIR=$(dirname $0)
OUTDIR="$BASEDIR/out"
GRAPHICS="--no-graphics"

if "$BASEDIR/yes-no.sh" -m "Do you want to see charts in a graphical interface ?"; then
  GRAPHICS=""
fi

# Tache 1.1
"$BASEDIR/test_time_post_processor.py" \
  -t "Problème des philosophes" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/philosophes_1.csv" -m "Synchronisation POSIX" \
  -i "$OUTDIR/philosophes_2.csv" -m "Synchronisation avec test-and-set" \
  -i "$OUTDIR/philosophes_3.csv" -m "Synchronisation avec test-and-test-and-set" \
  -o "$OUTDIR/philosophes" "$GRAPHICS" &

# Tache 1.2
"$BASEDIR/test_time_post_processor.py" \
  -t "Problème des producteurs-consommateurs" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/producteurs_consommateurs_1.csv" -m "Synchronisation POSIX" \
  -i "$OUTDIR/producteurs_consommateurs_2.csv" -m "Synchronisation avec test-and-set" \
  -i "$OUTDIR/producteurs_consommateurs_3.csv" -m "Synchronisation avec test-and-test-and-set" \
  -o "$OUTDIR/producteurs_consommateurs" "$GRAPHICS" &

# Tache 1.3
"$BASEDIR/test_time_post_processor.py" \
  -t "Problème des lecteurs et écrivains" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/readers_writers_1.csv" -m "Synchronisation POSIX" \
  -i "$OUTDIR/readers_writers_2.csv" -m "Synchronisation avec test-and-set" \
  -i "$OUTDIR/readers_writers_3.csv" -m "Synchronisation avec test-and-test-and-set" \
  -o "$OUTDIR/readers_writers" "$GRAPHICS" &
  
# Tache 2.2
"$BASEDIR/test_time_post_processor.py" \
  -t "Attente active" \
  -x "Nombre de threads totaux" \
  -y "Nombre de secondes" \
  -i "$OUTDIR/lock_test_1.csv" -m "test-and-set" \
  -i "$OUTDIR/lock_test_2.csv" -m "test-and-test-and-set" \
  -o "$OUTDIR/lock_test" "$GRAPHICS" &

wait $(jobs -p)
#!/bin/sh

print_usage()
{
  echo "Usage: $0 [OPTION]... command
	
DESCRIPTION
	Benchmark execution time for a command and output an csv on stdout

	--attempts [number]
		set the number of attempts. Default is 5

	--help
		print this message

	--max-threads [number]
		maximum number of threads used by this script. Default is the result of nproc

	--min-threads [number]
		minmum number of threads used by this script. Default is 1

	--type [type]
		set the type. Default is unique

		Available type:

		unique
			the number of thread is passed as the first argument
		bigest_first
			the number of thread is divided by 2. In case of odd number the bigest number is passed as the first argument
		smallest_first
			the number of thread is divided by 2. In case of odd number the smallest number is passed as the first argument
" >&2
}

# Default options
type="unique"
min_threads=1
max_threads=$(nproc)
number_of_attempt=5

while test $# -gt 0; do
	case "$1" in
		--help)
			print_usage
			exit 0
		;;
		--type)
			type="$2"
			shift 2
		;;
		--min-threads)
			min_threads="$2"
			shift 2
		;;
		--max-threads)
			max_threads="$2"
			shift 2
		;;
		--attempts)
			number_of_attempt="$2"
			shift 2
		;;
		*)
			break
		;;
	esac
done 

case $type in
	unique|bigest_first|smallest_first)
	;;
	*)
		echo "type unknown\n" >&2
		print_usage
		exit 1
	;;
esac

if [ "$#" -lt 1 ]; then
	print_usage
	exit 1
fi

cmd="$@"

# echo "Executing benchmark for command \"$cmd\""

echo "threads,attempt_number,execution_time"

for i in $(seq $min_threads $max_threads)
do   
	for j in $(seq 1 $number_of_attempt)
	do
		case $type in
			unique)
				time=$(/usr/bin/time -f "%e" $cmd $i 2>&1 | tail -n1)
				;;
			bigest_first|smallest_first)
				smallest=$(expr "$i / 2" | bc)
				bigest=$(expr "$smallest + $(expr "$i % 2" | bc)" | bc)
				
				if [ "$type" = "bigest_first" ]; then
					time=$(/usr/bin/time -f "%e" $cmd $bigest $smallest 2>&1 | tail -n1)
				else
					time=$(/usr/bin/time -f "%e" $cmd $smallest $bigest 2>&1 | tail -n1)
				fi

				;;
		esac
		echo "$i,$j,$time"
	done
done

exit 0

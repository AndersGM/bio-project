#!/usr/bin/env bash
#TODO experiment with larger alphabet
#TODO experiment with fractions of the string size
mkdir -p results
set -e
epochs=100
$(rm exp.* |true) # because i'm no sure whether o construction will just append the file if it exists
for alphabet in "abcdefghijklmnop"
do
    mkdir -p results/${alphabet}
    for (( i=500; i<=500; i=i*2 ))
    do
        # make table headers
        filename="results/${alphabet}/exp_${i}_${epochs}.csv"
        filename_co="results/${alphabet}/exp_${i}_${epochs}_co.csv"
        echo "step,m,time" > ${filename}
        echo "step,time" > ${filename_co}
        echo "generating for i=${i}"
        # ./bwt -f exp -b -g -l ${i} -a ${alphabet}
        python3 random_string.py ${i} ${alphabet} exp.x
        ./mksary exp.x exp.sa # generate sa

        for ((s = 1; s <= 128; s=s*2))
        do
            echo "building o for ${s}"

            build_cmd="./bwt -f exp -b dummy -s ${s} -a ${alphabet}"

            walltime=`command time -p ${build_cmd} 2>&1 1> /dev/null | awk '/^real/ { print $2 }'`
            echo "${s},${walltime}" >> ${filename_co}

            for ((m = 2; m <=150000; m=m+5))
            do
                if [ $m -ge $i ]; then
                    echo "too large"
                    continue
                fi

                echo "warming up.."
                # warmup to get the queries into memory
                for ((epoch = 1; epoch <=$epochs; epoch++))
                do
                    query="./bwt -b -f exp -p ${m} -a ${alphabet}"
                    ${query} > exp.q
                    mapper_cmd="./bwt -r dummy -s ${s} -f exp -q file -a ${alphabet}"
                    ${mapper_cmd}
                done
                echo "warm"

                echo "starting"
                for ((epoch = 1; epoch <=$epochs; epoch++))
                do
                    query="./bwt -b -f exp -p ${m} -a ${alphabet}"
                    ${query} > exp.q
                    mapper_cmd="./bwt -r dummy -s ${s} -f exp -q file -a ${alphabet}"
                    result=$($mapper_cmd)
                    echo "${s},${m},${result}" >> ${filename}
                done
                echo "done"
            done
        done
        $(rm exp.*)
    done
done

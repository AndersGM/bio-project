#!/usr/bin/env bash

mkdir -p results
epochs=10
    # string length
#for (( i=2000000000; i<=200000000000; i=i*2 ))
for (( i=200000000; i<=20000000000; i=i+1000000))
do
    # make table headers
    filename="results/exp_${i}_${epochs}.csv"
    echo "step,m,time" > ${filename}

    echo "generating for i=${i}"
    ./bwt -f exp -b -g -l ${i} # preprocess
    for ((s = 1; s <= 64; s=s*2))
    do
        echo "building o for ${s}"
        ./bwt -f exp -b dummy -s ${s}

        for ((m = 1; m <=30; m=m+5))
        do
            # warmup to get the queries into memory
            for ((epoch = 1; epoch <=10; epoch++))
            do
                query=$(cat /dev/urandom | tr -dc 'acgt' | fold -w ${m} | head -n 1)
                mapper_cmd="./bwt -r dummy -s ${s} -f exp -q ${query}"
                ${mapper_cmd}
                if [ "$?" -ne 0 ]; then
                    echo "error"
                    exit 1
                fi
            done
            echo "${s},${m},${wall}" >> ${filename}
        done
    done
done

$(rm exp.*)

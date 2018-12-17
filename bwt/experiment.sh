#!/usr/bin/env bash
#TODO experiment with larger alphabet
#TODO experiment with fractions of the string size
mkdir -p results
set -e
epochs=100
# $(rm exp.* |true) # because i'm no sure whether o construction will just append the file if it exists
for alphabet in "abcdefghijklmnop"
do
    mkdir -p results/${alphabet}
    #for i in 5000000 20000000 40000000 256000000
    #for i in 512000000
    for i in 56000 60000 80000
    do
        # make table headers
        filename="results/${alphabet}/exp_${i}_${epochs}.csv"
        filename_co="results/${alphabet}/exp_${i}_${epochs}_co.csv"
        echo "step,m,time" > ${filename}
        echo "step,time" > ${filename_co}
        echo "generating for i=${i}"
         ./bwt -f exp -b -g -l ${i} -a ${alphabet}
        python3 random_string.py ${i} ${alphabet} exp.x
        ./mksary exp.x exp.sa # generate sa

        for ((s = 1; s <= 128; s=s*2))
        do
            echo "building o for ${s}"

            build_cmd="./bwt -f exp -b dummy -s ${s} -a ${alphabet}"

            walltime=`command time -p ${build_cmd} 2>&1 1> /dev/null | awk '/^real/ { print $2 }'`
            echo "${s},${walltime}" >> ${filename_co}

            for m in 5 10 15 20 25 30 35 40 50 60 70 80 90 100 200 300 400 500 600 700 800 900 1000 1500 2000 2500 3000 3500 4000 4500 5000 5500 6000 6500 7000 7500 8000 8500 9000 9500 10000 15000 20000 25000 30000 35000 40000 # 50000 60000 70000 80000 90000
            do
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

#!/bin/bash

db_path="/mnt/pmemdir/"

outfilepath="..."
# output file
outfile="$outfilepath/result.out"
# db_bench binary path
bench_path=".."

# test type in db_bench
test_type="fillrandom,stats,readrandom"

# value array 65536
value_array=( 512 1024 4096 16384 65536)
#value_array=(16384)

write_buffer_size=67108864
write_key_num=1000000

# write buffer size array
write_buffer_size_array=(2097152 4194304 8388608 16777216 33554432 67108864)
# write_buffer_size_array=( 33554432 67108864)

# data size 1GB
#data_size="1073741824"

# 10GB
# data_size="10737418240"
# 20GB
 data_size="21474836480"
# 40GB
# data_size="42949672960"
# 80GB
# data_size="85899345920"


CLEAN_DB_PATH() {
    if [ -d "$db_path" ];then
        echo "Clean db path!"
        rm -rf $db_path*
    fi
}

CREATE_OUTPUT_FILE_PATH() {
    if [ ! -d "$outfilepath" ];then
        echo "Make output file path!"
        mkdir $outfilepath
    fi
}


RUN_ONE_TEST() {
    CLEAN_DB_PATH
    parameters="--num=$write_key_num \
                --value_size=$size \
                --benchmarks=$test_type \
                --write_buffer_size=$write_buffer_size
                "
    cmd="$bench_path/db_bench $parameters >> $outfile"
    echo $cmd > "$outfile"
    echo $cmd
    eval $cmd
}

PARSER(){
echo ""
echo "--------------------------------------------------------"
echo "random write            latency       throughput"
cat $outfilepath/value-${size}.out | grep "fillrandom  "
echo "--------------------------------------------------------"
echo "random read             latency       throughput"
cat $outfilepath/value-${size}.out | grep "readrandom  "
echo ""
cat $outfilepath/value-${size}.out | grep "memtable stall time: "
cat $outfilepath/value-${size}.out | grep "L0 stall time: "
cat $outfilepath/value-${size}.out | grep "L0 slow stall time: "
echo ""
}

PARSER_VM(){
echo ""
echo "--------------------------------------------------------"
echo "random write            latency       throughput"
cat  $outfilepath/value-${size}_mem-${mem_size}.out | grep "fillrandom  "
echo "--------------------------------------------------------"
echo "random read             latency       throughput"
cat $outfilepath/value-${size}_mem-${mem_size}.out | grep "readrandom  "
echo ""
}


RUN_VALUE_TEST() {
    if [ ! -d "$outfilepath" ];then
        echo "Make output file path!"
        mkdir $outfilepath
    fi
    for value in ${value_array[@]}; do
        size="$value"
        write_key_num="`expr $data_size / $size`"
        outfile="$outfilepath/value-${size}.out"
        RUN_ONE_TEST
        PARSER >> "$outfile"
        sleep 5
    done
}

RUN_VALUE_MEMTABLE_TEST() {
    if [ ! -d "$outfilepath" ];then
        echo "Make output file path!"
        mkdir $outfilepath
    fi
    for value in ${value_array[@]}; do
        size="$value"
        write_key_num="`expr $data_size / $size`"
        for mem_size in ${write_buffer_size_array[@]}; do
            write_buffer_size="$mem_size"
            outfile="$outfilepath/value-${size}_mem-${mem_size}.out"
            RUN_ONE_TEST
            PARSER_VM >> "$outfile"
            sleep 5
        done
    done
}




CREATE_OUTPUT_FILE_PATH

RUN_VALUE_TEST

# RUN_VALUE_MEMTABLE_TEST
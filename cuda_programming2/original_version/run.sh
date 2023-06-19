#! /bin/bash

sizes=("256" "1024" "2048" "4096" "8192")
block_sizes=("32" "64" "128" "256")
output_file="time_using.txt"

echo "" > "${output_file}"

# 测试文件输出
if [ $1 == "test" ];then
    for i in {1..5}
    do
        ./mat_vec_multiply -m test -t ${i}
    done
fi

for size in "${sizes[@]}"
do
    for block_size in "${block_sizes[@]}"
    do 
        echo "参数列表: size(${size},${size})  block_size(${block_size})" >> "${output_file}"
        for i in {1..10}
        do 
            ./mat_vec_multiply -m random -b ${block_size} -i ${size} --time>> "${output_file}"
        done
    done
done
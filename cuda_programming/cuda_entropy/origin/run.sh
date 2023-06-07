#! /bin/bash

# 定义参数列表
widths=("256" "1024" "2048" "4096" "8192")
window_sizes=("5" "7" "9" "11")
output_file="time_using.txt"


echo "" > "${output_file}"

# 环遍历参数列表，运行程序并将结果输出到文件中
for  width in "${widths[@]}"
do
    for window_size in "${window_sizes[@]}"
    do  
        echo "参数列表: width(${width}) height(${width}) window_size(${window_size})" >> "${output_file}"
        for i in {1..10}
        do 
            echo "" > "random_input.txt"
            echo "" > "entropy.txt"
            ./entropy.o "${width}" "${width}" "${window_size}" >> "${output_file}"
        done
    done
done
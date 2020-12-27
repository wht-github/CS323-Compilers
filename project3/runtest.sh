#!/bin/bash
parser=$1
testpath=$2/*.spl
output_path=${2}_out
mkdir -p ${output_path}
for i in ${testpath}; do
    name=$(basename $i)
    base=${name%.spl}
    $($parser $i  &>${output_path}/$base.out )
    echo $i
    # diff $2/$base.out ${output_path}/$base.out
done

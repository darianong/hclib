#!/bin/bash

[ -z "$PROJECT_NAME" ] && PROJECT_NAME=$(basename $PWD)

do_test() {
    set -x
    eval make $1 $PROJECT_MAKE_ARGS
    eval ./$1 $HC_BIN_FLAGS $PROJECT_RUN_ARGS
    if [ "$PROJECT_VERIFY" ]; then
        eval $PROJECT_VERIFY
    fi
}


export HCLIB_WORKERS=8
mkdir log


for prefix in f fh t th nb hclang; do
    if [ $prefix = hclang ]; then
        # --wf -> work first
        # --hf -> help first
        export HC_BIN_FLAGS="--nproc $HCLIB_WORKERS --hf" 
    fi
    for i in {1..30}; do
        echo "===> $prefix $i"
        do_test ${prefix}_${PROJECT_NAME}
    done &>log/${prefix}_data.txt
done

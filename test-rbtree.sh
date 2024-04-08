#!/bin/zsh

# test 폴더의 Makefile에서 clean 실행
cd test
make clean

# src 폴더의 Makefile에서 clean 실행
cd ../src
make clean

# test 폴더로 돌아가서 test 실행
cd ../test
make test

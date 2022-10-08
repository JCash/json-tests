#!/usr/bin/env bash

set -e

mkdir -p build

clang -O3 -flto -fpic -c -o build/main.o src/main.c

clang -Os -flto -fpic -o build/null src/example_null.c build/main.o

YYJSON_DIR=../yyjson
clang -Os -flto -fpic -o build/yyjson -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1  build/main.o src/example_yyjson.c ${YYJSON_DIR}/src/yyjson.c -I${YYJSON_DIR}

CJSON_DIR=../cjson
clang -O3 -flto -fpic -o build/cjson  build/main.o src/example_cjson.c ${CJSON_DIR}/cJSON.c ${CJSON_DIR}/cJSON_Utils.c -I${CJSON_DIR}

PARSON_DIR=../parson
clang -O3 -flto -fpic -o build/parson build/main.o src/example_parson.c ${PARSON_DIR}/parson.c -I${PARSON_DIR}

ZZZJSON_DIR=../zzzjson
clang -O3 -flto -fpic -o build/zzzjson build/main.o src/example_zzzjson.c -I${ZZZJSON_DIR}

LIB_INCLUDE=./modules/simdjson/singleheader
LIB_SOURCE=./modules/simdjson/singleheader
clang++ -O3 -flto -fpic -std=c++11 -o build/simdjson build/main.o -I${LIB_INCLUDE} ${LIB_SOURCE}/simdjson.cpp src/example_simdjson.cpp

rm build/*.o

strip build/yyjson
strip build/cjson
strip build/parson
strip build/zzzjson
strip build/simdjson
strip build/null
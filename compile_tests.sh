#!/usr/bin/env bash

mkdir -p build

clang -Os -flto -fpic -o build/null src/example_null.c src/main.c

YYJSON_DIR=../yyjson
clang -Os -flto -fpic -o build/yyjson -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1  src/main.c src/example_yyjson.c ${YYJSON_DIR}/src/yyjson.c -I${YYJSON_DIR}

CJSON_DIR=../cjson
clang -O3 -flto -fpic -o build/cjson  src/main.c src/example_cjson.c ${CJSON_DIR}/cJSON.c ${CJSON_DIR}/cJSON_Utils.c -I${CJSON_DIR}

PARSON_DIR=../parson
clang -O3 -flto -fpic -o build/parson src/main.c src/example_parson.c ${PARSON_DIR}/parson.c -I${PARSON_DIR}

ZZZJSON_DIR=../zzzjson
clang -O3 -flto -fpic -o build/zzzjson src/main.c src/example_zzzjson.c -I${ZZZJSON_DIR}

strip build/yyjson
strip build/cjson
strip build/parson
strip build/zzzjson
strip build/null
#!/usr/bin/env bash

set -e

mkdir -p build

(cd modules/memprofile && ./compile_clang.sh)

clang -O3 -flto -fpic -c -o build/main.o src/main.c

clang -Os -flto -fpic -o build/null src/example_null.c build/main.o

LIB_INCLUDE=./modules/yyjson
LIB_SOURCE=./modules/yyjson
clang -Os -flto -fpic -o build/yyjson -DYYJSON_DISABLE_WRITER -DYYJSON_DISABLE_FAST_FP_CONV=1  build/main.o src/example_yyjson.c ${LIB_SOURCE}/src/yyjson.c -I${LIB_INCLUDE}

LIB_INCLUDE=./modules/cjson
LIB_SOURCE=./modules/cjson
clang -O3 -flto -fpic -o build/cjson  build/main.o src/example_cjson.c ${LIB_SOURCE}/cJSON.c ${LIB_SOURCE}/cJSON_Utils.c -I${LIB_INCLUDE}

LIB_INCLUDE=./modules/parson
LIB_SOURCE=./modules/parson
clang -O3 -flto -fpic -o build/parson build/main.o src/example_parson.c ${LIB_SOURCE}/parson.c -I${LIB_INCLUDE}

LIB_INCLUDE=./modules/zzzjson
LIB_SOURCE=
clang -O3 -flto -fpic -o build/zzzjson build/main.o src/example_zzzjson.c -I${LIB_INCLUDE}

LIB_INCLUDE=./modules/simdjson/singleheader
LIB_SOURCE=./modules/simdjson/singleheader
clang++ -O3 -flto -fpic -std=c++11 -o build/simdjson build/main.o -I${LIB_INCLUDE} ${LIB_SOURCE}/simdjson.cpp src/example_simdjson.cpp

LIB_INCLUDE=./modules/centijson
LIB_SOURCE=./modules/centijson
clang -Os -flto -fpic -o build/centijson build/main.o src/example_centijson.c ${LIB_SOURCE}/src/json.c -I${LIB_INCLUDE}/src

LIB_INCLUDE=./modules/jaxon
LIB_SOURCE=./modules/jaxon
clang -Os -flto -fpic -o build/jaxon build/main.o src/example_jaxon.c ${LIB_SOURCE}/c_src/decoder.c -I${LIB_INCLUDE}/c_src

LIB_INCLUDE=./modules/libjson
LIB_SOURCE=./modules/libjson
clang -Os -flto -fpic -o build/libjson build/main.o src/example_libjson.c ${LIB_SOURCE}/json.c -I${LIB_INCLUDE}

LIB_INCLUDE=./modules/pdjson
LIB_SOURCE=./modules/pdjson
clang -Os -flto -fpic -o build/pdjson build/main.o src/example_pdjson.c ${LIB_SOURCE}/pdjson.c -I${LIB_INCLUDE}

rm build/*.o

strip build/yyjson
strip build/cjson
strip build/parson
strip build/zzzjson
strip build/simdjson
strip build/null
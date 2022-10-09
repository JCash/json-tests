#

## Setup + Build

Install submodules and then build:

    ./update_submodules.sh
    ./compile_tests.sh

## Run tests

    ./run_tests.py
    ./generate_images.sh

## Results

[parse](./reports/parse.json)
[traverse](./reports/traverse.json)
[memory](./reports/memory.json)
[allocations](./reports/allocations.json)


<img src="generated/parse.png" alt="time parsing json files" width="350">
<img src="generated/traverse.png" alt="time traversing json files" width="350">

<img src="generated/memory.png" alt="amount of memory used" width="350">
<img src="generated/allocations.png" alt="number of allocations" width="350">


# Links

* https://github.com/ibireme/yyjson
* https://github.com/DaveGamble/cJSON
* https://github.com/dacez/zzzjson
* https://github.com/kgabis/parson
* https://github.com/simdjson/simdjson

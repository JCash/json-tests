#!/usr/bin/env bash

./render_bar_chart.py reports/parse.json generated/parse.png
./render_bar_chart.py reports/traverse.json generated/traverse.png
./render_bar_chart.py reports/allocations.json generated/allocations.png
./render_bar_chart.py reports/memory.json generated/memory.png
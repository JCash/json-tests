#!/usr/bin/env python3

import os, sys, json
import matplotlib.pyplot as plt
import numpy as np

def parse_json(path):
    with open(path, 'r') as f:
        data = json.loads(f.read())
    return data

# {
#     "title": "Parson .json files",
#     "ylabel": "Time (s)",
#     "yscale": "log", # linear if omitted
#     "xlabels": [
#         "1.json (364b)",
#         "2.json (945kb)",
#         "3.json (1720kb)",
#         "4.json (3980kb)"
#     ],
#     "xvalues": {
#         "yyjson": [1.3e-05, 0.000822, 0.001469, 0.017246],
#         "cjson": [1.7e-05, 0.005614, 0.00924, 0.14927],
#         "parson": [5e-05, 0.016044, 0.025027, 0.25136],
#         "zzzjson": [1.2e-05, 0.002121, 0.003584, 0.032257],
#         "simdjson": [0.000105, 0.001687, 0.002455, 0.022555]
#     }
# }

def render_bar_data(data, outpath):

    xlabels = data['xlabels']
    xvalues = data['xvalues']

    x = np.arange(len(xlabels))  # the label locations

    num_bars = len(xvalues)//2
    width = (1.0 - 0.2) / num_bars
    max_width = width * num_bars
    half_max_width = max_width * 0.5

    fig, ax = plt.subplots()

    ax.set_yscale(data.get('yscale', 'linear'))

    for i in range(0, len(xvalues)//2):
        name = xvalues[i*2+0]
        values = xvalues[i*2+1]
        print("values", values, len(values))
        print(name, values)
        ax.bar(x - half_max_width + i*width, values, width, label=name)

    hlines = data.get('hlines', None)
    if hlines is not None:
        ax.hlines(hlines, xmin=x-half_max_width-width*0.5, xmax=x+half_max_width+width*0.5, color='w', linestyle = 'dotted')

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel(data['ylabel'])
    ax.set_title(data['title'])
    ax.set_xticks(x, xlabels)
    ax.legend()

    fig.tight_layout()

    plt.savefig(outpath)
    print("Wrote", outpath)


if __name__ == '__main__':
    path = None
    if len(sys.argv) > 1:
        path = sys.argv[1]
    if not path:
        print("no .json file supplied")
        sys.exit(1)

    outpath = None
    if len(sys.argv) > 2:
        outpath = sys.argv[2]
    if not outpath:
        outpath = path + ".png"

    data = parse_json(path)
    render_bar_data(data, outpath)

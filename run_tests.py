#!/usr/bin/env python3
import os, re, sys, json, subprocess
from typing import OrderedDict
from collections import OrderedDict

ALL_TESTS=['yyjson', 'simdjson', 'zzzjson', 'cjson', 'parson']
JSON_FILES=['assets/1.json', 'assets/2.json', 'assets/3.json', 'assets/4.json']
JSON_SIZES={}
REPORTS={}

MEMPROF=os.environ.get('MEMPROF','./modules/memprofile/libmemprofile.dylib')

class RunException(Exception):
    pass

def run(args, **kwargs):
    process = subprocess.Popen(args, stdout = subprocess.PIPE, stderr = subprocess.STDOUT, **kwargs)
    output = process.communicate()[0]
    if process.returncode != 0:
        print("Failed command:\n" + ' '.join(args), file=sys.stderr)
        raise RunException(output)
    return str(output, encoding='utf-8').strip()

def env_run(env, args, **kwargs):
    return run(args, env = env, **kwargs)

def get_file_size(path):
    s = os.stat(path)
    return s.st_size

def parse_output(output):
    p_times = re.compile(r".*Size:\s*(.*)\s*Parse:\s*(.*)s\s*Stringify:\s*(.*)s\s*Traverse:\s*(.*)s")
    p_allocations = re.compile(r".*?Total:.*?([0-9]+).*?([0-9]+).*")
    t_parse = 0.0
    t_stringify = 0.0

    for line in output.split('\n'):
        line = line.strip()

        m = p_times.match(line)
        if m:
            # Size: print(m.group(1))
            t_parse = float(m.group(2))
            t_stringify = float(m.group(3))
            t_traverse = float(m.group(4))

        m = p_allocations.match(line)
        if m:
            # Size: print(m.group(1))
            n_memory = int(m.group(1))
            n_allocations = int(m.group(2))
    return t_parse, t_stringify, t_traverse, n_memory, n_allocations

def create_report(libname, json_path, json_size, output):
    report = {}
    report['name'] = libname
    report['path'] = json_path
    report['size'] = json_size
    t_parse, t_stringify, t_traverse, n_memory, n_allocations = parse_output(output)
    report['t_parse'] = t_parse
    report['t_stringify'] = t_stringify
    report['t_traverse'] = t_traverse
    report['n_memory'] = n_memory
    report['n_allocations'] = n_allocations
    return report

def run_test(libname):
    print("")
    print("*********************************************************")
    print(libname)
    print("*********************************************************")

    exe = './build/%s' % libname

    report = {}
    report['name'] = libname
    report['tests'] = []
    report['exesize'] = get_file_size(exe)
    REPORTS[libname] = report

    env = dict(os.environ)
    env['DYLD_INSERT_LIBRARIES'] = MEMPROF
    for json_path, json_size in JSON_SIZES.items():
        args=[]
        args.append('./build/' + libname)
        args.append(json_path)
        output = env_run(env, args)
        print(output)
        test_report = create_report(libname, json_path, json_size, output)
        report['tests'].append(test_report)


def get_json_sizes(sizes):
    for json_path in JSON_FILES:
        sizes[json_path] = get_file_size(json_path)
    return sizes


def run_all_tests():
    get_json_sizes(JSON_SIZES)
    for test in ALL_TESTS:
        run_test(test)

def get_values_from_reports(reports, key):
    out = OrderedDict()
    for libname, report in reports.items():
        values = []
        for testdata in report['tests']:
            values.append(testdata[key])
        out[libname] = values
    return out

def print_reports():
    print("*********************************************************")
    print("Reports")
    print("*********************************************************")
    print("name, exesize")

    null_size = get_file_size('./build/null')
    print(','.join(['null', str(null_size)]))
    for name, r in REPORTS.items():
        print(','.join([r['name'], str(r['exesize'])]))

    reports = {}
    for json_path in JSON_FILES:
        reports[json_path] = []

    for _, r in REPORTS.items():
        for t in r['tests']:
            reports[t['path']].append(t)

    print("*********************************************************")
    print("json, lib, parse, stringify, traverse, memory, allocations")
    for path, r in reports.items():
        for t in r:
            print(','.join([path, t['name'],
                            str(t['t_parse']), str(t['t_stringify']), str(t['t_traverse']),
                            str(t['n_memory']), str(t['n_allocations'])]))

    def create_report(key_name):
        data = OrderedDict()

        xlabels = []
        for file in JSON_FILES:
            size = JSON_SIZES[file]
            if size < 1024:
                sz = '%db' % size
            else:
                sz = '%dkb' % (size//1024)
            lbl = '%s\n(%s)' % (file, sz)
            xlabels.append(lbl)
        data["xlabels"] = xlabels

        xvalues = []
        allvalues = get_values_from_reports(REPORTS, key_name)
        for libname, values in allvalues.items():
            xvalues.append(libname)
            xvalues.append(values)

        data["xvalues"] = xvalues
        return data

    def write_report(name, data):
        outpath = 'reports/%s.json' % name
        with open(outpath, 'w') as f:
            s = json.dumps(data, indent = 2)
            f.write(s)
            #print(s)
        print("Wrote", outpath)

    print("*********************************************************")

    data = create_report('t_parse')
    data["title"] = "Parsing .json files"
    data["ylabel"] = "Time (s)"
    data["yscale"] = "log"
    write_report('parse', data)

    data = create_report('t_traverse')
    data["title"] = "Traversing DOM"
    data["ylabel"] = "Time (s)"
    write_report('traverse', data)

    data = create_report('n_allocations')
    data["title"] = "Number of allocations"
    data["ylabel"] = "Count"
    data["yscale"] = "log"
    write_report('allocations', data)

    data = create_report('n_memory')
    data["title"] = "Amount of memory"
    data["ylabel"] = "kilobytes"
    newxvalues = []
    for v in data['xvalues']:
        if isinstance(v, list):
            newxvalues.append([x // 1024 for x in v])
        else:
            newxvalues.append(v)
    data['xvalues'] = newxvalues
    write_report('memory', data)

# {
#     "title": "Parson .json files",
#     "ylabel": "Time (s)",
#     "xlabels": [
#         "1.json (364b)",
#         "2.json (945kb)",
#         "3.json (1720kb)",
#         "4.json (3980kb)"
#     ],
#     "xvalues": [
#         "yyjson", [1.3e-05, 0.000822, 0.001469, 0.017246],
#         "simdjson", [0.000105, 0.001687, 0.002455, 0.022555],
#         "zzzjson", [1.2e-05, 0.002121, 0.003584, 0.032257],
#         "cjson", [1.7e-05, 0.005614, 0.00924, 0.14927],
#         "parson", [5e-05, 0.016044, 0.025027, 0.25136]
#     ]
# }

if __name__ == "__main__":
    if len(sys.argv) > 1:
        ALL_TESTS = [sys.argv[1]]

    run_all_tests()
    print_reports()
    print("")

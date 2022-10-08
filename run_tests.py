#!/usr/bin/env python3
import os, re, sys, subprocess

ALL_TESTS=['yyjson', 'cjson', 'parson', 'zzzjson']
JSON_FILES=['assets/1.json', 'assets/2.json', 'assets/3.json', 'assets/4.json']
JSON_SIZES={}
REPORTS={}

MEMPROF=os.environ.get('MEMPROF','./libmemprofile.dylib')

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

def create_report(libname, json, json_size, output):
    report = {}
    report['name'] = libname
    report['path'] = json
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
    for json, json_size in JSON_SIZES.items():
        args=[]
        args.append('./build/' + libname)
        args.append(json)
        output = env_run(env, args)
        print(output)
        test_report = create_report(libname, json, json_size, output)
        report['tests'].append(test_report)


def get_json_sizes(sizes):
    for json in JSON_FILES:
        sizes[json] = get_file_size(json)
    return sizes


def run_all_tests():
    get_json_sizes(JSON_SIZES)
    for test in ALL_TESTS:
        run_test(test)

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
    for json in JSON_FILES:
        reports[json] = []

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

if __name__ == "__main__":
    if len(sys.argv) > 1:
        ALL_TESTS = [sys.argv[1]]

    run_all_tests()
    print_reports()
    print("")

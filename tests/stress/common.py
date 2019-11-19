import os
import subprocess
import re
import time

def run_cmd(cmd_list):
    process = subprocess.Popen(cmd_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=1, universal_newlines=True)

    print("Running command: {}".format(subprocess.list2cmdline(process.args)))

    stdout, stderr = process.communicate()
    rc = process.wait()
    
    return rc, stdout, stderr


def get_data_path():
    return os.path.join(os.path.dirname(os.path.abspath(__file__)), 'data/')


def get_files(ends_with):
    path = get_data_path()
    list_of_files = []

    for (dirpath, dirnames, filenames) in os.walk(path):
        for filename in filenames:
            if filename.endswith(ends_with):
                list_of_files.append(os.sep.join([dirpath, filename]))
                
    return list_of_files


def remove_previous():
    for dirpath, _, files in os.walk('.'):
        for f in files:
            _, ext = os.path.splitext(f)
            if ext == ".testdb" or ext == ".tmp": 
                os.remove(os.path.join(dirpath, f)) 


def get_test_data(endswith):
    filename_re = re.compile(r'.*history_(?P<number>\d+).*')
    test_data = {}
    files = get_files(endswith)
    
    for f in files:
        res = re.match(filename_re, f)
        if res:
            number = int(res.group('number'))
            test_data[number] = f
    
    return test_data


def run_test(command, key):
    print('Run test for {} entries'.format(key))
    start = time.time()
    rc, stdout, _ = run_cmd(command)
    end = time.time()
    result = (end - start)
    print('{}: total {} sec / per entry {} sec'.format(key, result, result/key))
   
    return result, stdout


def save_results(filename, results):
    with open(os.path.join(get_data_path(), filename), 'w') as f:
        for key in sorted(results.keys()):
            print('{}: total {} sec / per entry {} sec'.format(key, results[key], results[key]/key), file=f)
    
    
def gen_test_data(numbers):
    
    for number in numbers:
        filename = os.path.join(get_data_path(), 'history_{}.tmp'.format(number))

        with open(filename, 'w') as f:
            for i in range(number):
                print('1!!hmseparator!!2000-01-01 12:00:00!!hmseparator!!/some/fixed/path!!hmseparator!!command number {}!!hmseparator!!0'.format(i), file=f)


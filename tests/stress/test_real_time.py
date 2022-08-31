from common import *

def db_insertion():
    print('DB insertion tests')
    test_data = get_test_data('.tmp')
    
    results = {}
    for key in sorted(test_data.keys()):
        hm_history_db = get_data_path() + 'history_{}.testdb'.format(key)
        cmd = ['hm-db', hm_history_db, 'parse', test_data[key], '!!hmseparator!!']
        results[key], stdout = run_test(cmd, key)
    
    save_results('db_insertion.out', results)


def select_by_path():
    print('Selection by path tests')
    test_data = get_test_data('.testdb')
    
    results = {}
    for key in sorted(test_data.keys()):
        hm_history_db = test_data[key]
        cmd = ['hm-db', hm_history_db, 'select', '1', '/some/fixed/path']
        results[key], stdout = run_test(cmd, key)
        
        assert len(stdout.strip().split('\n')) == key
    
    save_results('select_by_path.out', results)


def select_by_session_and_path():
    print('Selection by session and path tests')
    test_data = get_test_data('.testdb')
    
    results = {}
    for key in sorted(test_data.keys()):
        hm_history_db = test_data[key]
        cmd = ['hm-db', hm_history_db, 'select', '1', '/some/fixed/path', '-s']
        results[key], stdout = run_test(cmd, key)

        assert len(stdout.strip().split('\n')) == key
    
    save_results('select_by_session_and_path.out', results)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('-n','--numbers', nargs='+', required=True)
    flags = parser.parse_args()
    numbers = [int(x) for x in flags.numbers]
    
    # Remove all test databases
    remove_previous()
    
    # Generate text files with given number of commands
    gen_test_data(numbers)
    
    # Run db_insertion tests first
    db_insertion()
    
    # Now we have prepared databases - run tests on them
    select_by_path()
    select_by_session_and_path()
    
    

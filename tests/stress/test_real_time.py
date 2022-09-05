from common import *

def db_insertion(numbers):
    print('DB insertion tests with add')
    
    for number in numbers:
        hm_history_db = get_data_path() + 'history_{}.testdb'.format(number)
        
        start = time.time()
        for i in range(number):
            cmd = ['hm-db', hm_history_db, 'add', "1", "\"2000-01-01 12:00:00\"", "/some/fixed/path", f"command number {i}", "0"]
            rc, stdout, _ = run_cmd(cmd)

        end = time.time()
        result = (end - start)
        print('{}: total {} sec / per entry {} sec'.format(number, result, result/number))
    

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
    
    # Run db_insertion tests first
    db_insertion(numbers)
    
    # Now we have prepared databases - run tests on them
    select_by_path()
    select_by_session_and_path()
    
    

import subprocess, sys


RED = "\033[31m";
GREEN = "\033[32m";
RESET = "\033[0m";

def double_free(filename: str, expected_double_frees: int):
    out = subprocess.run(f"c++ -g3 -I ../includes ./src/tester.cpp ./error/{filename} -L.. -lft_malloc -Wl,-rpath,.. && ./a.out && /bin/rm a.out", shell=True, capture_output=True)
    print(out.stdout.decode())
    err = out.stderr.decode()
    print(err)
    c = err.count("free(): double free detected")
    if c != expected_double_frees:
        print(f"{RED}{filename} error: expected {expected_double_frees}, received {c}{RESET}\n")
        sys.exit(1)
    print(f"{GREEN}{filename} success{RESET}\n")

double_free("double_free_1.cpp", 1)
double_free("double_free_2.cpp", 2)

sys.exit(0)

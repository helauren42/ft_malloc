import os
import time
import subprocess, sys
from pathlib import Path

RED = "\033[31m";
GREEN = "\033[32m";
RESET = "\033[0m";

LD_LIBRARY_PATH=".."
LD_PRELOAD="libft_malloc.so"

def exec(cmd: str, custom: bool, path: str, valgrind: bool, killtime: int)-> str:
    preload = f"LD_LIBRARY_PATH={LD_LIBRARY_PATH} LD_PRELOAD={LD_PRELOAD}" if custom else ""
    filepath = os.path.join(os.getcwd(), LD_LIBRARY_PATH, LD_PRELOAD)
    if Path(filepath).is_symlink():
        print("libft_malloc.so path exists")
    else:
        print("libft_malloc.so path does not exist")
        sys.exit(1)
    killstr = f"(sleep {killtime} && pkill -f valgrind) &" if killtime > 0 else ""
    valstr = f"valgrind --leak-check=full  --show-leak-kinds=all" if valgrind else ""
    process = subprocess.run([f"{killstr} make && {preload} {valstr} ./{cmd}"], shell=True, capture_output=True, cwd=Path.joinpath(Path.cwd(), "./system/", path).resolve())
    if valgrind == False:
        return  process.stdout.decode()
    return process.stderr.decode()

def valtest(og: str, custom: str):
    og_lines = og.splitlines()
    custom_lines = custom.splitlines()
    found = False
    for i in range(len(og_lines)):
        if "HEAP SUMMARY:" in og_lines[i]:
            found = True
            og_lines = og_lines[i:]
            break
    if found == False:
        print("Valgrind failed to run program")
        print("OG_LINES: ", og_lines)
        print("CUSTOM_LINES: " ,custom_lines)
    for i in range(len(custom_lines)):
        if "HEAP SUMMARY:" in custom_lines[i]:
            custom_lines = custom_lines[i:]
            break
    custom_lines = [x[x.find("== ")+3:].strip() for x in custom_lines]
    og_lines = [x[x.find("== ")+3:].strip() for x in og_lines]
    for i in range(len(custom_lines)):
        if custom_lines[i] != og_lines[i]:
            print(f"{RED}Error: valgrind output does not match{RESET}\n")
            print("OG: ", og_lines[i])
            print("CUSTOM: ", custom_lines[i])
            sys.exit(1)

def valtests():
    cmd='./AMAZING'
    path="./AICu/"
    killtime=5
    valtest(exec(cmd, False, path, True, killtime), exec(cmd, True, path, True, killtime))
    print(f"{GREEN}{path} Success{RESET}")

def nonValtests():
    cmd='./philo 5 800 200 200 7'
    path="./philo/"
    print("Check that the philosophers output is valid:")
    time.sleep(2)
    print(exec(cmd, True, path, False, 0))
    time.sleep(8)

def main():
    valtests()
    nonValtests()
    sys.exit(0)

main()

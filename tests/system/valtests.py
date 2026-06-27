import os
import subprocess, sys
from pathlib import Path

RED = "\033[31m";
GREEN = "\033[32m";
RESET = "\033[0m";

def exec(cmd: str, custom: bool, path: str):
    lib_path = Path.joinpath(Path.cwd(), "../libft_malloc.so").resolve()
    preload = f"LD_PRELOAD={lib_path}" if custom else ""
    stderr = subprocess.run([f"(sleep 5 && pkill -f valgrind) & make && {preload} valgrind --leak-check=full  --show-leak-kinds=all ./{cmd}"], shell=True, capture_output=True, cwd=Path.joinpath(Path.cwd(), "./system/", path).resolve()).stderr
    return stderr.decode()

def test(og: str, custom: str):
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

def run(cmd: str, path: str):
    test(exec(cmd, False, path), exec(cmd, True, path))
    print(f"{GREEN}{path} Success{RESET}")

def main():
    run(cmd='./AMAZING', path="./AICu/")
    sys.exit(0)

main()

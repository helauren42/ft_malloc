import sys
from typing import Literal

Mode_Type = Literal["PRE", "POST"]
MODE: Mode_Type = "PRE"

Function_Call_Type = Literal["malloc(", "realloc(", "free("]
FUNCTION_CALLS: list[Function_Call_Type] = ["malloc(", "realloc(", "free("]

FILE_PATHS = ["./includes/ft_malloc.h", "./src/malloc.c", "./src/realloc.c", "./src/free.c"]

# returns tuple of function called and its index
def hasFunctionCall(line: str, index: int)-> tuple[int, Function_Call_Type] | None:
    for function_call in FUNCTION_CALLS:
        if MODE == "POST":
            function_call = "ft_" + function_call
        print("Looking for: ", function_call)
        index = line.find(function_call, index)
        if index >= 0:
            print("found function_call: ", line)
            return (index, function_call) # pyright: ignore
    return None

def isPrepended(line: str, index: int)-> bool:
    if line[index-3:index] == "ft_" or line[index:index+3] == "ft_":
        return True
    return False

def prepend_ft(index: int, line: str):
    if index >= 0 and isPrepended(line, index) == False:
        return line[0:index] + "ft_" + line[index:]
    return line

def remove_ft(index: int, line: str):
    if index >= 0 and line[index: index+3] == "ft_":
        return line[0:index] + line[index+3:]
    return line

def process_line(line: str)-> str:
    index = 0
    while (has := hasFunctionCall(line, index)):
        index, function_call = has
        if MODE == "PRE":
            line = prepend_ft(index, line)
            index += len(function_call) + 3
        if MODE == "POST":
            line = remove_ft(index, line)
            index += len(function_call) - 3
    return line

def main(mode: Mode_Type):
    global MODE
    MODE = mode
    for file_path in FILE_PATHS:
        lines = []
        with open(file_path, "r") as f:
            lines = f.readlines()
            for i in range(len(lines)):
                lines[i] = process_line(lines[i])
        with open(file_path, "w") as f:
            f.writelines(lines)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("wrong args")
        exit(1)
    mode = sys.argv[1]
    if mode != "PRE" and mode != "POST":
        print("invalid mode")
        exit(1)
    main(mode)

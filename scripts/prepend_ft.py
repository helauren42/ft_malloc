import sys
from typing import Literal

MODE: Literal["PRE", "POST"] = "PRE"
FILE_PATHS = ["../includes/ft_malloc.h", "../src/malloc.c"]
PRE_RESULT = "malloc("
POST_RESULT = "ft_malloc("

FUNCTION_CALLS = ["malloc(", "realloc(", "free("]

Function_Call_Type = Literal["malloc(", "realloc(", "free("]

# returns tuple of function called and its index
def hasFunctionCall(line: str)-> tuple[int, Function_Call_Type] | None:
    for function_call in FUNCTION_CALLS:
        index = line.find(function_call)
        if index >= 0:
            return (index, function_call) # pyright: ignore
    return None

# prepend ft_ to function call
def process_line(line: str)-> str:
    has = hasFunctionCall(line)
    if has is None:
        return line
    index, function_call = has
    if index >= 0 and POST_RESULT not in line:
        return line[0:index] + "ft_" + line[index:]
    return line

def main():
    if len(sys.argv) > 1: # assume it's post
        MODE = "POST"
    for file_path in FILE_PATHS:
        with open(file_path, "r") as f:
            lines = f.readlines()
            for i in range(len(lines)):
                line = lines[i]
                line = process_line(line)

import difflib

RED = "\033[31m"
GREEN = "\033[32m"
RESET = "\033[0m"

def exctractBlocksSize(lines: list[str])-> list[int]:
    found = []
    for line in lines:
        if line.startswith("Zone") or line.startswith("\n"):
            continue
        start = line.find(":", 6)
        if start == -1:
            continue
        s = line[start+2:].strip()
        num = int(s)
        found.append(num)
    return found

def matchBlocksSize(blocksSizes: list[int], expected: list[int]):
    blocksSizes.sort()
    expected.sort()
    if blocksSizes != expected:
        print(f"{RED}\n-------- Error in blocks")
        it = difflib.ndiff([str(x) for x in expected], [str(x) for x in blocksSizes])
        print("\n".join(it))
        print("- EXPECTED:", expected)
        print("- RECEIVED:", blocksSizes)
    else:
        print(f"{GREEN}success: {blocksSizes}{RESET}")

def processBlocks(out: str, expectedBlocks: tuple[list[int], list[int], list[int]]):
    lines = out.splitlines(keepends=True)
    i = 0
    print("!!",lines)
    while lines[i] != "------------ TINY HEAP ------------\n":
        i += 1
    tiny_heap_start = i+1
    while lines[i] != "------------ SMALL HEAP ------------\n":
        i += 1
    small_heap_start = i+1
    while lines[i] != "------------ LARGE HEAP ------------\n":
        i += 1
    large_heap_start = i+1
    while lines[i] != "------------ HEAP END ------------\n":
        i += 1
    heap_end = i
    print("--- Expected blocks: ", expectedBlocks)
    print("- TINY: ", end="")
    matchBlocksSize(exctractBlocksSize(lines[tiny_heap_start:small_heap_start]), expectedBlocks[0])
    print("- SMALL: ", end="")
    matchBlocksSize(exctractBlocksSize(lines[small_heap_start:large_heap_start]), expectedBlocks[1])
    print("- LARGE: ", end="")
    matchBlocksSize(exctractBlocksSize(lines[large_heap_start:heap_end]), expectedBlocks[2])

def matchOutput(out: str, expectedBlocks: tuple[list[int], list[int], list[int]], err: str, expectedErr: str):
    processBlocks(out, expectedBlocks)
    if err != expectedErr:
        print(f"{RED}\n-------- STDERR mismatch:")
        print('\n'.join(
            difflib.ndiff(
                expectedErr.splitlines(),
                err.splitlines()
            )
        ) + RESET)


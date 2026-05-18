import os
import subprocess

from calculateExpectedBlocks import calculateExpectedBlocks
from utils import CC_RUN_FILE
from matchOutput import matchOutput

VALID_TESTS = ["test0_tiny_small.c", "test1_structs.c", "test2_free.c"]

def getTestFiles(dirName: str):
    l = os.listdir(dirName)
    l.sort()
    ret = []
    for filename in l:
        if not filename.endswith(".c"):
            continue
        ret.append(dirName + "/" + filename)
    return ret

def runTests(tests: list[str]):
    for filename in tests:
        caught = subprocess.run([CC_RUN_FILE(filename)], shell=True, capture_output=True, cwd="../")
        err = caught.stderr.decode()
        out = caught.stdout.decode()
        expectedBlocks = calculateExpectedBlocks(filename)
        print("\n")
        print("TEST: ", filename)
        matchOutput(out, expectedBlocks, err, "")

runTests(getTestFiles("valid"))
# runTests(getTestFiles("invalid"))

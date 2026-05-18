import subprocess

from calculateExpectedBlocks import calculateExpectedBlocks
from utils import CC_RUN_FILE
from matchOutput import matchOutput

BASIC_FILES = ["test0.c"]

for filename in BASIC_FILES:
    caught = subprocess.run([CC_RUN_FILE(filename)], shell=True, capture_output=True, cwd="../")
    err = caught.stderr.decode()
    out = caught.stdout.decode()
    expectedBlocks = calculateExpectedBlocks(filename)
    matchOutput(out, expectedBlocks, err, "")

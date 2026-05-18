import utils
from parseAllocAmount import allocAmount

allocs: dict[str,int] = {}

def handleFree(line: str):
    start: int = line.find("free(")
    if start < 0:
        return
    end: int = line.find(");", start)
    if end < 0:
        raise Exception("Free parsing issue")
    varName = line[start+5:end]
    allocs.pop(varName)

def getAllocs(filename: str):
    global allocs
    utils.FILENAME = filename
    allocs = {}
    def getVarName(line: str)-> str:
        pos = line.find("=")
        return line[:pos].strip().split(" ")[-1].strip(" *")

    path = f"./src/{filename}"
    with open(path, "r") as f:
        lines = f.readlines()
        utils.C_FILE_CONTENT = lines
        for line in lines:
            mallocStart = line.find("malloc(")
            if mallocStart < 0:
                handleFree(line)
                continue
            mallocEnd = line.find(");", mallocStart)
            if mallocEnd < 0:
                raise Exception("Malloc parsing issue")
            amount = allocAmount(line[mallocStart+7:mallocEnd])
            varName = getVarName(line)
            allocs[varName] = amount

def zoneMaxPayloads():
    path = "../includes/ft_malloc.h"
    tinyMax = 0
    smallMax = 0
    with open(path, "r") as f:
        lines = f.readlines()
        for line in lines:
            if line.find("#define TINY_MAX_PAYLOAD") >= 0:
                tinyMax = int(line.split(" ")[2])
            if line.find("#define SMALL_MAX_PAYLOAD") >= 0:
                smallMax = int(line.split(" ")[2])
    return (tinyMax, smallMax)

def sortAllocs(zoneMax: tuple[int, int])->tuple[list[int], list[int], list[int]]:
    global allocs
    tinyHeap = []
    smallHeap = []
    largeHeap = []
    for _, alloc in allocs.items():
        if alloc <= zoneMax[0]:
            tinyHeap.append(alloc)
        elif alloc <= zoneMax[1]:
            smallHeap.append(alloc)
        else:
            largeHeap.append(alloc)
    return (tinyHeap, smallHeap, largeHeap)

def calculateExpectedBlocks(filename: str):
    zoneMax = zoneMaxPayloads()
    getAllocs(filename)
    expectedBlocks = sortAllocs(zoneMax)
    return expectedBlocks

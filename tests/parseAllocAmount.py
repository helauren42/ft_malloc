import re

from structSize import getStructSize

OPPERATORS = "*+-()"

def retrieveSize(element: str) -> int:
    element = element.strip()

    if element.startswith("t_"):
        return getStructSize(element)
    elif "*" in element:
        return 8
    else:
        match element:
            case "char":
                return 1
            case "short":
                return 2
            case "int":
                return 4
            case "float":
                return 4
            case "long":
                return 8
            case "double":
                return 8
            case _: # maybe get the size of struct if struct
                raise ValueError(f"retrieveSize unknown type: {element}")

def compute(a: int, b: int, op: str)-> int:
    assert len(op) == 1
    match op:
        case "*":
            return a * b
        case "+":
            return a + b
        case "-":
            return a - b
        case _:
            raise ValueError(f"compute unknown operator: {op}")

def hasOperators(input: str)-> bool:
    for operator in OPPERATORS:
        if operator in input:
            return True
    return False

def allocAmount(input: str)-> int:
    global filename
    if hasOperators(input) is False:
        return int(input.strip())
    parts = input.split(" ")
    # should probably do some recusive computation to calculate the amount instead of just handling len 1 or len 3
    if len(parts) != 3:
        raise ValueError(f"Nah not handling this: {parts}")
    print(parts)
    elements = []
    for i in range(len(parts)):
        parts[i] = parts[i].strip()
        part = parts[i]
        if i % 2 and len(part) != 1 and part not in OPPERATORS:
            raise Exception("parseAllocAmount error: element should is not an operator or this operator is not handled")
        if part.startswith("sizeof("):
            elements.append(retrieveSize(part[7:-1]))
        else:
            elements.append(part)
    if len(elements) % 3 != 0:
        raise Exception(f"parseAllocAmount error: invalid number of elements")
    return compute(int(elements[0]), int(elements[2]), elements[1])

if __name__ == "__main__":
    print("test 5: ", allocAmount("5"))
    print("test 'sizeof(int) * 5': ", allocAmount("sizeof(int) * 5"))


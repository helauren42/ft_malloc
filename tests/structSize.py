import utils
import subprocess

STRUCT_SIZES: dict[str, int] = {}

def getAllStructs()-> list[str]:
    start = f"typedef struct s_"
    definition: list[str] = []
    inStructDef = False
    print(1)
    print(utils.C_FILE_CONTENT)
    for line in utils.C_FILE_CONTENT:
        print(line)
        if line.find(start) >= 0:
            inStructDef = True
        if inStructDef:
            definition.append(line)
        if line.find("}") >= 0 and line.find(";") >= 0:
            inStructDef = False
    if len(definition) == 0:
        raise ValueError("Struct not found in file")
    return definition

def createCFile(structDef: list[str], structName: str):
    with open("structSize.c", "w") as f:
        f.write("#include <stdio.h>\n")
        f.writelines(structDef)
        f.write(
            "int main() {" + 'printf("%zu", sizeof(' + structName + ")); return 0;}"
        )

def getStructSize(structName: str)-> int:
    t_size = STRUCT_SIZES.get(structName)
    if t_size:
        return t_size
    definition = getAllStructs()
    createCFile(definition, structName)
    subprocess.run(["cc", "structSize.c", "-o", "structSizeBin"], check=True)
    output = subprocess.run(["./structSizeBin"], capture_output=True, check=True)
    subprocess.run(["/bin/rm", "structSize.c", "structSizeBin"], check=True)
    out = output.stdout.decode().strip()
    if output.stderr or out.isnumeric() == False:
        raise Exception("Error running c file to get struct size")
    # print(f"STRUCT SIZE FOUND for '{structName}': {int(out)}")
    return int(out)

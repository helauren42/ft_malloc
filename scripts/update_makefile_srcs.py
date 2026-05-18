import os
import re

def update_makefile():
    src_dir = 'src'
    makefile_path = 'Makefile'
    
    c_files = []
    for root, _, files in os.walk(src_dir):
        for file in files:
            if file.endswith('.c'):
                rel_path = os.path.join(root, file)
                c_files.append(rel_path)
    
    c_files.sort()
    srcs_list = ' '.join(c_files)
    
    if not os.path.exists(makefile_path):
        print(f"Error: {makefile_path} not found.")
        return

    with open(makefile_path, 'r') as f:
        content = f.read()

    pattern = r'^(SRCS\s*[:?]?=).*$'
    
    new_line = f'SRCS := {srcs_list}'
    
    new_content, count = re.subn(pattern, new_line, content, flags=re.MULTILINE)
    
    if count == 0:
        pattern_src = r'^(SRC\s*[:?]?=).*$'
        new_line_src = f'SRC := {srcs_list}'
        new_content, count = re.subn(pattern_src, new_line_src, content, flags=re.MULTILINE)

    if count > 0:
        with open(makefile_path, 'w') as f:
            f.write(new_content)
        print(f"Found {len(c_files)} source files.")
    else:
        print("Error: Could not find SRCS or SRC variable in Makefile.")

if __name__ == "__main__":
    update_makefile()

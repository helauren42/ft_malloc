C_FILE_CONTENT: list[str] = []
FILENAME: str = ""


def CC_RUN_FILE(filename: str)-> str:
    return f"cc -g3 -I ./includes ./tests/src/{filename} -L. -lft_malloc -Wl,-rpath,. && ./a.out && /bin/rm a.out"


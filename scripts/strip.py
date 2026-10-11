#!/usr/bin/env python
import sys
import re


allowed_directives = [
    "align",
    "ascii",
    "asciiz",
    "byte",
    "data",
    "double",
    "end_macro",
    "eqv",
    # "extern",
    "float",
    "globl",
    "half",
    "include",
    "kdata",
    "ktext",
    "macro",
    # "set",
    "space",
    "text",
    "space",
    "word",
]
whitespace_regex = re.compile(r"\s")


def transform(line: str) -> str:
    if len(line.strip()) == 0:
        return ""
    elif line.strip()[0] != ".":
        return line
    elif line.replace("\t", " ").strip().split(" ")[0].split(".")[-1].strip().split(" ")[0] in allowed_directives:
        return line
    return ""


def strip(filepath: str) -> str:
    with open(filepath, "r") as file:
        for line in file:
            yield transform(line)


def write(filepath: str) -> None:
    output = ""
    for line in strip(filepath):
        if len(line.strip()) == 0:
            continue
        output += line
    # print(output)
    with open(filepath, "w") as file:
        file.write(output)


def main(argv: list[str]) -> None:
    for filepath in argv:
        if len(filepath) == 0:
            continue
        elif filepath.split(".")[-1] != "asm":
            continue
        write(filepath)


main(sys.argv)

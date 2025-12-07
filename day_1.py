#!/usr/bin/env python3

import requests

"""AOC 2025 Day 1"""


def problem1(inp: str) -> int:
    ret = 0
    pos = 50
    for ang in map(
        lambda s: (-1 if s[0] == "L" else 1) * int(s[1:]), filter(None, inp.split("\n"))
    ):
        pos = (pos + ang) % 100
        ret += pos == 0
    return ret


def problem2(inp: str) -> int:
    ret = 0
    pos = 50
    for ang in map(
        lambda s: (-1 if s[0] == "L" else 1) * int(s[1:]), filter(None, inp.split("\n"))
    ):
        zc = (pos > 0) and (pos + ang <= 0)
        pos += ang
        ret += zc + abs(pos) // 100
        pos %= 100
    return ret


SESSION_ID = "<your session id>"


def main():
    resp = requests.get(
        f"https://adventofcode.com/2025/day/1/input", cookies={"session": SESSION_ID}
    )
    inp = resp.content.decode()
    print(problem1(inp))
    print(problem2(inp))


if __name__ == "__main__":
    main()

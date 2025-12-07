# AOC2025

Solutions for Advent of Code puzzles 2025 — implemented in C++.  

## 📁 Project Structure

Each `day_X.h` corresponds to a solution for a day's puzzle. `main.cpp` serves as the entry point.  

## 🔧 Build & Run

Prerequisites:

* C++ compiler with C++20 or later support
* CMake
* libcurl

Build:

```bash
git clone https://github.com/CaptainJK/AOC2025.git
cd AOC2025
cmake -Bbuild .
cmake --build build
```

Run:
```bash
export SESSION=<session cookie (copy from browser)>

# Solve all days
./build/aoc $SESSION
```

Or you can pass list of days (solve only days 1,5 and 7):
```bash
./build/aoc $SESSION 1 5 7
```

## 🧑‍💻 How to Use / Add Solutions

Create a new `day_X.h` for a new day’s puzzle. Implement `Day` interface.
Include the header into `main.cpp` and append the created class to the tuple template parameter list:

```cpp
constexpr std::tuple<Day1, Day2, Day3, ..., DayX> puzzles;
```

## 📝 Why This Repo

This repo is a personal workspace for solving Advent of Code 2025 puzzles.

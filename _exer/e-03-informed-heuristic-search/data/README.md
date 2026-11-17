# Data — Puzzle Files and Runners

This folder contains input puzzles, result files, and batch runner scripts for the N-Sliding Blocks Puzzle exercise.

## Contents

| Path | Description |
|---|---|
| [`puzzles/`](puzzles/) | 162 input puzzles (2×2 through 10×10, including five hard 4×4) |
| [`results-cpp/`](results-cpp/) | One `.txt` result per puzzle — C++ implementation |
| [`results-java/`](results-java/) | One `.txt` result per puzzle — Java implementation |
| [`results-python/`](results-python/) | One `.txt` result per puzzle — Python implementation |
| [`ANALYSIS.md`](ANALYSIS.md) | Full results table and performance analysis |
| [`SOLUTION.md`](SOLUTION.md) | Algorithm explanation, optimisations, move sequences for all 5 hard puzzles |
| [`run_cpp.py`](run_cpp.py) | Batch runner — C++ |
| [`run_java.py`](run_java.py) | Batch runner — Java |
| [`run_python.py`](run_python.py) | Batch runner — Python |

---

## Running the batch runners

All three runners are Python 3 scripts. They build/compile the target language if needed, run every puzzle with a 300 s timeout, and skip puzzles whose result file already exists.

### Prerequisites

| Language | Requirement |
|---|---|
| C++ | `clang++` with C++17 support (macOS: Xcode Command Line Tools; Windows: LLVM or MinGW) |
| Java | JDK 11+ (`javac` and `java` on PATH) |
| Python | Python 3.8+ |

### macOS / Linux

Run from the repository root or from this `data/` folder — both work because the scripts resolve paths relative to their own location.

```bash
# From the e-03 exercise directory:
python3 data/run_cpp.py
python3 data/run_java.py
python3 data/run_python.py

# Or from inside data/:
cd data
python3 run_cpp.py
python3 run_java.py
python3 run_python.py
```

### Windows (Command Prompt)

```bat
cd _exer\e-03-informed-heuristic-search\data

python run_cpp.py
python run_java.py
python run_python.py
```

> On Windows the C++ runner calls `clang++`. If you have MSVC instead, edit `run_cpp.py` and change `clang++` to `cl` and adjust the flags (`/O2 /std:c++17 /Fe:...`).

### Windows (PowerShell)

```powershell
cd _exer\e-03-informed-heuristic-search\data

python run_cpp.py
python run_java.py
python run_python.py
```

---

## Running a single puzzle

```bash
# C++ (from cpp/SlidingBlocksPuzzleProceduralSolution/)
clang++ -O2 -std=c++17 -o n_sliding_puzzle NSlidingPuzzle.cpp
./n_sliding_puzzle ../../data/puzzles/081-puzzle-4x4.txt

# Java (from java/SlidingBlocksPuzzleProceduralSolution/)
javac -d out src/*.java
java -cp out NSlidingPuzzle ../../data/puzzles/081-puzzle-4x4.txt

# Python (from the e-03 exercise directory)
python3 python/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle.py \
    data/puzzles/081-puzzle-4x4.txt
```

On Windows replace `./n_sliding_puzzle` with `n_sliding_puzzle.exe` and use `\` path separators where needed.

---

## Result file format

Each `.txt` result file contains the solver output exactly as printed to stdout:

```
<number of moves>
<move 1>
...
<move N>

Success!
[IDA* Sliding Blocks Puzzle] HH:MM:SS.mmm
```

For C++ the elapsed line uses the format `Elapsed: <time>` (sub-ms precision for short runs).  
For unsolvable puzzles the file contains only `Puzzle cannot be solved!`.

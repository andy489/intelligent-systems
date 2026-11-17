#!/usr/bin/env python3
"""Run all sliding-block puzzles with the C++ binary and write results to data/results-cpp/."""

import subprocess
import sys
from pathlib import Path

DATA_DIR   = Path(__file__).parent
ROOT_DIR   = DATA_DIR.parent
BINARY     = ROOT_DIR / "cpp/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle"
PUZZLES    = DATA_DIR / "puzzles"
RESULTS    = DATA_DIR / "results-cpp"
TIMEOUT    = 300  # seconds per puzzle


def build():
    src = ROOT_DIR / "cpp/SlidingBlocksPuzzleProceduralSolution"
    print("Building C++ binary...")
    subprocess.run(
        ["clang++", "-O2", "-std=c++17", "-o", str(BINARY), "NSlidingPuzzle.cpp"],
        cwd=src, check=True
    )
    print("Build OK")


def run_puzzle(puzzle: Path) -> str:
    proc = subprocess.Popen(
        [str(BINARY), str(puzzle)],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    try:
        out, _ = proc.communicate(timeout=TIMEOUT)
        return out.decode(errors="replace")
    except subprocess.TimeoutExpired:
        proc.kill()
        return f"TIMED OUT (>{TIMEOUT}s)\n"


def main():
    if not BINARY.exists():
        build()

    RESULTS.mkdir(parents=True, exist_ok=True)

    puzzles = sorted(PUZZLES.glob("*.txt"))
    total = solved = unsolvable = skipped = timed_out = 0

    for puzzle in puzzles:
        name   = puzzle.stem
        result = RESULTS / (name + ".txt")
        total += 1

        if result.exists():
            print(f"  [skip] {name}")
            skipped += 1
            continue

        print(f"  [run ] {name} ... ", end="", flush=True)
        output = run_puzzle(puzzle)
        result.write_text(output)

        lines = output.strip().splitlines()
        if "TIMED OUT" in output:
            print(f"TIMED OUT (>{TIMEOUT}s)")
            timed_out += 1
        elif "cannot be solved" in output:
            print("unsolvable")
            unsolvable += 1
        else:
            moves = lines[0] if lines else "?"
            if moves == "0":
                print(f"solved  moves=0  time=—")
            else:
                elapsed = next((l.replace("Elapsed: ", "") for l in lines if "Elapsed:" in l), "?")
                print(f"solved  moves={moves}  time={elapsed}")
            solved += 1

    print()
    print("─" * 50)
    print(f"Total:      {total}")
    print(f"Solved:     {solved}")
    print(f"Unsolvable: {unsolvable}")
    print(f"Timed out:  {timed_out}")
    print(f"Skipped:    {skipped}")
    print("─" * 50)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Run all sliding-block puzzles with the Java implementation and write results to data/results-java/."""

import subprocess
import sys
from pathlib import Path

DATA_DIR   = Path(__file__).parent
ROOT_DIR   = DATA_DIR.parent
JAVA_SRC   = ROOT_DIR / "java/SlidingBlocksPuzzleProceduralSolution"
JAVA_OUT   = JAVA_SRC / "out"
PUZZLES    = DATA_DIR / "puzzles"
RESULTS    = DATA_DIR / "results-java"
TIMEOUT    = 2400  # seconds per puzzle (40 min)


def build():
    print("Compiling Java sources...")
    JAVA_OUT.mkdir(parents=True, exist_ok=True)
    srcs = list((JAVA_SRC / "src").glob("*.java"))
    subprocess.run(
        ["javac", "-d", str(JAVA_OUT)] + [str(s) for s in srcs],
        check=True
    )
    print("Compile OK")


def run_puzzle(puzzle: Path) -> str:
    proc = subprocess.Popen(
        ["java", "-cp", str(JAVA_OUT), "NSlidingPuzzle", str(puzzle)],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    try:
        out, _ = proc.communicate(timeout=TIMEOUT)
        return out.decode(errors="replace")
    except subprocess.TimeoutExpired:
        proc.kill()
        return f"TIMED OUT (>{TIMEOUT}s)\n"


def main():
    if not (JAVA_OUT / "NSlidingPuzzle.class").exists():
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
                elapsed = next((l for l in lines if l.startswith("[IDA*")), "?")
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

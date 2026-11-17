#!/usr/bin/env python3
"""Run all sliding-block puzzles with the Java solver and write one result file per puzzle."""

import subprocess
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
JAR        = SCRIPT_DIR / "java/SlidingBlocksPuzzleProceduralSolution/target/SlidingBlocksPuzzleProceduralSolution-1.0.jar"
PUZZLES    = SCRIPT_DIR / "data/puzzles"
RESULTS    = SCRIPT_DIR / "data/results-java"
TIMEOUT    = 300  # seconds per puzzle

def build():
    src = SCRIPT_DIR / "java/SlidingBlocksPuzzleProceduralSolution"
    print("Building jar...")
    subprocess.run(["mvn", "package", "-q"], cwd=src, check=True)
    print("Build OK")

def run_puzzle(puzzle: Path) -> str:
    proc = subprocess.Popen(
        ["java", "-jar", str(JAR), str(puzzle)],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    try:
        out, _ = proc.communicate(timeout=TIMEOUT)
        return out.decode(errors="replace")
    except subprocess.TimeoutExpired:
        proc.kill()
        return f"TIMED OUT (>{TIMEOUT}s)\n"

def main():
    if not JAR.exists():
        build()

    RESULTS.mkdir(parents=True, exist_ok=True)

    puzzles = sorted(PUZZLES.glob("*.txt"))
    total = solved = unsolvable = skipped = timed_out = 0

    for puzzle in puzzles:
        name   = puzzle.stem
        result = RESULTS / (name + ".txt")
        total += 1

        if result.exists():
            print(f"  [skip] {name}  (result exists)")
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
            moves   = lines[0] if lines else "?"
            elapsed = next((l.replace("[IDA* Sliding Blocks Puzzle] ", "") for l in lines if "IDA*" in l), "?")
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

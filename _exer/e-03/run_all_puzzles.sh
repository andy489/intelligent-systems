#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BINARY="$SCRIPT_DIR/cpp/SlidingBlocksPuzzleProceduralSolution/n_sliding_puzzle"
PUZZLES_DIR="$SCRIPT_DIR/data/puzzles"
RESULTS_DIR="$SCRIPT_DIR/data/results"
TIMEOUT_SEC=300  # 5 minutes per puzzle

if [[ ! -x "$BINARY" ]]; then
    echo "Binary not found. Building..."
    cd "$SCRIPT_DIR/cpp/SlidingBlocksPuzzleProceduralSolution"
    clang++ -O2 -std=c++17 -o n_sliding_puzzle NSlidingPuzzle.cpp
    echo "Build OK"
    cd "$SCRIPT_DIR"
fi

mkdir -p "$RESULTS_DIR"

total=0
solved=0
unsolvable=0
skipped=0
timed_out=0

for puzzle in "$PUZZLES_DIR"/puzzle*.txt; do
    name="$(basename "$puzzle" .txt)"
    result="$RESULTS_DIR/$name.txt"

    total=$((total + 1))

    if [[ -f "$result" ]]; then
        echo "  [skip] $name  (result exists)"
        skipped=$((skipped + 1))
        continue
    fi

    echo -n "  [run ] $name ... "

    output="$(python3 -c "
import subprocess, sys
proc = subprocess.Popen(
    ['$BINARY', '$puzzle'],
    stdout=subprocess.PIPE, stderr=subprocess.STDOUT
)
try:
    out, _ = proc.communicate(timeout=$TIMEOUT_SEC)
    sys.stdout.buffer.write(out)
except subprocess.TimeoutExpired:
    proc.kill()
    sys.stdout.write('TIMED OUT\n')
" 2>&1)"

    echo "$output" > "$result"

    if echo "$output" | grep -q "TIMED OUT"; then
        echo "TIMED OUT (>${TIMEOUT_SEC}s)"
        timed_out=$((timed_out + 1))
    elif echo "$output" | grep -q "cannot be solved"; then
        echo "unsolvable"
        unsolvable=$((unsolvable + 1))
    else
        moves=$(echo "$output" | head -1)
        elapsed=$(echo "$output" | grep "Elapsed:" | sed 's/Elapsed: //')
        echo "solved  moves=$moves  time=$elapsed"
        solved=$((solved + 1))
    fi
done

echo ""
echo "────────────────────────────────────────────────"
echo "Total:      $total"
echo "Solved:     $solved"
echo "Unsolvable: $unsolvable"
echo "Timed out:  $timed_out"
echo "Skipped:    $skipped"
echo "────────────────────────────────────────────────"

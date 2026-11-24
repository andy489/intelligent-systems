#!/usr/bin/env bash
# Usage: ./run.sh <dataset> <K>
#   dataset: normal | unbalance   (or a full path to a .txt file)
#   K:       number of clusters
# Always uses Dunn index + K-Means++ (the defaults when called with 2 args).
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$SCRIPT_DIR/.."
BINARY="$SCRIPT_DIR/k_means"
PLOT="$ROOT/python/plot.py"

if [[ ! -x "$BINARY" ]]; then
    echo "Building k_means..."
    clang++ -std=c++17 -O2 -o "$BINARY" "$SCRIPT_DIR/k_means.cpp"
fi

DATASET="${1:-normal}"
K="${2:-3}"

if [[ -f "$DATASET" ]]; then
    DATA_PATH="$DATASET"
elif [[ -f "$ROOT/datasets/$DATASET/$DATASET.txt" ]]; then
    DATA_PATH="$ROOT/datasets/$DATASET/$DATASET.txt"
else
    echo "Dataset not found: $DATASET"
    exit 1
fi

BASENAME="$(basename "$DATA_PATH" .txt)"
RESULT_CSV="$ROOT/output/result.csv"
OUTPUT_PNG="$ROOT/output/${BASENAME}_k${K}.png"
TITLE="K-Means++ | $BASENAME | K=$K | Dunn index"

"$BINARY" "$DATA_PATH" "$K" "$RESULT_CSV"
python3 "$PLOT" "$RESULT_CSV" "$TITLE" "$OUTPUT_PNG"
echo "Plot saved: $OUTPUT_PNG"

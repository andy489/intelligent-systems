# Decision Tree — Play Tennis

ID3 decision tree classifier on the classic Play Tennis dataset (14 instances, 4 attributes).

## Build & Run

**C++**
```bash
clang++ -std=c++17 -O2 -o cpp/decision_tree cpp/decision_tree.cpp
./cpp/decision_tree data/play_tennis.csv
```

**Java**
```bash
javac java/src/DecisionTree.java -d java/out
java -cp java/out DecisionTree data/play_tennis.csv
```

**Python**
```bash
python3 python/decision_tree.py data/play_tennis.csv
```

Non-interactive single prediction (all languages):
```bash
./cpp/decision_tree data/play_tennis.csv "Sunny,Hot,High,Weak"
# → No
```

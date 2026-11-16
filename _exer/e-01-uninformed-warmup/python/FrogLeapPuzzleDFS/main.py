import sys
from dfs_solution import DFSSolution

n = int(input("N: "))
sys.setrecursionlimit(n * n + 2 * n + 100)
dfs_solution = DFSSolution(n)
dfs_solution.solve()

# Homework 21.2 — Classic Algorithm Problems

C++ algorithms solutions for fundamental algorithmic problems.

## Algorithms & Complexity

| Algorithm | Time | Space |
|---|---|---|
| Merge K Sorted Arrays | O(N log K) | O(K) heap + O(N) output |
| Inversion Count | O(n log n) | O(n) |
| LIS (Patience Sort) | O(n log n) | O(n) |
| Next Permutation | O(n) | O(1) in-place |

## Build & Run

```bash
g++ -std=c++17 -O2 -o homework21 main.cpp
./homework21
```

## Description

- Merge K sorted arrays using a min-heap for efficient multi-way merge
- Count inversions via modified merge sort (cross-inversion counting during merge step)
- Longest Increasing Subsequence with patience sorting and full sequence reconstruction
- Lexicographic next permutation generation with in-place O(n) algorithm

# C++ Game Performance Analyzer

A C++17 command-line tool that processes frame-time CSV exports and reports average FPS, 1% low FPS, frame-time percentiles, and stutter counts. Uses no third-party libraries.

## Run
```bash
cmake -S . -B build
cmake --build build
./build/game_performance_analyzer data/sample_frametimes.csv
```
On Windows, the executable may be under `build/Debug/`.

## CSV format
One `frametime_ms` column, with one positive measurement per frame. A 1% low is computed as the inverse of the mean frame time of the slowest 1% of frames. Stutters are frames taking >50 ms. The 99th percentile is computed with nearest-rank indexing.

## Portfolio note
Synthetic demonstration data; results are not real game benchmarks. GitHub Actions compiles and runs the program.

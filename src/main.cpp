#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2) { std::cerr << "Usage: game_performance_analyzer <csv>\n"; return 1; }
    std::ifstream input(argv[1]);
    if (!input) { std::cerr << "Cannot open input CSV\n"; return 1; }
    std::string line;
    if (!std::getline(input, line) || line != "frametime_ms") {
        std::cerr << "Expected CSV header: frametime_ms\n"; return 1;
    }
    std::vector<double> frames;
    try {
        while (std::getline(input, line)) {
            if (line.empty()) continue;
            size_t consumed = 0;
            double value = std::stod(line, &consumed);
            if (consumed != line.size() || !std::isfinite(value) || value <= 0)
                throw std::invalid_argument("Invalid frame time");
            frames.push_back(value);
        }
    } catch (const std::exception& e) { std::cerr << e.what() << "\n"; return 1; }
    if (frames.empty()) { std::cerr << "No frames found\n"; return 1; }
    const double total = std::accumulate(frames.begin(), frames.end(), 0.0);
    const double avgMs = total / frames.size();
    std::sort(frames.begin(), frames.end());
    const size_t slowCount = std::max<size_t>(1, static_cast<size_t>(std::ceil(frames.size() * .01)));
    const double slowTotal = std::accumulate(frames.end() - slowCount, frames.end(), 0.0);
    const size_t p99Index = static_cast<size_t>(std::ceil(.99 * frames.size())) - 1;
    const auto stutters = std::count_if(frames.begin(), frames.end(), [](double x){ return x > 50.0; });
    std::cout << std::fixed << std::setprecision(2)
              << "Frames: " << frames.size() << "\n"
              << "Average FPS: " << 1000.0 / avgMs << "\n"
              << "1% low FPS: " << 1000.0 * slowCount / slowTotal << "\n"
              << "Average frame time (ms): " << avgMs << "\n"
              << "99th percentile frame time (ms): " << frames[p99Index] << "\n"
              << "Stutters (>50 ms): " << stutters << "\n";
}

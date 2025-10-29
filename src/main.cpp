#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstdlib>

#include "KgramModel.h"
#include "Slm.h"

static std::string read_all(const std::string &filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) throw std::runtime_error("Could not open file: " + filename);
    std::ostringstream ss; ss << in.rdbuf();
    return ss.str();
}

static void print_usage() {
    std::cerr << "Usage:\n"
              << "  slm <k> <input.txt> <num_chars> [--seed <N>]\n"
              << "Examples:\n"
              << "  slm 2 data/moby_mini.txt 200\n"
              << "  slm 2 data/moby_mini.txt 200 --seed 123\n";
}

int main(int argc, char* argv[]) {
    using namespace std;

    if (argc != 4 && !(argc == 6 && std::string(argv[4]) == "--seed")) {
        print_usage();
        return 1;
    }

    size_t k = 0; size_t n = 0;
    try {
        k = static_cast<size_t>(stoul(argv[1]));
        n = static_cast<size_t>(stoul(argv[3]));
    } catch (...) {
        std::cerr << "Error: <k> and <num_chars> must be positive integers.\n";
        return 1;
    }

    try {
        auto model = std::make_unique<KgramModel>(k);
        Slm slm(std::move(model));

        if (argc == 6) {
            try {
                unsigned seed = static_cast<unsigned>(stoul(argv[5]));
                slm.seed(seed);
            } catch (...) {
                std::cerr << "Error: seed must be a non-negative integer.\n";
                return 1;
            }
        }

        const string text = read_all(argv[2]);
        slm.train(text);

        const string generated = slm.generate(n);
        cout << generated << "\n";
    } catch (const exception &e) {
        cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

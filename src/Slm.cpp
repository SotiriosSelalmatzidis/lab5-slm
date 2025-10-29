#include "Slm.h"
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <random>

Slm::Slm(std::unique_ptr<KgramModel> model)
: model_(std::move(model)), rng_(std::random_device{}()) {
    
    if (const char* s = std::getenv("SLM_SEED")) {
        try { rng_.seed(static_cast<unsigned>(std::stoul(s))); } catch (...) {}
    }
}

void Slm::train(const std::string &text) {
    model_->train(text);
}

KgramModel::Kgram Slm::sample_start_kgram() {
    std::vector<KgramModel::Kgram> ws;
    std::vector<std::size_t> counts;
    model_->kgram_counts(ws, counts);
    if (ws.empty()) throw std::runtime_error("Model has no k-grams; did you train it?");

    auto looks_like_sentence_start = [](const std::string& w) {
        if (w.empty()) return false;
        if (w.front() == ' ') return true;
        if (std::isupper(static_cast<unsigned char>(w.front()))) return true;
        for (std::size_t i = 0; i + 1 < w.size(); ++i) {
            char c = w[i];
            if (c == '.' || c == '!' || c == '?') return true;
        }
        return false;
    };

    std::vector<std::size_t> preferred_idx;
    preferred_idx.reserve(ws.size());
    for (std::size_t i = 0; i < ws.size(); ++i) {
        if (looks_like_sentence_start(ws[i])) preferred_idx.push_back(i);
    }

    if (!preferred_idx.empty()) {
        std::vector<std::size_t> preferred_counts;
        preferred_counts.reserve(preferred_idx.size());
        for (auto i : preferred_idx) preferred_counts.push_back(counts[i]);
        std::discrete_distribution<std::size_t> dist(preferred_counts.begin(), preferred_counts.end());
        return ws[preferred_idx[dist(rng_)]];
    }

    
    std::discrete_distribution<std::size_t> dist(counts.begin(), counts.end());
    return ws[dist(rng_)];
}
 


char Slm::sample_next_char(const KgramModel::Kgram &w, bool &ok) {
    std::vector<char> chars;
    std::vector<std::size_t> counts;
    model_->next_char_counts(w, chars, counts);
    if (chars.empty()) { ok = false; return '\0'; }
    std::discrete_distribution<std::size_t> dist(counts.begin(), counts.end());
    ok = true;
    return chars[dist(rng_)];
}

std::string Slm::generate(std::size_t n) {
    if (n == 0) return std::string();
    

    std::string out;
    out.reserve(n);

    KgramModel::Kgram w = sample_start_kgram();

    while (out.size() < n) {
        bool ok = false;
        char c = sample_next_char(w, ok);
        if (!ok) { // 
            w = sample_start_kgram();
            continue;
        }
        out.push_back(c);
        
        w.erase(w.begin());
        w.push_back(c);
    }
    return out;
}

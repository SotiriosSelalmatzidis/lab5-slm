#include "KgramModel.h"
#include <stdexcept>

KgramModel::KgramModel(std::size_t k) : k_(k) {
    if (k_ == 0) {
        throw std::invalid_argument("k must be >= 1");
    }
}

void KgramModel::train(const std::string &text) {
    if (text.size() < k_) return; // 

    
    for (std::size_t i = 0; i + k_ <= text.size(); ++i) {
        Kgram w = text.substr(i, k_);
        ++kfreq_[w];
        if (i + k_ < text.size()) {
            char c = text[i + k_];
            ++trans_[w][c];
        }
    }
}

std::size_t KgramModel::count_of(const Kgram &w) const {
    auto it = kfreq_.find(w);
    return (it == kfreq_.end()) ? 0u : it->second;
}

std::vector<KgramModel::Kgram> KgramModel::kgrams() const {
    std::vector<Kgram> v;
    v.reserve(kfreq_.size());
    for (const auto &kv : kfreq_) v.push_back(kv.first);
    return v;
}

void KgramModel::next_char_counts(const Kgram &w,
                                  std::vector<char> &chars,
                                  std::vector<std::size_t> &counts) const {
    chars.clear();
    counts.clear();
    auto it = trans_.find(w);
    if (it == trans_.end()) return;
    const auto &inner = it->second;
    chars.reserve(inner.size());
    counts.reserve(inner.size());
    for (const auto &kv : inner) {
        chars.push_back(kv.first);
        counts.push_back(kv.second);
    }
}

void KgramModel::kgram_counts(std::vector<Kgram> &ws,
                              std::vector<std::size_t> &counts) const {
    ws.clear();
    counts.clear();
    ws.reserve(kfreq_.size());
    counts.reserve(kfreq_.size());
    for (const auto &kv : kfreq_) {
        ws.push_back(kv.first);
        counts.push_back(kv.second);
    }
}

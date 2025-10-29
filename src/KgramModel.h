#ifndef KGRAMMODEL_H
#define KGRAMMODEL_H

#include <map>
#include <string>
#include <vector>
#include <cstddef>


class KgramModel {
public:
    using Kgram = std::string;

    explicit KgramModel(std::size_t k);

    
    void train(const std::string &text);

    
    std::size_t k() const { return k_; }

   
    std::size_t count_of(const Kgram &w) const;

    
    std::vector<Kgram> kgrams() const;

   
    void next_char_counts(const Kgram &w, std::vector<char> &chars, std::vector<std::size_t> &counts) const;

   
    void kgram_counts(std::vector<Kgram> &ws, std::vector<std::size_t> &counts) const;

private:
    std::size_t k_;  
    std::map<Kgram, std::size_t> kfreq_;  
    std::map<Kgram, std::map<char, std::size_t>> trans_; 
};

#endif // KGRAMMODEL_H

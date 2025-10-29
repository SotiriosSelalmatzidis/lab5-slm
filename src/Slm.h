#ifndef SlM_H
#define SlM_H

#include <memory>
#include <random>
#include <string>
#include "KgramModel.h"


class Slm {
public:
   
    explicit Slm(std::unique_ptr<KgramModel> model);

    
    void train(const std::string &text);

    
    std::string generate(std::size_t n);

    
    const KgramModel &model() const { return *model_; }

    
    void seed(unsigned int s) { rng_.seed(s); }

private:
    std::unique_ptr<KgramModel> model_; 
    std::mt19937 rng_; // Mersenne Twister RNG

    KgramModel::Kgram sample_start_kgram(); 
    char sample_next_char(const KgramModel::Kgram &w, bool &ok); 
};

#endif // SLM_H

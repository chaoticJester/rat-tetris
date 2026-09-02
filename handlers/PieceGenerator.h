// PieceGenerator.h
#pragma once

#include "../models/Types.h"
#include <deque>
#include <array>
#include <random>
#include <algorithm>

class PieceGenerator {
    private:
        std::deque<PieceType> queue;
        std::mt19937 rng;
        void refillBag();
    public:
        PieceGenerator();
        PieceType next();
        std::array<PieceType,5> peek5Next() const;

};  

#include "PieceGenerator.h"

PieceGenerator::PieceGenerator() {
    rng.seed(std::random_device{}());
    refillBag();
}

void PieceGenerator::refillBag() {
    std::array<PieceType, 7> bag = {PieceType::I, PieceType::O, PieceType::L, PieceType::J, PieceType::S, PieceType::Z, PieceType::T};
    std::shuffle(bag.begin(), bag.end(), this->rng);
    for(int i = 0; i < 7; i++) {
        queue.push_back(bag[i]);
    }
}

PieceType PieceGenerator::next() {
    PieceType front = this->queue.front();
    queue.pop_front();
    if(queue.size() < 7) refillBag();
    return front;
}

std::array<PieceType,5> PieceGenerator::peek5Next() const {
    std::array<PieceType,5> result;
    for(int i = 0; i < 5; i++) {
        result[i] = queue[i];
    }
    return result;
}
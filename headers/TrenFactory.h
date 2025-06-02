#ifndef TRAINFACTORY_H
#define TRAINFACTORY_H

#include <memory>

class Tren; 

class TrainFactory {
public:
    static std::shared_ptr<Tren> createTrain(int ora);
};

#endif // TRAINFACTORY_H
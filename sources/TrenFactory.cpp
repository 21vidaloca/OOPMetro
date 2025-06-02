#include "../headers/TrenFactory.h"
#include "../headers/Tren.h"
#include "../headers/TrenRapid.h"
#include "../headers/TrenElectric.h"
#include "../headers/TrenNoapte.h"
#include <memory>
std::shared_ptr<Tren> TrainFactory::createTrain(int ora) {
    if (ora >= 5 && ora < 13) {
        return std::make_shared<TrenRapid>("Tren-Rapid-Factory", 80, 25, true);
    }
    else if (ora >= 13 && ora < 23) {
        return std::make_shared<TrenElectric>("Tren-Electric-Factory", 75, 90, 480, true);
    }
    else {

        return std::make_shared<TrenNoapte>("Tren-Noapte-Factory", 70, 23, 5, 0.8);
    }
}
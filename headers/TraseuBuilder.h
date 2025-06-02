#ifndef TRASEUBUILDER_H
#define TRASEUBUILDER_H

#include <string>
#include <vector>
#include <memory>

#include "../headers/Tren.h"
#include "../headers/Statia.h"
#include "../headers/Traseu.h"

class TraseuBuilder {
private:
    std::string m_numeRuta;
    std::shared_ptr<Tren> m_tren;
    std::vector<std::shared_ptr<Statia>> m_statii;

public:

    TraseuBuilder();


    TraseuBuilder& setName(const std::string& nume);
    TraseuBuilder& assignTrain(std::shared_ptr<Tren> tren);
    TraseuBuilder& addStatia(std::shared_ptr<Statia> statie);
    TraseuBuilder& addStatiaDetails(const std::string& numeStatie, int timpAsteptare, int distantaUrmatoare = 2);


    std::shared_ptr<Traseu> build();


    void reset();
};

#endif // TRASEUBUILDER_H
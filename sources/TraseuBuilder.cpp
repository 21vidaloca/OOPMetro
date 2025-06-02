// TraseuBuilder.cpp
#include "../headers/TraseuBuilder.h"
#include "../headers/Traseu.h"
#include "../headers/Statia.h"
#include <iostream>


TraseuBuilder::TraseuBuilder() = default;

TraseuBuilder& TraseuBuilder::setName(const std::string& nume) {
    m_numeRuta = nume;
    return *this;
}

TraseuBuilder& TraseuBuilder::assignTrain(std::shared_ptr<Tren> tren) {
    m_tren = tren;
    return *this;
}

TraseuBuilder& TraseuBuilder::addStatia(std::shared_ptr<Statia> statie) {
    if (statie) {
        m_statii.push_back(statie);
    } else {
        std::cerr << "Warning [TraseuBuilder]: Incercare de a adauga o statie nula." << std::endl;
    }
    return *this;
}

TraseuBuilder& TraseuBuilder::addStatiaDetails(const std::string& numeStatie, int timpAsteptare, int distantaUrmatoare) {
    m_statii.push_back(std::make_shared<Statia>(numeStatie, timpAsteptare, distantaUrmatoare));
    return *this;
}

std::shared_ptr<Traseu> TraseuBuilder::build() {
    if (m_numeRuta.empty()) {
        std::cerr << "Warning [TraseuBuilder]: Traseul este construit fara nume." << std::endl;
    }
    if (!m_tren) {
        std::cerr << "Warning [TraseuBuilder]: Traseul '" << m_numeRuta << "' este construit fara un tren alocat." << std::endl;
    }
    if (m_statii.empty()) {
        std::cerr << "Warning [TraseuBuilder]: Traseul '" << m_numeRuta << "' este construit fara statii." << std::endl;
    }

    auto traseuProdus = std::make_shared<Traseu>(m_numeRuta, m_tren);

    for (const auto& statie : m_statii) {
        traseuProdus->adaugaStatia(statie);
    }

    return traseuProdus;
}

void TraseuBuilder::reset() {
    m_numeRuta.clear();
    m_tren = nullptr;
    m_statii.clear();
}
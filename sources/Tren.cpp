#include "../headers/Tren.h"
#include "../headers/TrenRapid.h"
#include "../headers/TrenNoapte.h"
#include "../headers/TrenElectric.h"
#include <memory>
#include <vector>


Tren::Tren(const std::string& id, int viteza)
    : id(id), viteza(viteza) {}

Tren::Tren(const Tren& other)
    : id(other.id), viteza(other.viteza) {}

Tren& Tren::operator=(const Tren& other) {
    if (this != &other) {
        id = other.id;
        viteza = other.viteza;
    }
    return *this;
}

Tren::~Tren() {}

void Tren::afisare() const {
    std::cout << "Tren ID: " << id << " | Viteza: " << viteza << " km/h";
}

double Tren::calculeazaTimpParcurgere(double distanta) const {
    // Timpul in ore
    double timpOre = distanta / viteza;
    return timpOre * 60.0; // conversie la minute
}

double Tren::calculeazaEficienta() const {
    // Implementare de baza
    return 0.5; // Eficienta standard
}

int Tren::getViteza() const { return viteza; }

std::ostream& operator<<(std::ostream& os, const Tren& tren) {
    tren.afisare();
    return os;
}

#ifndef TRENELECTRIC_H
#define TRENELECTRIC_H

#include "Tren.h"
#include <iostream>

class TrenElectric : public Tren {
private:
    double eficientaEnergetica; // procent (0-100)
    int autonomieBaterie;      // minute de functionare
    bool modEco;               // daca modul eco este activat

public:
    TrenElectric(const std::string& id, int viteza, double eficienta,
                int autonomie, bool eco = false);

    void afisare() const ;

    double calculeazaTimpParcurgere(double distanta) const ;

    double calculeazaEficienta() const ;

    // Getteri specifici
    double getEficientaEnergetica() const;
    int getAutonomieBaterie() const;
    bool getModEco() const;

    // Operator pentru afisare
    friend std::ostream& operator<<(std::ostream& os, const TrenElectric& tren);
};

#endif // TRENELECTRIC_H
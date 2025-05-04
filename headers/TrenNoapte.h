#ifndef TRENNOAPTE_H
#define TRENNOAPTE_H

#include "Tren.h"
#include <iostream>

class TrenNoapte : public Tren {
private:
    int oraStart;
    int oraStop;
    double factorViteza;

public:
    TrenNoapte(const std::string& id, int viteza, int start, int stop, double factor);

    void afisare() const;

    double calculeazaTimpParcurgere(double distanta) const ;

    double calculeazaEficienta() const ;

    // Getteri specifici
    int getOraStart() const;
    int getOraStop() const;
    double getFactorViteza() const;
};

#endif // TRENNOAPTE_H
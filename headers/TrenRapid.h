#ifndef TRENRAPID_H
#define TRENRAPID_H

#include "Tren.h"
#include <vector>
#include <string>

class TrenRapid : public Tren {
private:
    int acceleratie;
    bool oprireStatiiSelectate;
    std::vector<std::string> statiiDeOprire;

public:
    TrenRapid(const std::string& id, int viteza, int acceleratie, bool oprireSelectiva);

    // Constructor cu lista de statii de oprire
    TrenRapid(const std::string& id, int viteza, int acceleratie,
              const std::vector<std::string>& statiiPermise);

    void afisare() const ;

    double calculeazaEficienta() const ;

    double calculeazaTimpParcurgere(double distanta) const ;

    // Getteri specifici
    int getAcceleratie() const;
    bool getOprireStatiiSelectate() const;
};

#endif // TRENRAPID_H
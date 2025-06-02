#ifndef TREN_H
#define TREN_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>


class Tren {
protected:
    std::string id;
    int viteza; // in km/h

public:
    Tren(const std::string& id, int viteza);

    // Constructor de copiere
    Tren(const Tren& other);

    // Operator de atribuire
    Tren& operator=(const Tren& other);

    // Destructor virtual pentru a permite polimorfismul
    virtual ~Tren();

    // Functie virtuala pentru afisare
    virtual void afisare() const;

    // Functie virtuala pentru calculul timpului de parcurgere
    virtual double calculeazaTimpParcurgere(double distanta) const;

    // Functie virtuala pentru calculul eficientei
    virtual double calculeazaEficienta() const;

    // Getteri
    int getViteza() const;
    const std::string& getNume() const;

    // Operator pentru afisare
    friend std::ostream& operator<<(std::ostream& os, const Tren& tren);
};

#endif // TREN_H
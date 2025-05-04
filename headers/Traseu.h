#ifndef TRASEU_H
#define TRASEU_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

#include "Tren.h"    // Assuming Tren.h is in the same directory
#include "Statia.h"  // Assuming Statia.h is in the same directory

class Traseu {
private:
    std::string numeRuta;
    std::shared_ptr<Tren> tren;
    std::vector<std::shared_ptr<Statia>> statii;

public:
    // Constructor
    explicit Traseu(const std::string& numeRuta, std::shared_ptr<Tren> tren);

    // Constructor de copiere
    Traseu(const Traseu& other);

    // Operator de atribuire
    Traseu& operator=(const Traseu& other);

    // Destructor (not strictly necessary here, but good practice if you might add resources later)
    ~Traseu() = default;

    // Metoda pentru afisare
    void afisare() const;

    // Adauga o statie la traseu
    void adaugaStatia(std::shared_ptr<Statia> statie);

    // Calculeaza distanta totala a traseului
    double calculeazaDistantaTotala() const;

    // Calculeaza ruta intre doua statii din acest traseu
    bool calculeazaRutaIntreStatii(const std::string& start, const std::string& destinatie,
                                    double& timpCalatorie, std::vector<std::string>& rutaStatii,
                                    const std::shared_ptr<Tren>& trenDisponibil) const;

    // Verifica daca traseul contine o anumita statie
    bool contineStatia(const std::string& numeStatie) const;

    // Getteri
    std::shared_ptr<Tren> getTren() const;
    const std::vector<std::shared_ptr<Statia>>& getStatii() const;
    const std::string& getNumeRuta() const;

    friend std::ostream& operator<<(std::ostream& os, const Traseu& traseu);
};

#endif // TRASEU_H
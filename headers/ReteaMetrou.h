#ifndef RETEAMETROU_H
#define RETEAMETROU_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>
#include <limits>

#include "../headers/Traseu.h"

class ReteaMetrou {
private:
    std::string numeRetea;
    std::vector<std::shared_ptr<Traseu>> trasee;
    std::unordered_map<std::string, std::shared_ptr<Statia>> statii;

    struct Nod {
        std::string numeStatie;
        double distanta;
        std::vector<std::string> ruta;

        Nod(const std::string& nume, double dist, const std::vector<std::string>& r)
            : numeStatie(nume), distanta(dist), ruta(r) {}

        bool operator>(const Nod& other) const {
            return distanta > other.distanta;
        }
    };

public:
    ReteaMetrou(const std::string& numeRetea);

    void adaugaTraseu(std::shared_ptr<Traseu> traseu);

    void afisareTrasee() const;

    bool existaStatieInRetea(const std::string& numeStatie) const;

    std::shared_ptr<Tren> obtineTrenDisponibil(int ora) const;

    bool calculeazaRutaOptima(const std::string& start, const std::string& destinatie,
                            double& timpTotal, std::vector<std::string>& rutaCompleta, int ora) const;

    void afisareStatisticiTrenuri() const;

    int numarTotalStatii() const;

    double lungimeTotalaRetea() const;

    std::string statiaAglomerata() const;
};

#endif // RETEAMETROU_H
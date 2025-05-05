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

class ReteaMetrouException : public std::exception {
public:
    explicit ReteaMetrouException(const char* message) : message_(message) {}
    virtual const char* what() const noexcept override { return message_; }

protected:
    const char* message_;
};

class TraseuLimitExceededException : public ReteaMetrouException {
public:
    TraseuLimitExceededException() : ReteaMetrouException("Eroare: Numarul maxim de trasee a fost atins.") {}
};

class StatieNotFoundException : public ReteaMetrouException {
public:
    StatieNotFoundException(const std::string& numeStatie)
        : ReteaMetrouException(("Eroare: Statia " + numeStatie + " nu exista in retea.").c_str()) {}
};

class RutaNotFoundException : public ReteaMetrouException {
public:
    RutaNotFoundException(const std::string& start, const std::string& destinatie, int ora)
        : ReteaMetrouException(("Eroare: Nu s-a putut calcula o ruta intre " + start + " si " + destinatie + " la ora " + std::to_string(ora) + ".").c_str()) {}
};

class ReteaMetrou {
private:
    std::string numeRetea;
    std::vector<std::shared_ptr<Traseu>> trasee;
    std::unordered_map<std::string, std::shared_ptr<Statia>> statii;
    static const int MAX_TRASEE = 10;

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
    void adaugaTraseu(std::string numeTraseu, std::shared_ptr<Tren> tren);

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
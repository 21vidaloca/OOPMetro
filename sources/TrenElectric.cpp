#include "../headers/TrenElectric.h"

TrenElectric::TrenElectric(const std::string& id, int viteza, double eficienta,
            int autonomie, bool eco)
    : Tren(id, viteza), eficientaEnergetica(eficienta),
      autonomieBaterie(autonomie), modEco(eco) {}

void TrenElectric::afisare() const  {
    Tren::afisare();
    std::cout << " | Tip: Electric | Eficienta energetica: " << eficientaEnergetica
              << "% | Autonomie: " << autonomieBaterie
              << " min | Mod Eco: " << (modEco ? "Activat" : "Dezactivat");
}

double TrenElectric::calculeazaTimpParcurgere(double distanta) const  {
    double vitezaEfectiva = getViteza();

    // In modul eco, viteza este redusa pentru economie de energie
    if (modEco) {
        vitezaEfectiva *= 0.85; // reducere cu 15%
    }

    double timpOre = distanta / vitezaEfectiva;
    return timpOre * 60.0;
}

double TrenElectric::calculeazaEficienta() const  {
    // Pentru trenul electric, eficienta este determinata de eficienta energetica
    // si de faptul ca este sau nu in modul eco
    double eficientaModificare = modEco ? 1.2 : 1.0; // Modul eco creste eficienta cu 20%
    return eficientaEnergetica * eficientaModificare / 100.0;
}

double TrenElectric::getEficientaEnergetica() const { return eficientaEnergetica; }

int TrenElectric::getAutonomieBaterie() const { return autonomieBaterie; }

bool TrenElectric::getModEco() const { return modEco; }

std::ostream& operator<<(std::ostream& os, const TrenElectric& tren) {
    tren.afisare();
    return os;
}
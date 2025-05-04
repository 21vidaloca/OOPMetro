#include "../headers/TrenNoapte.h"

TrenNoapte::TrenNoapte(const std::string& id, int viteza, int start, int stop, double factor)
    : Tren(id, viteza), oraStart(start), oraStop(stop), factorViteza(factor) {}

void TrenNoapte::afisare() const {
    Tren::afisare();
    std::cout << " | Tip: Nocturn | Program: " << oraStart << ":00 - " << oraStop
              << ":00 | Factor viteza: " << factorViteza;
}

double TrenNoapte::calculeazaTimpParcurgere(double distanta) const  {
    // In timpul noptii, viteza este redusa conform factorului
    double timpOre = distanta / (getViteza() * factorViteza);
    return timpOre * 60.0;
}

double TrenNoapte::calculeazaEficienta() const {
    // Pentru trenul de noapte, eficienta este legata de factorul de viteza
    // si de faptul ca opereaza in ore cu mai putini pasageri
    // Un factor de viteza mai mic (tren mai lent) este considerat mai eficient
    double eficientaViteza = 0.8 + (1.0 - factorViteza) * 0.5;

    // Ore de noapte sunt in general mai eficiente din punct de vedere al incarcarii
    double eficientaProgram = 0.9; // 90% eficienta datorita programului de noapte

    return eficientaViteza * eficientaProgram;
}

int TrenNoapte::getOraStart() const { return oraStart; }

int TrenNoapte::getOraStop() const { return oraStop; }

double TrenNoapte::getFactorViteza() const { return factorViteza; }
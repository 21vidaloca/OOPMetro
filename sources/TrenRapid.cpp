#include "../headers/TrenRapid.h"

TrenRapid::TrenRapid(const std::string& id, int viteza, int acceleratie, bool oprireSelectiva)
    : Tren(id, viteza), acceleratie(acceleratie), oprireStatiiSelectate(oprireSelectiva) {}

TrenRapid::TrenRapid(const std::string& id, int viteza, int acceleratie,
          const std::vector<std::string>& statiiPermise)
    : Tren(id, viteza), acceleratie(acceleratie),
      oprireStatiiSelectate(true), statiiDeOprire(statiiPermise) {}

void TrenRapid::afisare() const  {
    Tren::afisare();
    std::cout << " | Tip: Rapid | Acceleratie: " << acceleratie
              << " | Oprire selectiva: " << (oprireStatiiSelectate ? "Da" : "Nu");

    if (oprireStatiiSelectate && !statiiDeOprire.empty()) {
        std::cout << " | Statii de oprire: ";
        for (size_t i = 0; i < statiiDeOprire.size(); i++) {
            std::cout << statiiDeOprire[i];
            if (i < statiiDeOprire.size() - 1) std::cout << ", ";
        }
    }
}

double TrenRapid::calculeazaEficienta() const  {
    // Pentru trenul rapid, eficienta este influentata de acceleratie
    // si de cate opriri face

    // Acceleratie mai mare reduce eficienta
    double eficientaAcceleratie = 1.0 - (acceleratie / 200.0); // max 50% penalizare

    // Opriri selective cresc eficienta
    double eficientaOpriri = oprireStatiiSelectate ?
                            (1.0 + (0.05 * (statiiDeOprire.empty() ? 5 : statiiDeOprire.size()))) :
                            1.0;

    return eficientaAcceleratie * eficientaOpriri;
}

double TrenRapid::calculeazaTimpParcurgere(double distanta) const {
    // Viteza efectiva creste cu X% din cauza acceleratiei
    double factorAcceleratie = 1.0 - (acceleratie / 100.0); // ex: acceleratie 20 => 0.8x timpul
    double timpOre = distanta / (getViteza() * (1.0 / factorAcceleratie));
    return timpOre * 60.0; // conversie la minute
}

int TrenRapid::getAcceleratie() const { return acceleratie; }

bool TrenRapid::getOprireStatiiSelectate() const { return oprireStatiiSelectate; }
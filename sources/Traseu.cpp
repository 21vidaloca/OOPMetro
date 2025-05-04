#include "../headers/Traseu.h"

Traseu::Traseu(const std::string& numeRuta, std::shared_ptr<Tren> tren)
    : numeRuta(numeRuta), tren(tren) {}

Traseu::Traseu(const Traseu& other)
    : numeRuta(other.numeRuta), tren(other.tren), statii(other.statii) {}

Traseu& Traseu::operator=(const Traseu& other) {
    if (this != &other) {
        numeRuta = other.numeRuta;
        tren = other.tren;
        statii = other.statii;
    }
    return *this;
}

void Traseu::afisare() const {
        std::cout << "Traseu: " << numeRuta << "\n";
        std::cout << "Tren: ";
        tren->afisare();
        std::cout << "\nStatii:\n";
        for (size_t i = 0; i < statii.size(); i++) {
            std::cout << i+1 << ". ";
            statii[i]->afisare();
            std::cout << "\n";
        }
    }

void Traseu::adaugaStatia(std::shared_ptr<Statia> statie) {
    statii.push_back(statie);
}

double Traseu::calculeazaDistantaTotala() const {
    double distantaTotala = 0;
    for (size_t i = 0; i < statii.size(); i++) {
        distantaTotala += statii[i]->getDistantaUrmatoareStatie();
    }
    return distantaTotala;
}

bool Traseu::calculeazaRutaIntreStatii(const std::string& start, const std::string& destinatie,
                                    double& timpCalatorie, std::vector<std::string>& rutaStatii,
                                    const std::shared_ptr<Tren>& trenDisponibil) const {
    int indexStart = -1;
    int indexDestinatie = -1;

    for (size_t i = 0; i < statii.size(); i++) {
        if (statii[i]->getNume() == start) {
            indexStart = i;
        }
        if (statii[i]->getNume() == destinatie) {
            indexDestinatie = i;
        }

        if (indexStart != -1 && indexDestinatie != -1) {
            break;
        }
    }

    if (indexStart == -1 || indexDestinatie == -1) {
        std::cout << "Eroare: Una dintre statii nu exista pe acest traseu." << std::endl;
        return false;
    }

    bool directie = indexStart < indexDestinatie;
    int startIdx, endIdx;

    if (directie) {
        startIdx = indexStart;
        endIdx = indexDestinatie;
    } else {
        startIdx = indexDestinatie;
        endIdx = indexStart;
    }

    double distanta = 0;
    for (int i = startIdx; i < endIdx; i++) {
        distanta += statii[i]->getDistantaUrmatoareStatie();
    }

    timpCalatorie = trenDisponibil->calculeazaTimpParcurgere(distanta);

    for (int i = startIdx; i <= endIdx; i++) {
        rutaStatii.push_back(statii[i]->getNume());

        if (i < endIdx) {
            timpCalatorie += statii[i]->getTimpAsteptare() / 60.0; // conversie la minute
        }
    }

    return true;
}

bool Traseu::contineStatia(const std::string& numeStatie) const {
    for (const auto& statie : statii) {
        if (statie->getNume() == numeStatie) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Tren> Traseu::getTren() const {
    return tren;
}

const std::vector<std::shared_ptr<Statia>>& Traseu::getStatii() const {
    return statii;
}

const std::string& Traseu::getNumeRuta() const {
    return numeRuta;
}

std::ostream& operator<<(std::ostream& os, const Traseu& traseu) {
    traseu.afisare();
    return os;
}
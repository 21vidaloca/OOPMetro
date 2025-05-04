#include "../headers/ReteaMetrou.h"
#include "../headers/TrenRapid.h"
#include "../headers/TrenNoapte.h"
#include "../headers/TrenElectric.h"

#include<algorithm>
ReteaMetrou::ReteaMetrou(const std::string& numeRetea)
    : numeRetea(numeRetea) {}

void ReteaMetrou::adaugaTraseu(std::shared_ptr<Traseu> traseu) {
    trasee.push_back(traseu);
    // Update the station map
    for (const auto& statie : traseu->getStatii()) {
        statii[statie->getNume()] = statie;
    }
}

void ReteaMetrou::afisareTrasee() const {
    std::cout << "RETEAUA DE METROU: " << numeRetea << "\n";
    std::cout << "Numar trasee: " << trasee.size() << "\n\n";

    for (const auto& traseu : trasee) {
        std::cout << *traseu << "\n";
    }
}


bool ReteaMetrou::existaStatieInRetea(const std::string& numeStatie) const {
    for (const auto& traseu : trasee) {
        if (traseu->contineStatia(numeStatie)) {
            return true;
        }
    }
    return false;
}
std::shared_ptr<Tren> ReteaMetrou::obtineTrenDisponibil(int ora) const {
    // Selectăm tipul de tren în funcție de ora din zi
    if (ora >= 5 && ora < 13) {
        return std::make_shared<TrenRapid>("Tren-Rapid", 80, 25, true); // 05:00-13:00
    }
    else if (ora >= 13 && ora < 23) {
        return std::make_shared<TrenElectric>("Tren-Electric", 75, 90, 480, true); // 13:00-23:00
    }
    else {
        return std::make_shared<TrenNoapte>("Tren-Noapte", 70, 23, 5, 0.8); // 23:00-05:00
    }
}

bool ReteaMetrou::calculeazaRutaOptima(const std::string& start, const std::string& destinatie,
                                    double& timpTotal, std::vector<std::string>& rutaCompleta,
                                    int ora) const {
    if (!existaStatieInRetea(start) || !existaStatieInRetea(destinatie)) {
            std::cout << "Eroare: Una dintre statii nu exista in retea." << std::endl;
            return false;
        }

        // Obținem trenul disponibil la ora specificată
        std::shared_ptr<Tren> trenDisponibil = obtineTrenDisponibil(ora);

        // Cazul simplu: ambele statii sunt pe acelasi traseu
        for (const auto& traseu : trasee) {
            if (traseu->contineStatia(start) && traseu->contineStatia(destinatie)) {
                std::vector<std::string> rutaStatii;
                double timp = 0;

                // Calculăm ruta folosind trenul disponibil (nu trenul traseu->getTren())
                // Ar trebui să modificăm funcția calculeazaRutaIntreStatii pentru a accepta un parametru tren
                bool success = traseu->calculeazaRutaIntreStatii(start, destinatie, timp, rutaStatii, trenDisponibil);
                if (success) {
                    rutaCompleta = rutaStatii;
                    timpTotal = timp;
                    return true;
                }
            }
        }

        // Cazul complex: statiile sunt pe trasee diferite
        // Construim graful cu trenul disponibil
        std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> graf;

        for (const auto& traseu : trasee) {
            const std::vector<std::shared_ptr<Statia>>& statii = traseu->getStatii();

            for (size_t i = 0; i < statii.size(); i++) {
                const std::string& statieActuala = statii[i]->getNume();

                if (i > 0) {
                    std::string statiePrecedenta = statii[i-1]->getNume();
                    double distanta = statii[i-1]->getDistantaUrmatoareStatie();
                    double timp = trenDisponibil->calculeazaTimpParcurgere(distanta) + statii[i-1]->getTimpAsteptare() / 60.0;

                    graf[statieActuala].push_back({statiePrecedenta, timp});
                    graf[statiePrecedenta].push_back({statieActuala, timp});
                }
            }
        }

        // Dacă graful este gol, înseamnă că nu există trenuri disponibile la ora specificată
        if (graf.empty()) {
            std::cout << "Eroare: Nu există trenuri disponibile la ora " << ora << std::endl;
            return false;
        }

        std::unordered_map<std::string, double> distante;
        std::unordered_map<std::string, bool> vizitat;

        for (const auto& traseu : trasee) {
            const std::vector<std::shared_ptr<Statia>>& statii = traseu->getStatii();
            for (const auto& statie : statii) {
                distante[statie->getNume()] = std::numeric_limits<double>::infinity();
                vizitat[statie->getNume()] = false;
            }
        }

        distante[start] = 0;

        std::priority_queue<Nod, std::vector<Nod>, std::greater<Nod>> coada;
        coada.push(Nod(start, 0, {start}));

        while (!coada.empty()) {
            Nod nodCurent = coada.top();
            coada.pop();

            std::string statieCurenta = nodCurent.numeStatie;

            if (statieCurenta == destinatie) {
                rutaCompleta = nodCurent.ruta;
                timpTotal = nodCurent.distanta;
                return true;
            }

            if (vizitat[statieCurenta]) {
                continue;
            }

            vizitat[statieCurenta] = true;

            for (const auto& vecin : graf[statieCurenta]) {
                std::string statieVecina = vecin.first;
                double costMargine = vecin.second;

                if (distante[statieVecina] > distante[statieCurenta] + costMargine) {
                    distante[statieVecina] = distante[statieCurenta] + costMargine;
                    std::vector<std::string> rutaNoua = nodCurent.ruta;
                    rutaNoua.push_back(statieVecina);

                    coada.push(Nod(statieVecina, distante[statieVecina], rutaNoua));
                }
            }
        }

        std::cout << "Eroare: Nu s-a putut calcula o ruta intre " << start << " si " << destinatie << " la ora " << ora << std::endl;
        return false;
}
int ReteaMetrou::numarTotalStatii() const {
    std::vector<std::string> toateStatiile;

        for (const auto& traseu : trasee) {
            const std::vector<std::shared_ptr<Statia>>& statiiTraseu = traseu->getStatii();
            for (const auto& statie : statiiTraseu) {
                toateStatiile.push_back(statie->getNume());
            }
        }

        sort(toateStatiile.begin(), toateStatiile.end());
        auto last = unique(toateStatiile.begin(), toateStatiile.end());
        toateStatiile.erase(last, toateStatiile.end());

        return toateStatiile.size();
}

double ReteaMetrou::lungimeTotalaRetea() const {
    double lungimeTotala = 0;

    for (const auto& traseu : trasee) {
        lungimeTotala += traseu->calculeazaDistantaTotala();
    }

    return lungimeTotala;
}

std::string ReteaMetrou::statiaAglomerata() const {
    std::string numeStatie;
    int timpMaxim = -1;

    for (const auto& traseu : trasee) {
        const std::vector<std::shared_ptr<Statia>>& statii = traseu->getStatii();
        for (const auto& statie : statii) {
            if (statie->getTimpAsteptare() > timpMaxim) {
                timpMaxim = statie->getTimpAsteptare();
                numeStatie = statie->getNume();
            }
        }
    }

    return numeStatie;
}

void ReteaMetrou::afisareStatisticiTrenuri() const {
    std::cout << "\nStatistici trenuri din reteaua " << numeRetea << ":\n";

        int nrTrenRapid = 0, nrTrenNoapte = 0, nrTrenElectric = 0;
        double eficientaMedieTrenuri = 0.0;

        for (const auto& traseu : trasee) {
            auto tren = traseu->getTren();

            // Adăugăm la eficiența medie
            eficientaMedieTrenuri += tren->calculeazaEficienta();

            // Numărăm tipul de trenuri
            if (dynamic_cast<TrenRapid*>(tren.get())) {
                nrTrenRapid++;
            }
            else if (dynamic_cast<TrenNoapte*>(tren.get())) {
                nrTrenNoapte++;
            }
            else if (dynamic_cast<TrenElectric*>(tren.get())) {
                nrTrenElectric++;
            }
        }

        if (!trasee.empty()) {
            eficientaMedieTrenuri /= trasee.size();
        }

        std::cout << "Total trenuri: " << trasee.size() << "\n";
        std::cout << "- Trenuri Rapide: " << nrTrenRapid << "\n";
        std::cout << "- Trenuri de Noapte: " << nrTrenNoapte << "\n";
        std::cout << "- Trenuri Electrice: " << nrTrenElectric << "\n";
        std::cout << "Eficienta medie a trenurilor: " << eficientaMedieTrenuri << "\n";

        // Afișăm detalii specifice pentru fiecare tip de tren
        for (const auto& traseu : trasee) {
            auto tren = traseu->getTren();
            std::cout << "\nTrenul de pe traseul " << traseu->getNumeRuta() << ":\n";

            if (const TrenElectric* trenElectric = dynamic_cast<const TrenElectric*>(tren.get())) {
                std::cout << "  - Tip: Electric\n";
                std::cout << "  - Eficienta energetica: " << trenElectric->getEficientaEnergetica() << "%\n";
                std::cout << "  - Autonomie baterie: " << trenElectric->getAutonomieBaterie() << " minute\n";
                std::cout << "  - Mod Eco: " << (trenElectric->getModEco() ? "Activat" : "Dezactivat") << "\n";
            }
            else if (const TrenRapid* trenRapid = dynamic_cast<const TrenRapid*>(tren.get())) {
                std::cout << "  - Tip: Rapid\n";
                std::cout << "  - Acceleratie: " << trenRapid->getAcceleratie() << "\n";
                std::cout << "  - Oprire selectiva: " << (trenRapid->getOprireStatiiSelectate() ? "Da" : "Nu") << "\n";
            }
            else if (const TrenNoapte* trenNoapte = dynamic_cast<const TrenNoapte*>(tren.get())) {
                std::cout << "  - Tip: Nocturn\n";
                std::cout << "  - Program: " << trenNoapte->getOraStart() << ":00 - "
                     << trenNoapte->getOraStop() << ":00\n";
                std::cout << "  - Factor viteza: " << trenNoapte->getFactorViteza() << "\n";
            }
            else {
                std::cout << "  - Tip: Necunoscut\n";
            }
        }
}

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <limits>
#include <memory>
#include <ctime>


#include "headers/Statia.h"
#include "headers/Tren.h"
#include "headers/TrenRapid.h"
#include "headers/TrenNoapte.h"
#include "headers/TrenElectric.h"
#include "headers/Traseu.h"
#include "headers/ReteaMetrou.h"
#include "headers/TrenFactory.h"
#include "headers/TraseuBuilder.h"

using namespace std;

template <typename T>
class CatalogEntitati {
private:
    std::vector<std::shared_ptr<T>> entitati;
    std::string numeCatalog;

public:
    CatalogEntitati(const std::string& nume = "Catalog Generic") : numeCatalog(nume) {}

    void adaugaEntitate(std::shared_ptr<T> entitate) {
        if (entitate) {
            entitati.push_back(entitate);
        }
    }

    // Presupunem că tipul T are o metodă publică:
    // const std::string& getNume() const; (sau un echivalent pentru identificare)
    std::shared_ptr<T> gasesteDupaNume(const std::string& numeDeCautat) const {
        auto it = std::find_if(entitati.begin(), entitati.end(),
                               [&numeDeCautat](const std::shared_ptr<T>& entitatePtr) {
                                   // Verifică dacă pointerul nu e null și apelează getNume()
                                   return entitatePtr && entitatePtr->getNume() == numeDeCautat;
                               });
        if (it != entitati.end()) {
            return *it; // Returnează shared_ptr-ul către entitatea găsită
        }
        return nullptr; // Nu s-a găsit
    }

    void afiseazaToateEntitatile() const {
        std::cout << "--- " << numeCatalog << " ---" << std::endl;
        if (entitati.empty()) {
            std::cout << "Catalogul este gol." << std::endl;
            return;
        }
        int index = 1;
        for (const auto& entitatePtr : entitati) {
            if (entitatePtr) {
                // Presupunem că tipul T are operator<< supraîncărcat pentru afișare
                std::cout << index++ << ". " << *entitatePtr << std::endl;
            }
        }
        std::cout << "-----------------------------" << std::endl;
    }

    const std::vector<std::shared_ptr<T>>& getAll() const {
        return entitati;
    }
};


int main() {
    try {
        // Creare retea de metrou
        ReteaMetrou retea("Metrou Bucuresti");
        // auto trenTemp = make_shared<Tren>("Temp", 0);
        // Creare trenuri folosind smart pointers - obiecte derivate din clasa Tren
        auto tren1 = make_shared<TrenRapid>("M1-R", 80, 25, true);
        auto tren2 = make_shared<TrenNoapte>("M2-N", 70, 23, 5, 0.8);
        auto tren3 = make_shared<TrenElectric>("M3-E", 75, 90, 480, true);
        //auto tren4 = std::make_shared<TrenIstoric>("TREN007", 40, 17, 19);

        cout << "\n--- DEMONSTRATIE PATTERN BUILDER PENTRU TRASEU ---\n";
        TraseuBuilder traseuBuilder;

        // Construirea Traseului 1 folosind Builder
        auto traseul1_builder = traseuBuilder
                                    .setName("Magistrala 1 (Builder): Dristor 2 - Pantelimon")
                                    .assignTrain(tren1)
                                    .addStatiaDetails("Dristor 2", 30, 3)
                                    .addStatia(make_shared<Statia>("Piata Muncii", 25, 2))
                                    .addStatiaDetails("Iancului", 20, 4)
                                    .addStatiaDetails("Obor", 35, 3)
                                    .addStatiaDetails("Pantelimon", 25, 0)
                                    .build();

        cout << "Traseul 1 construit cu Builder:\n";
        traseul1_builder->afisare(); // Afișăm traseul construit
        cout << "---------------------------------------------------\n";

        // Resetăm builder-ul pentru a construi Traseul 2
        traseuBuilder.reset();
        auto traseul2_builder = traseuBuilder
                                    .setName("Magistrala 2 (Builder): Pipera - Tudor Arghezi")
                                    .assignTrain(tren2)
                                    .addStatiaDetails("Pipera", 30, 2)
                                    .addStatiaDetails("Aurel Vlaicu", 20, 2)
                                    .addStatiaDetails("Piata Victoriei", 40, 1)
                                    .addStatiaDetails("Tudor Arghezi", 25, 0)
                                    .build();

        cout << "Traseul 2 construit cu Builder:\n";
        traseul2_builder->afisare();
        cout << "---------------------------------------------------\n";


        // Creare trasee folosind smart pointers
        auto traseul1 = make_shared<Traseu>("Magistrala 1: Pantelimon - Gara de Nord", tren1);
        auto traseul2 = make_shared<Traseu>("Magistrala 2: Pipera - Berceni", tren2);
        auto traseul3 = std::make_shared<Traseu>("Magistrala 3: Preciziei - Anghel Saligny", tren3);

        // Creare statii folosind smart pointers
        traseul1->adaugaStatia(make_shared<Statia>("Pantelimon", 20, 3));
        traseul1->adaugaStatia(make_shared<Statia>("Iancului", 25, 2));
        traseul1->adaugaStatia(make_shared<Statia>("Piata Muncii", 30, 4));
        traseul1->adaugaStatia(make_shared<Statia>("Dristor", 35, 3));
        traseul1->adaugaStatia(make_shared<Statia>("Piata Unirii", 40, 2));
        traseul1->adaugaStatia(make_shared<Statia>("Izvor", 20, 3));
        traseul1->adaugaStatia(make_shared<Statia>("Eroilor", 30, 2));
        traseul1->adaugaStatia(make_shared<Statia>("Gara de Nord", 25, 0));

        traseul2->adaugaStatia(make_shared<Statia>("Pipera", 30, 3));
        traseul2->adaugaStatia(make_shared<Statia>("Aurel Vlaicu", 20, 2));
        traseul2->adaugaStatia(make_shared<Statia>("Piata Victoriei", 35, 4));
        traseul2->adaugaStatia(make_shared<Statia>("Piata Romana", 25, 2));
        traseul2->adaugaStatia(make_shared<Statia>("Universitate", 30, 2));
        traseul2->adaugaStatia(make_shared<Statia>("Piata Unirii", 40, 3));
        traseul2->adaugaStatia(make_shared<Statia>("Tineretului", 25, 2));
        traseul2->adaugaStatia(make_shared<Statia>("Berceni", 20, 0));

        traseul3->adaugaStatia(make_shared<Statia>("Preciziei", 25, 4));
        traseul3->adaugaStatia(make_shared<Statia>("Politehnica", 20, 3));
        traseul3->adaugaStatia(make_shared<Statia>("Eroilor", 30, 2));
        traseul3->adaugaStatia(make_shared<Statia>("Piata Unirii", 40, 3));
        traseul3->adaugaStatia(make_shared<Statia>("Dristor", 35, 4));
        traseul3->adaugaStatia(make_shared<Statia>("Anghel Saligny", 20, 0));

        retea.adaugaTraseu(traseul1_builder);
        retea.adaugaTraseu(traseul2);
        retea.adaugaTraseu(traseul3);

        retea.afisareTrasee();

        cout << "--------------------\n";

        cout << "Afisare Finala: \n";
        retea.afisareStatisticiTrenuri();

        cout << "--------------------\n";
        // 2. Informatii despre retea
        cout << "\n";
        cout << "2. Informatii despre retea:\n";
        cout << "Numar total de statii: " << retea.numarTotalStatii() << "\n";
        cout << "Lungimea totala a retelei: " << retea.lungimeTotalaRetea() << " km \n";
        cout << "Statia cu cel mai mare timp de asteptare: " << retea.statiaAglomerata() << "\n";

        // 3. Calculare ruta intre doua statii

        cout << "\n3. Calculare ruta intre doua statii din trasee diferite:\n";
        // Test pentru diferite ore ale zilei
        cout << "PROGRAMUL DE CIRCULATIE A TRENURILOR:\n";
        cout << "- Trenurile Rapide circula intre orele 05:00 - 13:00\n";
        cout << "- Trenurile Electrice circula intre orele 13:00 - 23:00\n";
        cout << "- Trenurile de Noapte circula intre orele 23:00 - 05:00\n\n";

        // Testarea disponibilității trenurilor în funcție de oră
        vector<int> oreTest = {3, 8, 18, 23};

        cout << "Tipul de tren disponibil la diferite ore:\n";
        for (int ora : oreTest) {
            cout << "Ora " << ora << ":00 - ";
            if (ora >= 5 && ora < 13) {
                cout << "Trenul Rapid este disponibil pe toate magistralele";
            } else if (ora >= 13 && ora < 23) {
                cout << "Trenul Electric este disponibil pe toate magistralele";
            } else {
                cout << "Trenul de Noapte este disponibil pe toate magistralele";
            }
            cout << endl;
        }

        // Test pentru calcularea rutelor în funcție de oră
        vector<pair<string, string>> perechiTest = {
            {"Pantelimon", "Pipera"},
            {"Preciziei", "Berceni"},
            {"Gara de Nord", "Dristor"}
        };

        cout << "\nTestarea calcularii rutei optime pentru diferite ore ale zilei:\n";
        for (int ora : oreTest) {
            cout << "\n======================================\n";
            cout << "Ora curenta: " << ora << ":00\n";

            for (const auto& pereche : perechiTest) {
                const string& start = pereche.first;
                const string& destinatie = pereche.second;

                cout << "\nCalculare ruta de la " << start << " la " << destinatie << ":\n";

                double timpTotal = 0;
                vector<string> rutaCompleta;

                if (retea.calculeazaRutaOptima(start, destinatie, timpTotal, rutaCompleta, ora)) {
                    cout << "Ruta gasita:\n";
                    for (size_t i = 0; i < rutaCompleta.size(); i++) {
                        cout << rutaCompleta[i];
                        if (i < rutaCompleta.size() - 1) {
                            cout << " -> ";
                        }
                    }
                    cout << "\nTimp total estimat: " << timpTotal << " minute\n";
                }
                double timpCalatorie;
                vector<string> ruta;
                // Test RutaNotFoundException
                cout << "\nTest RutaNotFoundException\n";
                try {
                    retea.calculeazaRutaOptima("Gara de Nord", "Berceni", timpCalatorie, ruta, 10);
                } catch (const RutaNotFoundException& e) {
                    cerr << "Exceptie prinsa: " << e.what() << endl;
                }

                // Test TraseuLimitExceededException
                cout << "\nTest TraseuLimitExceededException\n";
                try {
                    for (int i = 0; i < 15; ++i) { // Adaugam mai multe trasee decat limita
                        retea.adaugaTraseu("Traseu" + to_string(i), make_shared<TrenElectric>("TE07", 110, 95, 130, true));
                    }
                } catch (const TraseuLimitExceededException& e) {
                    cerr << "Exceptie prinsa: " << e.what() << endl;
                }

            }
        }
        cout << "Afisare Finala: \n";
        retea.afisareStatisticiTrenuri();

        cout << "\n--- DEMONSTRATIE CLASA SABLON: CatalogEntitati ---" << std::endl;


        CatalogEntitati<Statia> catalogStatii("Catalog Statii Metrou");


        auto statieDemo1 = std::make_shared<Statia>("Piata Unirii 1", 45, 3);
        auto statieDemo2 = std::make_shared<Statia>("Piata Victoriei", 40, 2);
        auto statieDemo3 = std::make_shared<Statia>("Eroilor", 35, 2);

        catalogStatii.adaugaEntitate(statieDemo1);
        catalogStatii.adaugaEntitate(traseul1->getStatii()[0]);
        catalogStatii.adaugaEntitate(statieDemo2);
        catalogStatii.adaugaEntitate(statieDemo3);

        catalogStatii.afiseazaToateEntitatile();

        std::cout << "Cautare statie 'Piata Victoriei' in catalogStatii..." << std::endl;
        std::shared_ptr<Statia> statieGasita = catalogStatii.gasesteDupaNume("Piata Victoriei");
        if (statieGasita) {
            std::cout << "Gasit: " << *statieGasita << std::endl;
        } else {
            std::cout << "Statia 'Piata Victoriei' nu a fost gasita." << std::endl;
        }

        std::cout << "Cautare statie 'Grozavesti' (inexistenta) in catalogStatii..." << std::endl;
        statieGasita = catalogStatii.gasesteDupaNume("Grozavesti");
        if (statieGasita) {
            std::cout << "Gasit: " << *statieGasita << std::endl;
        } else {
            std::cout << "Statia 'Grozavesti' nu a fost gasita." << std::endl;
        }

        CatalogEntitati<Tren> catalogTrenuri("Catalog Trenuri Parc");

        catalogTrenuri.adaugaEntitate(tren1);
        catalogTrenuri.adaugaEntitate(tren2);
        catalogTrenuri.adaugaEntitate(tren3);


        auto trenDemoElectric = std::make_shared<TrenElectric>("T-Electric-004", 70, 88.0, 420, false);
        catalogTrenuri.adaugaEntitate(trenDemoElectric);

        catalogTrenuri.afiseazaToateEntitatile();

        std::cout << "Cautare tren 'M2-N' in catalogTrenuri..." << std::endl;
        std::shared_ptr<Tren> trenGasit = catalogTrenuri.gasesteDupaNume("M2-N");
        if (trenGasit) {
            std::cout << "Gasit: ";
            trenGasit->afisare();
            std::cout << std::endl;
        } else {
            std::cout << "Trenul 'M2-N' nu a fost gasit." << std::endl;
        }

        std::cout << "Cautare tren 'X99-Z' (inexistent) in catalogTrenuri..." << std::endl;
        trenGasit = catalogTrenuri.gasesteDupaNume("X99-Z");
        if (trenGasit) {
            std::cout << "Gasit: " << *trenGasit << std::endl;
        } else {
            std::cout << "Trenul 'X99-Z' nu a fost gasit." << std::endl;
        }
        std::cout << "----------------------------------------------------" << std::endl;

    }
    catch (const ReteaMetrouException& e) {
        cerr << "Exceptie generala prinsa: " << e.what() << endl;
        return 1;
    }
    cout << "Program terminat cu succes!\n";




    return 0;
}
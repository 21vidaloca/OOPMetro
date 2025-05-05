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

using namespace std;



int main() {
    try {
        // Creare retea de metrou
        ReteaMetrou retea("Metrou Bucuresti");
        // auto trenTemp = make_shared<Tren>("Temp", 0);
        // Creare trenuri folosind smart pointers - obiecte derivate din clasa Tren
        auto tren1 = make_shared<TrenRapid>("M1-R", 80, 25, true);
        auto tren2 = make_shared<TrenNoapte>("M2-N", 70, 23, 5, 0.8);
        auto tren3 = make_shared<TrenElectric>("M3-E", 75, 90, 480, true);

        // Creare trasee folosind smart pointers
        auto traseul1 = make_shared<Traseu>("Magistrala 1: Pantelimon - Gara de Nord", tren1);
        auto traseul2 = make_shared<Traseu>("Magistrala 2: Pipera - Berceni", tren2);
        auto traseul3 = make_shared<Traseu>("Magistrala 3: Preciziei - Anghel Saligny", tren3);

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

        retea.adaugaTraseu(traseul1);
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
        vector<int> oreTest = {3, 8, 15, 23};

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
                /*double timpCalatorie;
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
                }*/

            }
        }
        cout << "Afisare Finala: \n";
        retea.afisareStatisticiTrenuri();
    }
    catch (const ReteaMetrouException& e) {
        cerr << "Exceptie generala prinsa: " << e.what() << endl;
        return 1;
    }
    cout << "Program terminat cu succes!\n";
    return 0;
}
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <limits>
using namespace std;

class Statia {
    string nume;
    int timpAsteptare;
    int distantaUrmatoareStatie;

public:
    Statia() : nume("Unknown"), timpAsteptare(0), distantaUrmatoareStatie(0) {}

    Statia(const string& nume, int timpAsteptare, int distantaUrmatoareStatie = 2) {
        this->nume = nume;
        this->timpAsteptare = timpAsteptare;
        this->distantaUrmatoareStatie = distantaUrmatoareStatie;
    }

    // Constructor de copiere
    Statia(const Statia& other) :
        nume(other.nume),
        timpAsteptare(other.timpAsteptare),
        distantaUrmatoareStatie(other.distantaUrmatoareStatie) {}

    // Operator de atribuire
    Statia& operator=(const Statia& other) {
        if (this != &other) {
            nume = other.nume;
            timpAsteptare = other.timpAsteptare;
            distantaUrmatoareStatie = other.distantaUrmatoareStatie;
        }
        return *this;
    }

    // Destructor
    ~Statia() {}

    friend ostream& operator<<(ostream& os, const Statia& statie) {
        os << "Statia: " << statie.nume << " | Timp asteptare: " << statie.timpAsteptare
           << " sec | Distanta pana la urmatoarea statie: " << statie.distantaUrmatoareStatie << " km";
        return os;
    }

    int getTimpAsteptare() const { return timpAsteptare; }
    const string& getNume() const { return nume; }
    int getDistantaUrmatoareStatie() const { return distantaUrmatoareStatie; }

    bool operator==(const Statia& other) const {
        return nume == other.nume;
    }

    bool operator==(const string& numeStatie) const {
        return nume == numeStatie;
    }
};

class Tren {
    string id;
    int viteza; // in km/h

public:
    Tren() : id("Unknown"), viteza(0) {}

    Tren(const string& id, int viteza)
        : id(id), viteza(viteza) {}

    // Constructor de copiere
    Tren(const Tren& other)
        : id(other.id), viteza(other.viteza) {}

    // Operator de atribuire
    Tren& operator=(const Tren& other) {
        if (this != &other) {
            id = other.id;
            viteza = other.viteza;
        }
        return *this;
    }

    // Destructor
    ~Tren() {}

    friend ostream& operator<<(ostream& os, const Tren& tren) {
        os << "Tren ID: " << tren.id << " | Viteza: " << tren.viteza << " km/h";
        return os;
    }

    double calculeazaTimpParcurgere(double distanta) const {
        // Timpul in ore
        double timpOre = distanta / viteza;
        return timpOre * 60.0; // conversie
    }
};

class Traseu {
    Tren tren;
    string numeRuta;
    vector<Statia> statii;


public:
    explicit Traseu(const Tren& tren, const string& numeRuta = "Ruta standard")
        : tren(tren), numeRuta(numeRuta) {}

    // Constructor de copiere
    Traseu(const Traseu& other)
        : tren(other.tren),numeRuta(other.numeRuta), statii(other.statii) {}

    // Operator de atribuire
    Traseu& operator=(const Traseu& other) {
        if (this != &other) {
            numeRuta = other.numeRuta;
            tren = other.tren;
            statii = other.statii;
        }
        return *this;
    }

    // Destructor
    ~Traseu() {}


    // Getter pentru lista de statii
    const vector<Statia>& getStatii() const {
        return statii;
    }


    // Getter pentru tren
    const Tren& getTren() const {
        return tren;
    }

    void adaugaStatia(const Statia& statie) {
        statii.push_back(statie);
    }


    double calculeazaDistantaTotala() const {
        double distantaTotala = 0;
        for (size_t i = 0; i < statii.size(); i++) {
            distantaTotala += statii[i].getDistantaUrmatoareStatie();
        }
        return distantaTotala;
    }


    bool calculeazaRutaIntreStatii(const string& start, const string& destinatie,
                                 double& timpCalatorie, vector<string>& rutaStatii) const {

        int indexStart = -1;
        int indexDestinatie = -1;

        for (size_t i = 0; i < statii.size(); i++) {
            if (statii[i].getNume() == start) {
                indexStart = i;
            }
            if (statii[i].getNume() == destinatie) {
                indexDestinatie = i;
            }

            if (indexStart != -1 && indexDestinatie != -1) {
                break;
            }
        }

        if (indexStart == -1 || indexDestinatie == -1) {
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
            distanta += statii[i].getDistantaUrmatoareStatie();
        }

        timpCalatorie = tren.calculeazaTimpParcurgere(distanta);

        for (int i = startIdx; i <= endIdx; i++) {
            rutaStatii.push_back(statii[i].getNume());

            if (i < endIdx) {
                timpCalatorie += statii[i].getTimpAsteptare() / 60.0; // conversie
            }
        }

        return true;
    }

    // Verifica daca o statie face parte din traseu
    bool contineStatia(const string& numeStatie) const {
        for (const auto& statie : statii) {
            if (statie.getNume() == numeStatie) {
                return true;
            }
        }
        return false;
    }

    friend ostream& operator<<(ostream& os, const Traseu& traseu) {
        os << "Traseu: " << traseu.numeRuta << "\n";
        os << "Tren: " << traseu.tren << "\n";
        os << "Statii:\n";
        for (size_t i = 0; i < traseu.statii.size(); i++) {
            os << i+1 << ". " << traseu.statii[i] << "\n";
        }
        return os;
    }
};

class ReteaMetrou {
private:
    vector<Traseu> trasee;
    string numeRetea;

    struct Nod {
        string numeStatie;
        double distanta;
        vector<string> ruta;
        Nod(const string& nume, double dist, const vector<string>& r)
            : numeStatie(nume), distanta(dist), ruta(r) {}

        bool operator>(const Nod& other) const {
            return distanta > other.distanta;
        }
    };

public:
    // Constructor explicit
    explicit ReteaMetrou(const string& nume = "Metrou Bucuresti") : numeRetea(nume) {}

    // Constructor de copiere
    ReteaMetrou(const ReteaMetrou& other) : trasee(other.trasee), numeRetea(other.numeRetea) {}

    // Operator de atribuire
    ReteaMetrou& operator=(const ReteaMetrou& other) {
        if (this != &other) {
            trasee = other.trasee;
            numeRetea = other.numeRetea;
        }
        return *this;
    }

    // Destructor
    ~ReteaMetrou() {}

    void adaugaTraseu(const Traseu& traseu) {
        trasee.push_back(traseu);
    }

    void afisareTrasee() const {
        cout << "RETEAUA DE METROU: " << numeRetea << endl;
        cout << "Numar trasee: " << trasee.size() << endl << endl;

        for (const auto& traseu : trasee) {
            cout << traseu << "\n";
        }
    }

    bool existaStatieInRetea(const string& numeStatie) const {
        for (const auto& traseu : trasee) {
            if (traseu.contineStatia(numeStatie)) {
                return true;
            }
        }
        return false;
    }

    // Calculeaza ruta optima intre doua statii din intreaga retea
    bool calculeazaRutaOptima(const string& start, const string& destinatie,
                            double& timpTotal, vector<string>& rutaCompleta) const {

        if (!existaStatieInRetea(start) || !existaStatieInRetea(destinatie)) {
            return false;
        }

        // Cazul simplu: ambele statii sunt pe acelasi traseu
        for (const auto& traseu : trasee) {
            if (traseu.contineStatia(start) && traseu.contineStatia(destinatie)) {
                vector<string> rutaStatii;
                double timp = 0;
                bool success = traseu.calculeazaRutaIntreStatii(start, destinatie, timp, rutaStatii);

                if (success) {
                    rutaCompleta = rutaStatii;
                    timpTotal = timp;
                    return true;
                }
            }
        }

        // Cazul complex: statiile sunt pe trasee diferite
        unordered_map<string, vector<pair<string, double>>> graf;

        for (const auto& traseu : trasee) {
            const vector<Statia>& statii = traseu.getStatii();
            const Tren& tren = traseu.getTren();

            for (size_t i = 0; i < statii.size(); i++) {
                const string& statieActuala = statii[i].getNume();

                if (i > 0) {
                    string statiePrecedenta = statii[i-1].getNume();
                    double distanta = statii[i-1].getDistantaUrmatoareStatie();
                    double timp = tren.calculeazaTimpParcurgere(distanta) + statii[i-1].getTimpAsteptare() / 60.0;

                    graf[statieActuala].push_back({statiePrecedenta, timp});
                    graf[statiePrecedenta].push_back({statieActuala, timp});
                }
            }
        }

        unordered_map<string, double> distante;
        unordered_map<string, bool> vizitat;

        for (const auto& traseu : trasee) {
            const vector<Statia>& statii = traseu.getStatii();
            for (const auto& statie : statii) {
                distante[statie.getNume()] = numeric_limits<double>::infinity();
                vizitat[statie.getNume()] = false;
            }
        }

        distante[start] = 0;

        priority_queue<Nod, vector<Nod>, greater<Nod>> coada;
        coada.push(Nod(start, 0, {start}));

        while (!coada.empty()) {
            Nod nodCurent = coada.top();
            coada.pop();

            string statieCurenta = nodCurent.numeStatie;

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
                string statieVecina = vecin.first;
                double costMargine = vecin.second;

                if (distante[statieVecina] > distante[statieCurenta] + costMargine) {
                    distante[statieVecina] = distante[statieCurenta] + costMargine;
                    vector<string> rutaNoua = nodCurent.ruta;
                    rutaNoua.push_back(statieVecina);

                    coada.push(Nod(statieVecina, distante[statieVecina], rutaNoua));
                }
            }
        }

        return false;
    }

    int numarTotalStatii() const {
        vector<string> toateStatiile;

        for (const auto& traseu : trasee) {
            const vector<Statia>& statiiTraseu = traseu.getStatii();
            for (const auto& statie : statiiTraseu) {
                toateStatiile.push_back(statie.getNume());
            }
        }

        sort(toateStatiile.begin(), toateStatiile.end());
        auto last = unique(toateStatiile.begin(), toateStatiile.end());
        toateStatiile.erase(last, toateStatiile.end());

        return toateStatiile.size();
    }

    double lungimeTotalaRetea() const {
        double lungimeTotala = 0;

        for (const auto& traseu : trasee) {
            lungimeTotala += traseu.calculeazaDistantaTotala();
        }

        return lungimeTotala;
    }

    string statiaAglomerata() const {
        string numeStatie;
        int timpMaxim = -1;

        for (const auto& traseu : trasee) {
            const vector<Statia>& statii = traseu.getStatii();
            for (const auto& statie : statii) {
                if (statie.getTimpAsteptare() > timpMaxim) {
                    timpMaxim = statie.getTimpAsteptare();
                    numeStatie = statie.getNume();
                }
            }
        }

        return numeStatie;
    }
};

int main() {
    ReteaMetrou retea("Metrou Bucuresti");


    Tren tren1("M1", 80);
    Tren tren2("M2", 70);
    Tren tren3("M3", 75);


    Traseu traseul1(tren1, "Magistrala 1: Pantelimon - Gara de Nord");
    traseul1.adaugaStatia(Statia("Pantelimon", 20, 3));
    traseul1.adaugaStatia(Statia("Iancului", 25, 2));
    traseul1.adaugaStatia(Statia("Piata Muncii", 30, 4));
    traseul1.adaugaStatia(Statia("Dristor", 35, 3));
    traseul1.adaugaStatia(Statia("Piata Unirii", 40, 2));
    traseul1.adaugaStatia(Statia("Izvor", 20, 3));
    traseul1.adaugaStatia(Statia("Eroilor", 30, 2));
    traseul1.adaugaStatia(Statia("Gara de Nord", 25, 0));

    Traseu traseul2(tren2, "Magistrala 2: Pipera - Berceni");
    traseul2.adaugaStatia(Statia("Pipera", 30, 3));
    traseul2.adaugaStatia(Statia("Aurel Vlaicu", 20, 2));
    traseul2.adaugaStatia(Statia("Piata Victoriei", 35, 4));
    traseul2.adaugaStatia(Statia("Piata Romana", 25, 2));
    traseul2.adaugaStatia(Statia("Universitate", 30, 2));
    traseul2.adaugaStatia(Statia("Piata Unirii", 40, 3));
    traseul2.adaugaStatia(Statia("Tineretului", 25, 2));
    traseul2.adaugaStatia(Statia("Berceni", 20, 0));

    Traseu traseul3(tren3, "Magistrala 3: Preciziei - Anghel Saligny");
    traseul3.adaugaStatia(Statia("Preciziei", 25, 4));
    traseul3.adaugaStatia(Statia("Politehnica", 20, 3));
    traseul3.adaugaStatia(Statia("Eroilor", 30, 2));
    traseul3.adaugaStatia(Statia("Piata Unirii", 40, 3));
    traseul3.adaugaStatia(Statia("Dristor", 35, 4));
    traseul3.adaugaStatia(Statia("Anghel Saligny", 20, 0));


    retea.adaugaTraseu(traseul1);
    retea.adaugaTraseu(traseul2);
    retea.adaugaTraseu(traseul3);
    retea.afisareTrasee();

    cout << "--------------------\n";

    // 1. Calcularea timpului total intre doua statii
    cout << "\n1. Calculare timp intre doua statii:\n";
    double timpCalatorie = 0;
    vector<string> rutaStatii;

    if (traseul1.calculeazaRutaIntreStatii("Piata Unirii", "Eroilor", timpCalatorie, rutaStatii)) {
        cout << "Ruta de la Piata Unirii la Eroilor:\n";
        for (size_t i = 0; i < rutaStatii.size(); i++) {
            cout << i+1 << ". " << rutaStatii[i];
            if (i < rutaStatii.size() - 1) {
                cout << " -> ";
            }
            if ((i + 1) % 3 == 0) cout << "\n";
        }
        if (rutaStatii.size() % 3 != 0)
            cout << "\n";
        cout << "Timp total estimat: " << timpCalatorie << " minute\n";
    } else {
        cout << "Nu s-a putut calcula ruta intre aceste statii.\n";
    }

    // 2. Numarul total de statii din retea
    cout << "\n";
    cout << "2. Informatii despre retea:\n";
    cout << "Numar total de statii: " << retea.numarTotalStatii() << endl;
    cout << "Lungimea totala a retelei: " << retea.lungimeTotalaRetea() << " km" << endl;
    cout << "Statia cu cel mai mare timp de asteptare: " << retea.statiaAglomerata() << endl;

    // 3. Calculare ruta intre doua statii (posibil din trasee diferite)
    cout << "\n3. Calculare ruta intre doua statii din trasee diferite:\n";
    double timpTotal = 0;
    vector<string> rutaCompleta;

    if (retea.calculeazaRutaOptima("Pantelimon", "Pipera", timpTotal, rutaCompleta)) {
        cout << "Ruta de la Pantelimon la Pipera:\n";
        for (size_t i = 0; i < rutaCompleta.size(); i++) {
            cout << i+1 << ". " << rutaCompleta[i];
            if (i < rutaCompleta.size() - 1) {
                cout << " -> ";
            }
            if ((i + 1) % 3 == 0)
                cout << "\n";
        }
        if (rutaCompleta.size() % 3 != 0)
            cout << "\n";
        cout << "Timp total estimat: " << timpTotal << " minute\n";
    }
    else {
        cout << "Nu s-a putut calcula ruta intre aceste statii.\n";
    }

    return 0;
}
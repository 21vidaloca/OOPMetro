#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <limits>
#include <memory>
#include <ctime>
using namespace std;

// Clasa Statia pentru gestionarea statiilor
class Statia {
private:
    string nume;
    int timpAsteptare;
    int distantaUrmatoareStatie;

public:
    Statia(const string& nume, int timpAsteptare, int distantaUrmatoareStatie = 2)
        : nume(nume), timpAsteptare(timpAsteptare), distantaUrmatoareStatie(distantaUrmatoareStatie) {}

    // Constructor de copiere
    Statia(const Statia& other)
        : nume(other.nume), timpAsteptare(other.timpAsteptare),
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

    // Metoda pentru afisare
    void afisare() const {
        cout << "Statia: " << nume << " | Timp asteptare: " << timpAsteptare
             << " sec | Distanta pana la urmatoarea statie: " << distantaUrmatoareStatie << " km";
    }

    // Getteri
    const string& getNume() const { return nume; }
    int getTimpAsteptare() const { return timpAsteptare; }
    int getDistantaUrmatoareStatie() const { return distantaUrmatoareStatie; }

    // Operatori de comparare
    bool operator==(const Statia& other) const {
        return nume == other.nume;
    }

    bool operator==(const string& numeStatie) const {
        return nume == numeStatie;
    }

    // Operator pentru afisare
    friend ostream& operator<<(ostream& os, const Statia& statie) {
        statie.afisare();
        return os;
    }
};

// Clasa de baza Tren
class Tren {
protected:
    string id;
    int viteza; // in km/h

public:
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

    // Destructor virtual pentru a permite polimorfismul
    virtual ~Tren() {}

    // Functie virtuala pentru afisare
    virtual void afisare() const {
        cout << "Tren ID: " << id << " | Viteza: " << viteza << " km/h";
    }

    // Functie virtuala pentru calculul timpului de parcurgere
    virtual double calculeazaTimpParcurgere(double distanta) const {
        // Timpul in ore
        double timpOre = distanta / viteza;
        return timpOre * 60.0; // conversie la minute
    }

    // Functie virtuala pentru calculul eficientei
    virtual double calculeazaEficienta() const {
        // Implementare de baza
        return 0.5; // Eficienta standard
    }

    // Getteri
    // const string& getId() const { return id; }
    int getViteza() const { return viteza; }

    // Operator pentru afisare
    friend ostream& operator<<(ostream& os, const Tren& tren) {
        tren.afisare();
        return os;
    }
};

// Prima clasa derivata - TrenRapid
class TrenRapid : public Tren {
private:
    int acceleratie;
    bool oprireStatiiSelectate;
    vector<string> statiiDeOprire;

public:
    TrenRapid(const string& id, int viteza, int acceleratie, bool oprireSelectiva)
        : Tren(id, viteza), acceleratie(acceleratie), oprireStatiiSelectate(oprireSelectiva) {}

    // Constructor cu lista de statii de oprire
    TrenRapid(const string& id, int viteza, int acceleratie,
              const vector<string>& statiiPermise)
        : Tren(id, viteza), acceleratie(acceleratie),
          oprireStatiiSelectate(true), statiiDeOprire(statiiPermise) {}

    void afisare() const override {
        Tren::afisare();
        cout << " | Tip: Rapid | Acceleratie: " << acceleratie
             << " | Oprire selectiva: " << (oprireStatiiSelectate ? "Da" : "Nu");

        if (oprireStatiiSelectate && !statiiDeOprire.empty()) {
            cout << " | Statii de oprire: ";
            for (size_t i = 0; i < statiiDeOprire.size(); i++) {
                cout << statiiDeOprire[i];
                if (i < statiiDeOprire.size() - 1) cout << ", ";
            }
        }
    }

    double calculeazaEficienta() const override {
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

    double calculeazaTimpParcurgere(double distanta) const override {
        // Viteza efectiva creste cu X% din cauza acceleratiei
        double factorAcceleratie = 1.0 - (acceleratie / 100.0); // ex: acceleratie 20 => 0.8x timpul
        double timpOre = distanta / (getViteza() * (1.0 / factorAcceleratie));
        return timpOre * 60.0; // conversie la minute
    }


    // Getteri specifici
    int getAcceleratie() const { return acceleratie; }
    bool getOprireStatiiSelectate() const { return oprireStatiiSelectate; }
};

// A doua clasa derivata - TrenNoapte
class TrenNoapte : public Tren {
private:
    int oraStart;
    int oraStop;
    double factorViteza;

public:
    TrenNoapte(const string& id, int viteza, int start, int stop, double factor)
        : Tren(id, viteza), oraStart(start), oraStop(stop), factorViteza(factor) {}

    void afisare() const override {
        Tren::afisare();
        cout << " | Tip: Nocturn | Program: " << oraStart << ":00 - " << oraStop
             << ":00 | Factor viteza: " << factorViteza;
    }

    double calculeazaTimpParcurgere(double distanta) const override {
        // In timpul noptii, viteza este redusa conform factorului
        double timpOre = distanta / (getViteza() * factorViteza);
        return timpOre * 60.0;
    }

    double calculeazaEficienta() const override {
        // Pentru trenul de noapte, eficienta este legata de factorul de viteza
        // si de faptul ca opereaza in ore cu mai putini pasageri
        // Un factor de viteza mai mic (tren mai lent) este considerat mai eficient
        double eficientaViteza = 0.8 + (1.0 - factorViteza) * 0.5;

        // Ore de noapte sunt in general mai eficiente din punct de vedere al incarcarii
        double eficientaProgram = 0.9; // 90% eficienta datorita programului de noapte

        return eficientaViteza * eficientaProgram;
    }

    // Getteri specifici
    int getOraStart() const { return oraStart; }
    int getOraStop() const { return oraStop; }
    double getFactorViteza() const { return factorViteza; }
};

// A treia clasa derivata - TrenElectric
class TrenElectric : public Tren {
private:
    double eficientaEnergetica; // procent (0-100)
    int autonomieBaterie;      // minute de functionare
    bool modEco;               // daca modul eco este activat

public:
    TrenElectric(const string& id, int viteza, double eficienta,
                int autonomie, bool eco = false)
        : Tren(id, viteza), eficientaEnergetica(eficienta),
          autonomieBaterie(autonomie), modEco(eco) {}

    void afisare() const override {
        Tren::afisare();
        cout << " | Tip: Electric | Eficienta energetica: " << eficientaEnergetica
             << "% | Autonomie: " << autonomieBaterie
             << " min | Mod Eco: " << (modEco ? "Activat" : "Dezactivat");
    }

    double calculeazaTimpParcurgere(double distanta) const override {
        double vitezaEfectiva = getViteza();

        // In modul eco, viteza este redusa pentru economie de energie
        if (modEco) {
            vitezaEfectiva *= 0.85; // reducere cu 15%
        }

        double timpOre = distanta / vitezaEfectiva;
        return timpOre * 60.0;
    }

    double calculeazaEficienta() const override {
        // Pentru trenul electric, eficienta este determinata de eficienta energetica
        // si de faptul ca este sau nu in modul eco
        double eficientaModificare = modEco ? 1.2 : 1.0; // Modul eco creste eficienta cu 20%
        return eficientaEnergetica * eficientaModificare / 100.0;
    }


    // Getteri specifici
    double getEficientaEnergetica() const { return eficientaEnergetica; }
    int getAutonomieBaterie() const { return autonomieBaterie; }
    bool getModEco() const { return modEco; }

    // Operator pentru afisare
    friend ostream& operator<<(ostream& os, const TrenElectric& tren) {
        tren.afisare();
        return os;
    }
};

// Clasa Traseu pentru gestionarea traseelor
class Traseu {
private:
    string numeRuta;
    shared_ptr<Tren> tren;
    vector<shared_ptr<Statia>> statii;

public:
    explicit Traseu(const string& numeRuta, shared_ptr<Tren> tren)
        : numeRuta(numeRuta), tren(tren) {}

    // Constructor de copiere
    Traseu(const Traseu& other)
        : numeRuta(other.numeRuta), tren(other.tren) {
        // Deep copy pentru statii
        for (const auto& statie : other.statii) {
            statii.push_back(make_shared<Statia>(*statie));
        }
    }

    // Operator de atribuire
    Traseu& operator=(const Traseu& other) {
        if (this != &other) {
            numeRuta = other.numeRuta;
            tren = other.tren;

            // Deep copy pentru statii
            statii.clear();
            for (const auto& statie : other.statii) {
                statii.push_back(make_shared<Statia>(*statie));
            }
        }
        return *this;
    }

    // Metoda pentru afisare
    void afisare() const {
        cout << "Traseu: " << numeRuta << "\n";
        cout << "Tren: ";
        tren->afisare();
        cout << "\nStatii:\n";
        for (size_t i = 0; i < statii.size(); i++) {
            cout << i+1 << ". ";
            statii[i]->afisare();
            cout << "\n";
        }
    }

    void adaugaStatia(shared_ptr<Statia> statie) {
        statii.push_back(statie);
    }

    double calculeazaDistantaTotala() const {
        double distantaTotala = 0;
        for (size_t i = 0; i < statii.size(); i++) {
            distantaTotala += statii[i]->getDistantaUrmatoareStatie();
        }
        return distantaTotala;
    }

    bool calculeazaRutaIntreStatii(const string& start, const string& destinatie, double& timpCalatorie, vector<string>& rutaStatii, const shared_ptr<Tren>& trenDisponibil) const {
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
            cout << "Eroare: Una dintre statii nu exista pe acest traseu." << endl;
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

    bool contineStatia(const string& numeStatie) const {
        for (const auto& statie : statii) {
            if (statie->getNume() == numeStatie) {
                return true;
            }
        }
        return false;
    }

    shared_ptr<Tren> getTren() const {
        return tren;
    }

    const vector<shared_ptr<Statia>>& getStatii() const {
        return statii;
    }

    const string& getNumeRuta() const {
        return numeRuta;
    }

    friend ostream& operator<<(ostream& os, const Traseu& traseu) {
        traseu.afisare();
        return os;
    }
};

// Clasa ReteaMetrou pentru gestionarea retelei
class ReteaMetrou {
private:
    vector<shared_ptr<Traseu>> trasee;
    string numeRetea;

    // Membrii statici
    static const int MAX_TRASEE = 10;

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
    // Constructor
    explicit ReteaMetrou(const string& nume = "Metrou Bucuresti") : numeRetea(nume) {}

    // Destructor virtual
    virtual ~ReteaMetrou() {}

    // Constructor de copiere
    ReteaMetrou(const ReteaMetrou& other) : numeRetea(other.numeRetea) {
        // Deep copy pentru trasee
        for (const auto& traseu : other.trasee) {
            trasee.push_back(make_shared<Traseu>(*traseu));
        }
    }

    // Operator de atribuire
    ReteaMetrou& operator=(const ReteaMetrou& other) {
        if (this != &other) {
            numeRetea = other.numeRetea;

            // Deep copy pentru trasee
            trasee.clear();
            for (const auto& traseu : other.trasee) {
                trasee.push_back(make_shared<Traseu>(*traseu));
            }
        }
        return *this;
    }

    bool adaugaTraseu(shared_ptr<Traseu> traseu) {
        if (trasee.size() >= MAX_TRASEE) {
            cout << "Eroare: Numarul maxim de trasee a fost atins." << endl;
            return false;
        }
        trasee.push_back(traseu);
        return true;
    }

    virtual void afisareTrasee() const {
        cout << "RETEAUA DE METROU: " << numeRetea << "\n";
        cout << "Numar trasee: " << trasee.size() << "\n\n";

        for (const auto& traseu : trasee) {
            cout << *traseu << "\n";
        }
    }

    bool existaStatieInRetea(const string& numeStatie) const {
        for (const auto& traseu : trasee) {
            if (traseu->contineStatia(numeStatie)) {
                return true;
            }
        }
        return false;
    }
    shared_ptr<Tren> obtineTrenDisponibil(int ora) const {
        // Selectăm tipul de tren în funcție de ora din zi
        if (ora >= 5 && ora < 13) {
            return make_shared<TrenRapid>("Tren-Rapid", 80, 25, true); // 05:00-13:00
        }
        else if (ora >= 13 && ora < 23) {
            return make_shared<TrenElectric>("Tren-Electric", 75, 90, 480, true); // 13:00-23:00
        }
        else {
            return make_shared<TrenNoapte>("Tren-Noapte", 70, 23, 5, 0.8); // 23:00-05:00
        }
    }
    bool calculeazaRutaOptima(const string& start, const string& destinatie, double& timpTotal, vector<string>& rutaCompleta, int ora) const {
        if (!existaStatieInRetea(start) || !existaStatieInRetea(destinatie)) {
            cout << "Eroare: Una dintre statii nu exista in retea." << endl;
            return false;
        }

        // Obținem trenul disponibil la ora specificată
        shared_ptr<Tren> trenDisponibil = obtineTrenDisponibil(ora);

        // Cazul simplu: ambele statii sunt pe acelasi traseu
        for (const auto& traseu : trasee) {
            if (traseu->contineStatia(start) && traseu->contineStatia(destinatie)) {
                vector<string> rutaStatii;
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
        unordered_map<string, vector<pair<string, double>>> graf;

        for (const auto& traseu : trasee) {
            const vector<shared_ptr<Statia>>& statii = traseu->getStatii();

            for (size_t i = 0; i < statii.size(); i++) {
                const string& statieActuala = statii[i]->getNume();

                if (i > 0) {
                    string statiePrecedenta = statii[i-1]->getNume();
                    double distanta = statii[i-1]->getDistantaUrmatoareStatie();
                    double timp = trenDisponibil->calculeazaTimpParcurgere(distanta) + statii[i-1]->getTimpAsteptare() / 60.0;

                    graf[statieActuala].push_back({statiePrecedenta, timp});
                    graf[statiePrecedenta].push_back({statieActuala, timp});
                }
            }
        }

        // Dacă graful este gol, înseamnă că nu există trenuri disponibile la ora specificată
        if (graf.empty()) {
            cout << "Eroare: Nu există trenuri disponibile la ora " << ora << endl;
            return false;
        }

        unordered_map<string, double> distante;
        unordered_map<string, bool> vizitat;

        for (const auto& traseu : trasee) {
            const vector<shared_ptr<Statia>>& statii = traseu->getStatii();
            for (const auto& statie : statii) {
                distante[statie->getNume()] = numeric_limits<double>::infinity();
                vizitat[statie->getNume()] = false;
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

        cout << "Eroare: Nu s-a putut calcula o ruta intre " << start << " si " << destinatie << " la ora " << ora << endl;
        return false;
    }

    int numarTotalStatii() const {
        vector<string> toateStatiile;

        for (const auto& traseu : trasee) {
            const vector<shared_ptr<Statia>>& statiiTraseu = traseu->getStatii();
            for (const auto& statie : statiiTraseu) {
                toateStatiile.push_back(statie->getNume());
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
            lungimeTotala += traseu->calculeazaDistantaTotala();
        }

        return lungimeTotala;
    }

    string statiaAglomerata() const {
        string numeStatie;
        int timpMaxim = -1;

        for (const auto& traseu : trasee) {
            const vector<shared_ptr<Statia>>& statii = traseu->getStatii();
            for (const auto& statie : statii) {
                if (statie->getTimpAsteptare() > timpMaxim) {
                    timpMaxim = statie->getTimpAsteptare();
                    numeStatie = statie->getNume();
                }
            }
        }

        return numeStatie;
    }
    void afisareStatisticiTrenuri() const {
        cout << "\nStatistici trenuri din reteaua " << numeRetea << ":\n";

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

        cout << "Total trenuri: " << trasee.size() << "\n";
        cout << "- Trenuri Rapide: " << nrTrenRapid << "\n";
        cout << "- Trenuri de Noapte: " << nrTrenNoapte << "\n";
        cout << "- Trenuri Electrice: " << nrTrenElectric << "\n";
        cout << "Eficienta medie a trenurilor: " << eficientaMedieTrenuri << "\n";

        // Afișăm detalii specifice pentru fiecare tip de tren
        for (const auto& traseu : trasee) {
            auto tren = traseu->getTren();
            cout << "\nTrenul de pe traseul " << traseu->getNumeRuta() << ":\n";

            if (const TrenElectric* trenElectric = dynamic_cast<const TrenElectric*>(tren.get())) {
                cout << "  - Tip: Electric\n";
                cout << "  - Eficienta energetica: " << trenElectric->getEficientaEnergetica() << "%\n";
                cout << "  - Autonomie baterie: " << trenElectric->getAutonomieBaterie() << " minute\n";
                cout << "  - Mod Eco: " << (trenElectric->getModEco() ? "Activat" : "Dezactivat") << "\n";
            }
            else if (const TrenRapid* trenRapid = dynamic_cast<const TrenRapid*>(tren.get())) {
                cout << "  - Tip: Rapid\n";
                cout << "  - Acceleratie: " << trenRapid->getAcceleratie() << "\n";
                cout << "  - Oprire selectiva: " << (trenRapid->getOprireStatiiSelectate() ? "Da" : "Nu") << "\n";
            }
            else if (const TrenNoapte* trenNoapte = dynamic_cast<const TrenNoapte*>(tren.get())) {
                cout << "  - Tip: Nocturn\n";
                cout << "  - Program: " << trenNoapte->getOraStart() << ":00 - "
                     << trenNoapte->getOraStop() << ":00\n";
                cout << "  - Factor viteza: " << trenNoapte->getFactorViteza() << "\n";
            }
            else {
                cout << "  - Tip: Necunoscut\n";
            }
        }
    }
};

int main() {
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
            } else {
                cout << "Nu s-a putut gasi o ruta disponibila la aceasta ora.\n";
            }
        }
    }
    cout << "Afisare Finala: \n";
    retea.afisareStatisticiTrenuri();


    return 0;
}
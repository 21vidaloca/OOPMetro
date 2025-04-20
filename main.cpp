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
    const string& getId() const { return id; }
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

    bool trebuieOprireLaStatia(const string& numeStatie) const {
        if (!oprireStatiiSelectate) return true;

        for (const auto& statie : statiiDeOprire) {
            if (statie == numeStatie) return true;
        }
        return false;
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
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        int oraCurenta = localTime->tm_hour;

        bool esteInInterval = false;
        if (oraStart < oraStop) {
            // Interval normal (ex: 22:00 - 6:00)
            esteInInterval = (oraCurenta >= oraStart && oraCurenta < oraStop);
        } else {
            // Interval peste miezul noptii (ex: 22:00 - 6:00)
            esteInInterval = (oraCurenta >= oraStart || oraCurenta < oraStop);
        }

        if (esteInInterval) {
            double timpOre = distanta / (getViteza() * factorViteza);
            return timpOre * 60.0;
        } else {
            // Timpul standard in afara programului de noapte
            return Tren::calculeazaTimpParcurgere(distanta);
        }
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

    bool esteInProgram(int ora) const {
        if (oraStart < oraStop) {
            return ora >= oraStart && ora < oraStop;
        } else {
            return ora >= oraStart || ora < oraStop;
        }
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

    double calculeazaConsumEnergie(double distanta) const {
        // Consum de baza in kWh/km
        double consumBaza = 15.0;

        // Aplicare eficienta
        double consumReal = consumBaza * (1.0 - eficientaEnergetica / 100.0);

        // In modul eco, consumul scade
        if (modEco) {
            consumReal *= 0.7; // reducere cu 30%
        }

        return consumReal * distanta;
    }

    double calculeazaEficienta() const override {
        // Pentru trenul electric, eficienta este determinata de eficienta energetica
        // si de faptul ca este sau nu in modul eco
        double eficientaModificare = modEco ? 1.2 : 1.0; // Modul eco creste eficienta cu 20%
        return eficientaEnergetica * eficientaModificare / 100.0;
    }

    void setModEco(bool eco) {
        modEco = eco;
    }

    double getAutonomieRamasaKm(double consumMediuPeKm) const {
        // Calculeaza cati km mai poate parcurge cu bateria actuala
        double energieDisponibila = autonomieBaterie * consumMediuPeKm / 60.0;
        return energieDisponibila / (modEco ? consumMediuPeKm * 0.7 : consumMediuPeKm);
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

    bool calculeazaRutaIntreStatii(const string& start, const string& destinatie,
                                  double& timpCalatorie, vector<string>& rutaStatii) const {
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

        timpCalatorie = tren->calculeazaTimpParcurgere(distanta);

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
    static int numarRetele;
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
    explicit ReteaMetrou(const string& nume = "Metrou Bucuresti") : numeRetea(nume) {
        numarRetele++;
    }

    // Destructor virtual
    virtual ~ReteaMetrou() {
        numarRetele--;
    }

    // Constructor de copiere
    ReteaMetrou(const ReteaMetrou& other) : numeRetea(other.numeRetea) {
        // Deep copy pentru trasee
        for (const auto& traseu : other.trasee) {
            trasee.push_back(make_shared<Traseu>(*traseu));
        }
        numarRetele++;
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

    bool calculeazaRutaOptima(const string& start, const string& destinatie,
                            double& timpTotal, vector<string>& rutaCompleta) const {
        if (!existaStatieInRetea(start) || !existaStatieInRetea(destinatie)) {
            cout << "Eroare: Una dintre statii nu exista in retea." << endl;
            return false;
        }

        // Cazul simplu: ambele statii sunt pe acelasi traseu
        for (const auto& traseu : trasee) {
            if (traseu->contineStatia(start) && traseu->contineStatia(destinatie)) {
                vector<string> rutaStatii;
                double timp = 0;

                bool success = traseu->calculeazaRutaIntreStatii(start, destinatie, timp, rutaStatii);
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
            const vector<shared_ptr<Statia>>& statii = traseu->getStatii();
            shared_ptr<Tren> tren = traseu->getTren();

            for (size_t i = 0; i < statii.size(); i++) {
                const string& statieActuala = statii[i]->getNume();

                if (i > 0) {
                    string statiePrecedenta = statii[i-1]->getNume();
                    double distanta = statii[i-1]->getDistantaUrmatoareStatie();
                    double timp = tren->calculeazaTimpParcurgere(distanta) + statii[i-1]->getTimpAsteptare() / 60.0;

                    graf[statieActuala].push_back({statiePrecedenta, timp});
                    graf[statiePrecedenta].push_back({statieActuala, timp});
                }
            }
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

        cout << "Eroare: Nu s-a putut calcula o ruta intre " << start << " si " << destinatie << endl;
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

    // Functie statica
    static int getNumarRetele() {
        return numarRetele;
    }

    // Getter pentru nume
    const string& getNumeRetea() const {
        return numeRetea;
    }
};

// Initializare membri statici
int ReteaMetrou::numarRetele = 0;

// Clasa derivata pentru retele metropolitane
class ReteaMetrouExtinsa : public ReteaMetrou {
private:
    int numarLiniiRapide;
    double lungimeZonaMetropolitana;

public:
    ReteaMetrouExtinsa(const string& nume, int numarLiniiRapide, double lungimeZonaMetropolitana)
        : ReteaMetrou(nume), numarLiniiRapide(numarLiniiRapide), lungimeZonaMetropolitana(lungimeZonaMetropolitana) {}

    void afisareTrasee() const override {
        ReteaMetrou::afisareTrasee();
        cout << "Informatii suplimentare pentru reteaua extinsa:\n";
        cout << "Numar linii rapide: " << numarLiniiRapide << "\n";
        cout << "Lungime zona metropolitana: " << lungimeZonaMetropolitana << " km\n";
    }

    // Getteri
    int getNumarLiniiRapide() const { return numarLiniiRapide; }
    double getLungimeZonaMetropolitana() const { return lungimeZonaMetropolitana; }
};

int main() {
    // Creare retea de metrou
    ReteaMetrou retea("Metrou Bucuresti");

    // Creare trenuri folosind smart pointers - obiecte derivate din clasa Tren
    auto tren1 = make_shared<TrenRapid>("M1-R", 80, 25, true);
    auto tren2 = make_shared<TrenNoapte>("M2-N", 70, 22, 6, 0.8);
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

    cout << "Numar de retele de metrou active: " << ReteaMetrou::getNumarRetele() << "\n\n";

    retea.afisareTrasee();

    cout << "--------------------\n";

    // 1. Calcularea timpului total intre doua statii
    cout << "\n1. Calculare timp intre doua statii:\n";
    double timpCalatorie = 0;
    vector<string> rutaStatii;

    if (traseul2->calculeazaRutaIntreStatii("Piata Unirii", "Piata Victoriei", timpCalatorie, rutaStatii)) {
        cout << "Ruta de la Piata Unirii la Victoriei:\n";
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
    }

    // 2. Informatii despre retea
    cout << "\n";
    cout << "2. Informatii despre retea:\n";
    cout << "Numar total de statii: " << retea.numarTotalStatii() << "\n";
    cout << "Lungimea totala a retelei: " << retea.lungimeTotalaRetea() << " km \n";
    cout << "Statia cu cel mai mare timp de asteptare: " << retea.statiaAglomerata() << "\n";

    // 3. Calculare ruta intre doua statii
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

    // 4. Test pentru ReteaMetrouExtinsa
    cout << "\n4. Test pentru reteaua extinsa:\n";
    ReteaMetrouExtinsa reteaExtinsa("Metrou Bucuresti Extins", 2, 75.5);
    reteaExtinsa.adaugaTraseu(traseul1);
    reteaExtinsa.adaugaTraseu(traseul2);
    reteaExtinsa.afisareTrasee();

    // 5. Test pentru dynamic_cast
    cout << "\n5. Test pentru dynamic_cast:\n";
    ReteaMetrou* ptrRetea = &reteaExtinsa; // upcast
    ReteaMetrouExtinsa* ptrReteaExtinsa = dynamic_cast<ReteaMetrouExtinsa*>(ptrRetea); // downcast

    if (ptrReteaExtinsa) {
        cout << "Dynamic cast realizat cu succes!\n";
        ptrReteaExtinsa->afisareTrasee();
    } else {
        cout << "Dynamic cast a esuat!\n";
    }

    // 6. Test pentru erori
    cout << "\n6. Test pentru tratarea erorilor:\n";
    cout << "Incercare de calculare a unei rute cu o statie inexistenta...\n";
    if (!retea.calculeazaRutaOptima("Pantelimon", "StatieInexistenta", timpTotal, rutaCompleta)) {
        cout << "Operatiunea nu a putut fi efectuata.\n";
    }

    // 7. Test pentru clasele derivate din Tren
    cout << "\n7. Test pentru clasele derivate din Tren:\n";

    // Test TrenRapid
    auto trenRapid = make_shared<TrenRapid>("TR101", 120, 30,
                                         vector<string>{"Piata Unirii", "Eroilor", "Gara de Nord"});
    cout << "Informatii tren rapid:\n";
    trenRapid->afisare();
    cout << "\n";

    double distantaTest = 10.0; // 10 km
    cout << "Timp parcurgere pentru " << distantaTest << " km: "
         << trenRapid->calculeazaTimpParcurgere(distantaTest) << " minute\n";
    cout << "Oprire la Piata Unirii: "
         << (trenRapid->trebuieOprireLaStatia("Piata Unirii") ? "Da" : "Nu") << "\n";
    cout << "Oprire la Pipera: "
         << (trenRapid->trebuieOprireLaStatia("Pipera") ? "Da" : "Nu") << "\n";
    cout << "Eficienta trenului rapid: " << trenRapid->calculeazaEficienta() << "\n";

    // Test TrenNoapte
    auto trenNoapte = make_shared<TrenNoapte>("TN202", 60, 22, 5, 0.7);
    cout << "\nInformatii tren de noapte:\n";
    trenNoapte->afisare();
    cout << "\n";

    int oraCurenta = 23; // Exemplu de ora
    cout << "Tren operational la ora " << oraCurenta << ": "
         << (trenNoapte->esteInProgram(oraCurenta) ? "Da" : "Nu") << "\n";
    cout << "Timp parcurgere pentru " << distantaTest << " km (simulare ora 23): "
         << trenNoapte->calculeazaTimpParcurgere(distantaTest) << " minute\n";
    cout << "Eficienta trenului de noapte: " << trenNoapte->calculeazaEficienta() << "\n";

    // Test TrenElectric
    auto trenElectric = make_shared<TrenElectric>("TE303", 85, 92, 560, true);
    cout << "\nInformatii tren electric:\n";
    trenElectric->afisare();
    cout << "\n";

    cout << "Timp parcurgere pentru " << distantaTest << " km (mod eco): "
         << trenElectric->calculeazaTimpParcurgere(distantaTest) << " minute\n";

    double consumEnergetic = trenElectric->calculeazaConsumEnergie(distantaTest);
    cout << "Consum energetic pentru " << distantaTest << " km: "
         << consumEnergetic << " kWh\n";

    trenElectric->setModEco(false);
    cout << "Timp parcurgere pentru " << distantaTest << " km (mod normal): "
         << trenElectric->calculeazaTimpParcurgere(distantaTest) << " minute\n";

    double consumMediuPeKm = 2.5; // kWh/km
    cout << "Autonomie ramasa: "
         << trenElectric->getAutonomieRamasaKm(consumMediuPeKm) << " km\n";
    cout << "Eficienta trenului electric: " << trenElectric->calculeazaEficienta() << "\n";

    // 8. Test polimorfism prin pointer la clasa de baza
    cout << "\n8. Test polimorfism prin pointer la clasa de baza Tren:\n";
    vector<shared_ptr<Tren>> trenuriDiverse = {
        trenRapid, trenNoapte, trenElectric
    };

    for (const auto& t : trenuriDiverse) {
        cout << "\nAfisare polimorfica: ";
        t->afisare();
        cout << "\nTimp parcurgere polimorfic pentru " << distantaTest << " km: "
             << t->calculeazaTimpParcurgere(distantaTest) << " minute";
        cout << "\nEficienta polimorfica: " << t->calculeazaEficienta();
        cout << "\n";
    }

    return 0;
}
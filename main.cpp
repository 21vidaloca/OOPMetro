#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Statia {
    string nume;
    int timpAsteptare; // timp mediu de așteptare în secunde

public:
    Statia() : nume("Unknown"), timpAsteptare(0) {}
    Statia(const string& nume, int timpAsteptare) {
        this->nume = nume;
        this->timpAsteptare = timpAsteptare;
    }

    friend ostream& operator<<(ostream& os, const Statia& statie) {
        os << "Statia: " << statie.nume << " | Timp asteptare: " << statie.timpAsteptare << " sec";
        return os;
    }

    int getTimpAsteptare() const { return timpAsteptare; }

    /*const string& getNume() const {
        return nume;
    }*/
};

class Tren {
    string id;
    int capacitate;
    int viteza; // în km/h

public:
    Tren() : id("Unknown"), capacitate(0), viteza(0) {}

    Tren(const string& id, int capacitate, int viteza) : id(id), capacitate(capacitate), viteza(viteza) {}

    friend ostream& operator<<(ostream& os, const Tren& tren) {
        os << "Tren ID: " << tren.id << " | Capacitate: " << tren.capacitate
           << " | Viteza: " << tren.viteza << " km/h";
        return os;
    }

    int getViteza() const {
        return viteza;
    }
};

class Traseu {
    vector<Statia> statii;
    Tren tren;

public:
    explicit Traseu(const Tren& tren) : tren(tren) {}

    // Constructor de copiere corect
    Traseu(const Traseu& other) : statii(other.statii),tren(other.tren)  {}//

    // Operator= corect
    Traseu& operator=(const Traseu& other) {
        if (this != &other) {
            tren = other.tren;
            statii = other.statii;
        }
        return *this;
    }

    ~Traseu() {}

    void adaugaStatia(const Statia& statie) {
        statii.push_back(statie);
    }

    double calculeazaTimpTotal() const {
        double timp = 0;
        for (const auto& s : statii) {
            timp += s.getTimpAsteptare();
        }
        return timp;
    }

    friend ostream& operator<<(ostream& os, const Traseu& traseu) {
        os << "Traseu pentru " << traseu.tren << "\nStatii:\n";
        for (const auto& s : traseu.statii) {
            os << "- " << s << "\n";
        }
        return os;
    }
};

class ReteaMetrou {
    vector<Traseu> trasee;

public:
    void adaugaTraseu(const Traseu& traseu) {
        trasee.push_back(traseu);
    }

    void afisareTrasee() const {
        for (const auto& traseu : trasee) {
            cout << traseu << "\n";
        }
    }
};

int main() {
    ReteaMetrou retea;
    Tren tren1("M1", 300, 80);

    Traseu traseu1(tren1);
    traseu1.adaugaStatia(Statia("Piata Unirii", 30));
    traseu1.adaugaStatia(Statia("Universitate", 25));
    traseu1.adaugaStatia(Statia("Victoriei", 35));

    retea.adaugaTraseu(traseu1);

    cout << "Lista traseelor de metrou:\n";
    retea.afisareTrasee();

    cout << "Timp total pe traseu: " << traseu1.calculeazaTimpTotal() << " sec\n";
    cout<<tren1.getViteza()<<"\n";

    return 0;
}

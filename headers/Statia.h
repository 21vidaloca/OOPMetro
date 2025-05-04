#ifndef STATIA_H
#define STATIA_H

#include <iostream>
#include <string>

class Statia {
private:
    std::string nume;
    int timpAsteptare;
    int distantaUrmatoareStatie;

public:
    // Constructor
    Statia(const std::string& nume, int timpAsteptare, int distantaUrmatoareStatie = 2);

    // Constructor de copiere
    Statia(const Statia& other);

    // Operator de atribuire
    Statia& operator=(const Statia& other);

    // Metoda pentru afisare
    void afisare() const;

    // Getteri
    const std::string& getNume() const;
    int getTimpAsteptare() const;
    int getDistantaUrmatoareStatie() const;

    // Operatori de comparare
    bool operator==(const Statia& other) const;
    bool operator==(const std::string& numeStatie) const;

    // Operator pentru afisare
    friend std::ostream& operator<<(std::ostream& os, const Statia& statie);
};

#endif // STATIA_H
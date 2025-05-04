#include "../headers/Statia.h"

Statia::Statia(const std::string& nume, int timpAsteptare, int distantaUrmatoareStatie)
    : nume(nume), timpAsteptare(timpAsteptare), distantaUrmatoareStatie(distantaUrmatoareStatie) {}

Statia::Statia(const Statia& other)
    : nume(other.nume), timpAsteptare(other.timpAsteptare),
      distantaUrmatoareStatie(other.distantaUrmatoareStatie) {}

Statia& Statia::operator=(const Statia& other) {
    if (this != &other) {
        nume = other.nume;
        timpAsteptare = other.timpAsteptare;
        distantaUrmatoareStatie = other.distantaUrmatoareStatie;
    }
    return *this;
}

void Statia::afisare() const {
    std::cout << "Statia: " << nume << " | Timp asteptare: " << timpAsteptare
              << " sec | Distanta pana la urmatoarea statie: " << distantaUrmatoareStatie << " km";
}

const std::string& Statia::getNume() const { return nume; }

int Statia::getTimpAsteptare() const { return timpAsteptare; }

int Statia::getDistantaUrmatoareStatie() const { return distantaUrmatoareStatie; }

bool Statia::operator==(const Statia& other) const {
    return nume == other.nume;
}

bool Statia::operator==(const std::string& numeStatie) const {
    return nume == numeStatie;
}

std::ostream& operator<<(std::ostream& os, const Statia& statie) {
    statie.afisare();
    return os;
}
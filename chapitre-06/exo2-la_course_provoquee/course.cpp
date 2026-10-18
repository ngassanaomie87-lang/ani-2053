#include <iostream>
#include <vector>
#include "NkThread.h"

// Variable globale partagée
long long compteur = 0;

void incrementer() {
    for (int i = 0; i < 1000000; ++i) {
        compteur += 1; // Incrémentation sans verrou ni protection
    }
}

int main() {
    const int NB_THREADS = 8;
    const long long TOTAL_ATTENDU = 8LL * 1000000LL;

    compteur = 0;
    std::vector<NkThread> threads;
    threads.reserve(NB_THREADS);

    // Lancement de 8 fils
    for (int i = 0; i < NB_THREADS; ++i) {
        threads.emplace_back(incrementer);
    }

    // Joindre tous les fils
    for (auto& t : threads) {
        t.Join();
    }

    std::cout << "Total obtenu : " << compteur << std::endl;
    std::cout << "Total attendu : " << TOTAL_ATTENDU << std::endl;

    return 0;
}
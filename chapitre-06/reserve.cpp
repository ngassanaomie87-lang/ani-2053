#include <iostream>
#include <vector>
#include "NkThreadPool.h"
#include "NkThread.h"
#include "NkChrono.h"

// 1. Définition des tâches (courte et longue)
void tacheCourte() {
    volatile int somme = 0;
    for (int i = 0; i < 1000; ++i) {
        somme += i;
    }
}

void tacheLongue() {
    volatile int somme = 0;
    for (int i = 0; i < 100000; ++i) {
        somme += i;
    }
}

int main() {
    const int NB_TACHES = 200;

    // --- MESURE TÂCHE COURTE ---
    std::cout << "--- MESURES TÂCHE COURTE ---" << std::endl;

    // Version Réserve (3 fois)
    for (int run = 1; run <= 3; ++run) {
        NkThreadPool pool;
        NkChrono chrono;
        
        for (int i = 0; i < NB_TACHES; ++i) {
            pool.Enqueue(tacheCourte);
        }
        pool.Join();

        std::cout << "Réserve Run " << run << " : " 
                  << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    // Version Fils créés à la main (3 fois)
    for (int run = 1; run <= 3; ++run) {
        NkChrono chrono;
        std::vector<NkThread> threads;
        threads.reserve(NB_TACHES);

        for (int i = 0; i < NB_TACHES; ++i) {
            threads.emplace_back(tacheCourte);
        }
        for (auto& t : threads) {
            t.Join();
        }

        std::cout << "Fils Run " << run << " : " 
                  << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    // Affichage du nombre d'ouvriers
    NkThreadPool poolTest;
    std::cout << "Nombre d'ouvriers : " << poolTest.GetNumWorkers() << std::endl;


    // --- MESURE TÂCHE LONGUE ---
    std::cout << "\n--- MESURES TÂCHE LONGUE ---" << std::endl;

    // Version Réserve
    {
        NkThreadPool pool;
        NkChrono chrono;
        for (int i = 0; i < NB_TACHES; ++i) {
            pool.Enqueue(tacheLongue);
        }
        pool.Join();
        std::cout << "Réserve (Tâche longue) : " 
                  << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    // Version Fils
    {
        NkChrono chrono;
        std::vector<NkThread> threads;
        threads.reserve(NB_TACHES);

        for (int i = 0; i < NB_TACHES; ++i) {
            threads.emplace_back(tacheLongue);
        }
        for (auto& t : threads) {
            t.Join();
        }
        std::cout << "Fils (Tâche longue) : " 
                  << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    return 0;
}
#include <iostream>
#include <vector>
#include "NkThread.h"
#include "NkMutex.h"
#include "NkScopedLock.h"
#include "NKCore/NkAtomic.h"
#include "NkChrono.h"

long long compteurSansProtection = 0;
long long compteurMutex = 0;
NkMutex mutexGlobal;

NkAtomicInt compteurAtomique(0);

const int NB_THREADS = 8;
const int NB_INCREMENTS = 1000000;

void tacheSansProtection() {
    for (int i = 0; i < NB_INCREMENTS; ++i) {
        compteurSansProtection += 1;
    }
}

void tacheMutex() {
    for (int i = 0; i < NB_INCREMENTS; ++i) {
        NkScopedLock<NkMutex> lock(mutexGlobal);
        compteurMutex += 1;
    }
}

void tacheAtomique() {
    for (int i = 0; i < NB_INCREMENTS; ++i) {
        compteurAtomique.FetchAdd(1);
    }
}

void tacheLocale(long long* resultatLocal) {
    long long local = 0;
    for (int i = 0; i < NB_INCREMENTS; ++i) {
        local += 1;
    }
    *resultatLocal = local;
}

int main() {
    // 1. Sans protection
    {
        compteurSansProtection = 0;
        NkChrono chrono;
        std::vector<NkThread> threads;
        threads.reserve(NB_THREADS);
        for (int i = 0; i < NB_THREADS; ++i) {
            threads.emplace_back(tacheSansProtection);
        }
        for (auto& t : threads) { t.Join(); }
        std::cout << "Sans protection - Total: " << compteurSansProtection << " Temps: " << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    // 2. Version mutex
    {
        compteurMutex = 0;
        NkChrono chrono;
        std::vector<NkThread> threads;
        threads.reserve(NB_THREADS);
        for (int i = 0; i < NB_THREADS; ++i) {
            threads.emplace_back(tacheMutex);
        }
        for (auto& t : threads) { t.Join(); }
        std::cout << "Mutex - Total: " << compteurMutex << " Temps: " << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    // 3. Version atomique
    {
        compteurAtomique = 0;
        NkChrono chrono;
        std::vector<NkThread> threads;
        threads.reserve(NB_THREADS);
        for (int i = 0; i < NB_THREADS; ++i) {
            threads.emplace_back(tacheAtomique);
        }
        for (auto& t : threads) { t.Join(); }
        std::cout << "Atomique - Total: " << compteurAtomique.Get() << " Temps: " << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    // 4. Version locale
    {
        NkChrono chrono;
        std::vector<long long> totauxLocaux(NB_THREADS, 0);
        std::vector<NkThread> threads;
        threads.reserve(NB_THREADS);
        for (int i = 0; i < NB_THREADS; ++i) {
            threads.emplace_back(tacheLocale, &totauxLocaux[i]);
        }
        for (auto& t : threads) { t.Join(); }
        long long totalFinal = 0;
        for (long long val : totauxLocaux) { totalFinal += val; }
        std::cout << "Local - Total: " << totalFinal << " Temps: " << chrono.Elapsed().milliseconds() << " ms" << std::endl;
    }

    return 0;
}
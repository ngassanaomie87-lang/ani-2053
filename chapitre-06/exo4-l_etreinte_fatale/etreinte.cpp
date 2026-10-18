#include <iostream>
#include "NkThread.h"
#include "NkMutex.h"
#include "NkScopedLock.h"
#include "NkChrono.h"

NkMutex m1;
NkMutex m2;

/* ==========================================
   VERSION QUI SE FIGE (INTERBLOCAGE / DEADLOCK)
   À garder en commentaires au-dessus :
   
void filA_bloque() {
    NkScopedLock<NkMutex> lock1(m1);
    NkChrono::SleepMilliseconds(100);
    NkScopedLock<NkMutex> lock2(m2); // Attend m2 détenu par fil B
}

void filB_bloque() {
    NkScopedLock<NkMutex> lock2(m2);
    NkChrono::SleepMilliseconds(100);
    NkScopedLock<NkMutex> lock1(m1); // Attend m1 détenu par fil A
}
   ========================================== */

// VERSION CORRIGÉE (même ordre de prise des verrous : toujours m1 puis m2)
void filA() {
    NkScopedLock<NkMutex> lock1(m1);
    NkChrono::SleepMilliseconds(100);
    NkScopedLock<NkMutex> lock2(m2);
}

void filB() {
    NkScopedLock<NkMutex> lock1(m1); // Corrigé : prend m1 en premier aussi
    NkChrono::SleepMilliseconds(100);
    NkScopedLock<NkMutex> lock2(m2); // puis m2
}

int main() {
    std::cout << "Lancement du programme..." << std::endl;

    NkThread tA(filA);
    NkThread tB(filB);

    tA.Join();
    tB.Join();

    std::cout << "Programme terminé avec succès !" << std::endl;
    return 0;
}
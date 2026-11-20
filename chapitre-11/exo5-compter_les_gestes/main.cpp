#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Tache {
    string nom;
    long long minimum = 0;
    long long parJour = 0;
    long long debut = 0;
    long long gestes = 0;
    long long attente = 0;
};

static string formaterTemps(long long secondes) {
    ostringstream sortie;
    sortie << secondes / 60 << ':' << setw(2) << setfill('0') << secondes % 60;
    return sortie.str();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Tache tache;
    bool enCours = false;
    long long totalJour = 0;
    string ligne;

    while (getline(cin, ligne)) {
        if (ligne.empty()) {
            continue;
        }

        istringstream entree(ligne);
        long long instant = 0;
        string operation;
        entree >> instant >> operation;

        if (operation == "DEBUT") {
            entree >> tache.nom >> tache.minimum >> tache.parJour;
            tache.debut = instant;
            tache.gestes = 0;
            tache.attente = 0;
            enCours = true;
        } else if (operation == "FIN") {
            string nom;
            entree >> nom;

            if (enCours) {
                const long long enTrop =
                    tache.gestes > tache.minimum ? tache.gestes - tache.minimum : 0;
                const long long duree = instant - tache.debut;
                const long long jourSecondes = tache.attente * tache.parJour / 1000;

                cout << tache.nom << " GESTES " << tache.gestes << " EN_TROP " << enTrop
                     << " DUREE " << duree << " ATTENTE " << tache.attente
                     << " JOUR " << formaterTemps(jourSecondes) << '\n';
                totalJour += jourSecondes;
                enCours = false;
            }
        } else if (operation == "ATTENTE") {
            long long duree = 0;
            entree >> duree;
            if (enCours) {
                tache.attente += duree;
            }
        } else if (operation == "CLIC" || operation == "TOUCHE" ||
                   operation == "MOLETTE" || operation == "GLISSE") {
            if (enCours) {
                ++tache.gestes;
            }
        }
    }

    if (enCours) {
        cout << tache.nom << " INACHEVEE\n";
    }

    cout << "TOTAL_JOUR " << formaterTemps(totalJour) << '\n';
    return 0;
}
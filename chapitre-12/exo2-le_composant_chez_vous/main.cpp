#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string commande;
    string document;
    long long intervalle = 0;
    cin >> commande >> intervalle >> commande >> document;

    const string nomSecours = document == "-"
        ? "planche-sans-nom.nkref.auto"
        : document + ".auto";

    long long instant = 0;
    long long tempsEcoule = 0;
    long long sauvegardes = 0;
    long long echecs = 0;
    bool change = false;
    bool disqueOK = true;
    vector<string> sorties;

    while (cin >> commande) {
        if (commande == "TOUCH") {
            change = true;
        } else if (commande == "TICK") {
            long long duree = 0;
            cin >> duree;
            if (duree > 0) {
                instant += duree;
                tempsEcoule += duree;
            }

            if (change && tempsEcoule >= intervalle) {
                if (disqueOK) {
                    sorties.push_back(to_string(instant) + " SECOURS");
                    ++sauvegardes;
                    change = false;
                    tempsEcoule = 0;
                } else {
                    sorties.push_back(to_string(instant) + " ECHEC");
                    ++echecs;
                }
            }
        } else if (commande == "CTRL_S") {
            if (disqueOK) {
                sorties.push_back(to_string(instant) + " MANUELLE");
                change = false;
                tempsEcoule = 0;
            } else {
                sorties.push_back(to_string(instant) + " MANUELLE_ECHEC");
                ++echecs;
            }
        } else if (commande == "DISQUE") {
            string etat;
            cin >> etat;
            disqueOK = etat == "OK";
        }
    }

    for (const string& sortie : sorties) {
        cout << sortie << '\n';
    }
    cout << "SAUVEGARDES " << sauvegardes << '\n';
    cout << "ECHECS " << echecs << '\n';
    cout << "EN_ATTENTE " << (change ? "OUI" : "NON") << '\n';
    cout << "SECOURS_NOM " << nomSecours << '\n';
    return 0;
}

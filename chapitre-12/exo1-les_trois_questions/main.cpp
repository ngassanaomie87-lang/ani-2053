#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Composant {
    string nom;
    string lieu;
    string touche;
    string filet;
    bool aRevoir = false;
    bool filetAposer = false;
    bool pret = false;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string etiquette;
    size_t nombreIntouchables = 0;
    cin >> etiquette >> nombreIntouchables;

    unordered_set<string> intouchables;
    intouchables.reserve(nombreIntouchables);
    for (size_t i = 0; i < nombreIntouchables; ++i) {
        string partie;
        cin >> partie;
        intouchables.insert(partie);
    }

    size_t nombreComposants = 0;
    cin >> nombreComposants;

    vector<Composant> composants;
    composants.reserve(nombreComposants);
    size_t purs = 0;
    size_t aRevoir = 0;
    size_t filetsAposer = 0;

    for (size_t i = 0; i < nombreComposants; ++i) {
        Composant composant;
        size_t nombreBesoins = 0;
        cin >> composant.nom >> nombreBesoins;

        bool besoinGlue = false;
        bool besoinAction = false;
        for (size_t j = 0; j < nombreBesoins; ++j) {
            string besoin;
            cin >> besoin;
            if (besoin == "FENETRE" || besoin == "SOURIS" ||
                besoin == "CLAVIER" || besoin == "GPU") {
                besoinGlue = true;
            } else if (besoin == "DISQUE" || besoin == "HORLOGE" ||
                       besoin == "RESEAU") {
                besoinAction = true;
            }
        }

        size_t nombreParties = 0;
        cin >> nombreParties;
        vector<string> partiesModifiees;
        partiesModifiees.reserve(nombreParties);
        for (size_t j = 0; j < nombreParties; ++j) {
            string partie;
            cin >> partie;
            partiesModifiees.push_back(partie);
        }

        size_t nombreTests = 0;
        cin >> nombreTests;

        if (besoinGlue) {
            composant.lieu = "GLUE";
        } else if (besoinAction) {
            composant.lieu = "DECOUPER";
        } else {
            composant.lieu = "FICHIER_PUR";
            ++purs;
        }

        composant.touche = "INTACT";
        for (const string& partie : partiesModifiees) {
            if (intouchables.count(partie) != 0) {
                composant.touche = "REVOIR " + partie;
                composant.aRevoir = true;
                ++aRevoir;
                break;
            }
        }

        if (nombreTests == 0) {
            composant.filet = "FILET_D_ABORD";
            composant.filetAposer = true;
            ++filetsAposer;
        } else {
            composant.filet = "FILET_EXISTANT";
        }

        composant.pret = !composant.aRevoir && !composant.filetAposer;
        composants.push_back(composant);
    }

    vector<string> prets;
    for (const Composant& composant : composants) {
        cout << composant.nom << ' ' << composant.lieu << ' '
             << composant.touche << ' ' << composant.filet << '\n';
        if (composant.pret) {
            prets.push_back(composant.nom);
        }
    }

    cout << "PURS " << purs << '\n';
    cout << "A_REVOIR " << aRevoir << '\n';
    cout << "FILETS_A_POSER " << filetsAposer << '\n';
    cout << "PRETS";
    if (prets.empty()) {
        cout << " AUCUN";
    } else {
        for (const string& nom : prets) {
            cout << ' ' << nom;
        }
    }
    cout << '\n';

    return 0;
}

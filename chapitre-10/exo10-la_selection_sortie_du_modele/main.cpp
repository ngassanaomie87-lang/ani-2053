#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct Image {
    string nom;
    bool choisie = false;
};

struct Conception {
    vector<Image> planche;
    set<size_t> selectionParIndex;
    set<string> selectionParId;
};

static void afficherListe(const vector<string>& noms) {
    if (noms.empty()) {
        cout << " AUCUN";
        return;
    }

    for (const string& nom : noms) {
        cout << ' ' << nom;
    }
}

static vector<string> nomsPlanche(const Conception& conception) {
    vector<string> noms;
    noms.reserve(conception.planche.size());
    for (const Image& image : conception.planche) {
        noms.push_back(image.nom);
    }
    return noms;
}

static vector<string> nomsSelection(const Conception& conception, int type) {
    vector<string> noms;
    for (size_t i = 0; i < conception.planche.size(); ++i) {
        bool choisie = false;
        if (type == 0) {
            choisie = conception.planche[i].choisie;
        } else if (type == 1) {
            choisie = conception.selectionParIndex.count(i) != 0;
        } else {
            choisie = conception.selectionParId.count(conception.planche[i].nom) != 0;
        }

        if (choisie) {
            noms.push_back(conception.planche[i].nom);
        }
    }
    return noms;
}

static void afficherConception(const string& nom, const Conception& conception, int type) {
    cout << nom << " PLANCHE";
    afficherListe(nomsPlanche(conception));
    cout << '\n';
    cout << nom << " SELECTION";
    afficherListe(nomsSelection(conception, type));
    cout << '\n';
}

static bool identique(const Conception& gauche, int typeGauche,
                      const Conception& droite, int typeDroite) {
    return nomsPlanche(gauche) == nomsPlanche(droite) &&
           nomsSelection(gauche, typeGauche) == nomsSelection(droite, typeDroite);
}

static void cliquer(Conception& conception, int type, long long indice, bool ajouter) {
    if (!ajouter) {
        if (type == 0) {
            for (Image& image : conception.planche) {
                image.choisie = false;
            }
        } else if (type == 1) {
            conception.selectionParIndex.clear();
        } else {
            conception.selectionParId.clear();
        }
    }

    if (indice < 0 || static_cast<size_t>(indice) >= conception.planche.size()) {
        return;
    }

    const size_t index = static_cast<size_t>(indice);
    if (type == 0) {
        conception.planche[index].choisie = true;
    } else if (type == 1) {
        conception.selectionParIndex.insert(index);
    } else {
        conception.selectionParId.insert(conception.planche[index].nom);
    }
}

static void supprimerSelection(Conception& conception, int type) {
    vector<Image> restantes;
    restantes.reserve(conception.planche.size());
    for (size_t i = 0; i < conception.planche.size(); ++i) {
        bool choisie = false;
        if (type == 0) {
            choisie = conception.planche[i].choisie;
        } else if (type == 1) {
            choisie = conception.selectionParIndex.count(i) != 0;
        } else {
            choisie = conception.selectionParId.count(conception.planche[i].nom) != 0;
        }
        if (!choisie) {
            restantes.push_back(conception.planche[i]);
        }
    }
    conception.planche.swap(restantes);
    conception.selectionParIndex.clear();
    conception.selectionParId.clear();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Conception conceptions[3];
    string operation;
    while (cin >> operation) {
        if (operation == "AJOUTE") {
            string nom;
            cin >> nom;
            for (Conception& conception : conceptions) {
                conception.planche.push_back({nom, false});
            }
        } else if (operation == "CLIC" || operation == "AJOUTE_SEL") {
            long long indice = 0;
            cin >> indice;
            const bool ajouter = operation == "AJOUTE_SEL";
            for (int type = 0; type < 3; ++type) {
                cliquer(conceptions[type], type, indice, ajouter);
            }
        } else if (operation == "ECHANGE") {
            long long i = 0;
            long long j = 0;
            cin >> i >> j;
            for (Conception& conception : conceptions) {
                if (i >= 0 && j >= 0 &&
                    static_cast<size_t>(i) < conception.planche.size() &&
                    static_cast<size_t>(j) < conception.planche.size()) {
                    swap(conception.planche[static_cast<size_t>(i)],
                         conception.planche[static_cast<size_t>(j)]);
                }
            }
        } else if (operation == "RETIRE") {
            long long indice = 0;
            cin >> indice;
            for (Conception& conception : conceptions) {
                if (indice >= 0 && static_cast<size_t>(indice) < conception.planche.size()) {
                    conception.planche.erase(conception.planche.begin() + indice);
                }
            }
        } else if (operation == "SUPPRIME") {
            for (int type = 0; type < 3; ++type) {
                supprimerSelection(conceptions[type], type);
            }
        }
    }

    afficherConception("DANS_ITEM", conceptions[0], 0);
    afficherConception("PAR_INDEX", conceptions[1], 1);
    afficherConception("PAR_ID", conceptions[2], 2);

    cout << "FAUX";
    bool auMoinsUneFausse = false;
    for (int type = 1; type < 3; ++type) {
        if (!identique(conceptions[0], 0, conceptions[type], type)) {
            cout << ' ' << (type == 1 ? "PAR_INDEX" : "PAR_ID");
            auMoinsUneFausse = true;
        }
    }
    if (!auMoinsUneFausse) {
        cout << " AUCUN";
    }
    cout << '\n';

    return 0;
}
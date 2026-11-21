#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Entree {
    string nom;
    int valeur = 0;
    int cout = 0;
    string etiquette;
    string genre;
    string cible;
    string raison;
    string categorie;
    size_t ordre = 0;
    bool gardee = false;
};

static int rangEtquette(const string& etiquette) {
    if (etiquette == "MESUREE") {
        return 0;
    }
    if (etiquette == "OBSERVEE") {
        return 1;
    }
    return 2;
}

static int rangCase(const Entree& entree) {
    const bool valeurForte = entree.valeur >= 3;
    const bool coutEleve = entree.cout >= 3;

    if (valeurForte && !coutEleve) {
        return 0;
    }
    if (valeurForte && coutEleve) {
        return 1;
    }
    if (!valeurForte && !coutEleve) {
        return 2;
    }
    return 3;
}

static const char* nomCase(int rang) {
    switch (rang) {
        case 0:
            return "MAINTENANT";
        case 1:
            return "DECOUPER";
        case 2:
            return "EN_PASSANT";
        default:
            return "ON_NE_LE_FAIT_PAS";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t nombreEntrees = 0;
    cin >> nombreEntrees;

    vector<Entree> entrees;
    entrees.reserve(nombreEntrees);

    for (size_t i = 0; i < nombreEntrees; ++i) {
        Entree entree;
        cin >> entree.nom >> entree.valeur >> entree.cout >> entree.etiquette >> entree.genre;
        entree.ordre = i;
        if (entree.genre == "SOCLE") {
            cin >> entree.cible;
        }

        if (entree.genre == "REECRITURE") {
            entree.raison = "reecriture";
        } else if (entree.genre == "CHOIX") {
            entree.raison = "choix";
        } else if (entree.etiquette == "-") {
            entree.raison = "sans_etiquette";
        } else {
            entree.gardee = true;
        }

        entrees.push_back(entree);
    }

    unordered_map<string, size_t> indexParNom;
    indexParNom.reserve(entrees.size());
    for (size_t i = 0; i < entrees.size(); ++i) {
        indexParNom.emplace(entrees[i].nom, i);
    }

    for (Entree& entree : entrees) {
        if (!entree.gardee || entree.genre != "SOCLE") {
            continue;
        }

        const auto trouve = indexParNom.find(entree.cible);
        if (trouve != indexParNom.end()) {
            const Entree& cible = entrees[trouve->second];
            if (cible.gardee && cible.genre == "MANQUE") {
                entree.valeur = max(entree.valeur, cible.valeur);
            }
        }
    }

    vector<Entree> gardees;
    gardees.reserve(entrees.size());
    for (const Entree& entree : entrees) {
        if (entree.gardee) {
            gardees.push_back(entree);
        }
    }

    sort(gardees.begin(), gardees.end(), [](const Entree& gauche, const Entree& droite) {
        const int caseGauche = rangCase(gauche);
        const int caseDroite = rangCase(droite);
        if (caseGauche != caseDroite) {
            return caseGauche < caseDroite;
        }

        const int etiquetteGauche = rangEtquette(gauche.etiquette);
        const int etiquetteDroite = rangEtquette(droite.etiquette);
        if (etiquetteGauche != etiquetteDroite) {
            return etiquetteGauche < etiquetteDroite;
        }
        if (gauche.valeur != droite.valeur) {
            return gauche.valeur > droite.valeur;
        }
        if (gauche.cout != droite.cout) {
            return gauche.cout < droite.cout;
        }
        return gauche.ordre < droite.ordre;
    });

    bool honnete = false;
    for (const Entree& entree : gardees) {
        const int rang = rangCase(entree);
        cout << nomCase(rang) << ' ' << entree.nom << ' ' << entree.etiquette << '\n';
        if (rang == 3) {
            honnete = true;
        }
    }

    for (const Entree& entree : entrees) {
        if (!entree.gardee) {
            cout << "HORS_LISTE " << entree.nom << ' ' << entree.raison << '\n';
        }
    }

    cout << "VERDICT " << (honnete ? "HONNETE" : "COMPLAISANT") << '\n';
    return 0;
}
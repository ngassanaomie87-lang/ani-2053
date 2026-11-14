#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static bool contient(const string& ligne, const string& nom) {
    istringstream flux(ligne);
    string mot;
    while (flux >> mot) {
        if (mot == nom) {
            return true;
        }
    }
    return false;
}

static void echanger(vector<string>& tableau, size_t i, size_t j) {
    if (i < tableau.size() && j < tableau.size()) {
        swap(tableau[i], tableau[j]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string ligneAjout;
    string ligneSuppression;
    string ligneEchange;
    getline(cin, ligneAjout);
    getline(cin, ligneSuppression);
    getline(cin, ligneEchange);

    const bool ajouteTextures = contient(ligneAjout, "textures");
    const bool ajouteSources = contient(ligneAjout, "sources");
    const bool supprimeTextures = contient(ligneSuppression, "textures");
    const bool supprimeSources = contient(ligneSuppression, "sources");
    const bool echangeTextures = contient(ligneEchange, "textures");
    const bool echangeSources = contient(ligneEchange, "sources");

    int nombreOperations = 0;
    cin >> nombreOperations;

    vector<string> modele;
    vector<string> textures;
    vector<string> sources;

    for (int operation = 0; operation < nombreOperations; ++operation) {
        string type;
        cin >> type;

        if (type == "AJOUTE") {
            string nom;
            cin >> nom;
            modele.push_back(nom);
            if (ajouteTextures) {
                textures.push_back(nom);
            }
            if (ajouteSources) {
                sources.push_back(nom);
            }
        } else if (type == "SUPPRIME") {
            int nombreIndices = 0;
            cin >> nombreIndices;
            vector<size_t> indices(static_cast<size_t>(nombreIndices));
            for (size_t& indice : indices) {
                cin >> indice;
            }

            sort(indices.rbegin(), indices.rend());
            for (size_t indice : indices) {
                if (indice < modele.size()) {
                    modele.erase(modele.begin() + static_cast<ptrdiff_t>(indice));
                }
                if (supprimeTextures && indice < textures.size()) {
                    textures.erase(textures.begin() + static_cast<ptrdiff_t>(indice));
                }
                if (supprimeSources && indice < sources.size()) {
                    sources.erase(sources.begin() + static_cast<ptrdiff_t>(indice));
                }
            }
        } else if (type == "MONTE" || type == "DESCEND") {
            size_t indice = 0;
            cin >> indice;

            if (type == "MONTE") {
                if (indice + 1 < modele.size()) {
                    swap(modele[indice], modele[indice + 1]);
                    if (echangeTextures) {
                        echanger(textures, indice, indice + 1);
                    }
                    if (echangeSources) {
                        echanger(sources, indice, indice + 1);
                    }
                }
            } else if (indice > 0) {
                swap(modele[indice], modele[indice - 1]);
                if (echangeTextures) {
                    echanger(textures, indice, indice - 1);
                }
                if (echangeSources) {
                    echanger(sources, indice, indice - 1);
                }
            }
        }
    }

    const size_t longueur = max(modele.size(), max(textures.size(), sources.size()));
    size_t decales = 0;
    for (size_t i = 0; i < longueur; ++i) {
        const bool aModele = i < modele.size();
        const bool aTextures = i < textures.size();
        const bool aSources = i < sources.size();
        const string nomModele = aModele ? modele[i] : "-";
        const string nomTextures = aTextures ? textures[i] : "-";
        const string nomSources = aSources ? sources[i] : "-";

        cout << i << ' ' << nomModele << ' ' << nomTextures << ' ' << nomSources << '\n';
        if (!aModele || !aTextures || !aSources ||
            nomModele != nomTextures || nomModele != nomSources) {
            ++decales;
        }
    }

    cout << "DECALES " << decales << '\n';
    return 0;
}
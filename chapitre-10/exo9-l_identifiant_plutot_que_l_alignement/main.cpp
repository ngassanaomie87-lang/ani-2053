#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int nombreOperations = 0;
    cin >> nombreOperations;

    vector<uint64_t> ordre;
    uint64_t prochainIdentifiant = 1;
    uint64_t occasions = 0;
    uint64_t repercutes = 0;
    uint64_t recherches = 0;

    for (int operation = 0; operation < nombreOperations; ++operation) {
        string type;
        cin >> type;

        if (type == "AJOUTE") {
            ordre.push_back(prochainIdentifiant++);
        } else if (type == "SUPPRIME") {
            int compte = 0;
            cin >> compte;
            vector<size_t> indices(static_cast<size_t>(compte));
            for (size_t& indice : indices) {
                cin >> indice;
            }

            for (auto it = indices.rbegin(); it != indices.rend(); ++it) {
                ordre.erase(ordre.begin() + static_cast<ptrdiff_t>(*it));
            }
            ++occasions;
            repercutes += 2 * static_cast<uint64_t>(compte);
        } else if (type == "MONTE") {
            size_t indice = 0;
            cin >> indice;
            if (indice + 1 < ordre.size()) {
                swap(ordre[indice], ordre[indice + 1]);
                ++occasions;
                repercutes += 2;
            }
        } else if (type == "DESCEND") {
            size_t indice = 0;
            cin >> indice;
            if (indice > 0) {
                swap(ordre[indice], ordre[indice - 1]);
                ++occasions;
                repercutes += 2;
            }
        } else if (type == "DESSINE") {
            recherches += static_cast<uint64_t>(ordre.size());
        }
    }

    cout << "ORDRE";
    if (ordre.empty()) {
        cout << " AUCUN";
    } else {
        for (uint64_t identifiant : ordre) {
            cout << ' ' << identifiant;
        }
    }
    cout << '\n';
    cout << "OCCASIONS " << occasions << '\n';
    cout << "REPERCUTES " << repercutes << '\n';
    cout << "RECHERCHES " << recherches << '\n';

    return 0;
}

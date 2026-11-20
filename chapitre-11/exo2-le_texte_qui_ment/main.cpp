#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long nombreCommits = 0;
    cin >> nombreCommits;

    unordered_map<string, long long> indiceCommit;
    unordered_set<string> presentes;
    unordered_map<string, long long> dernierChangement;
    vector<string> hashes;
    hashes.reserve(static_cast<size_t>(nombreCommits));

    for (long long i = 0; i < nombreCommits; ++i) {
        string hash;
        string action;
        long long nombreFonctions = 0;
        cin >> hash >> action >> nombreFonctions;
        indiceCommit[hash] = i;
        hashes.push_back(hash);

        for (long long j = 0; j < nombreFonctions; ++j) {
            string fonction;
            cin >> fonction;

            if (action == "AJOUTE") {
                if (presentes.insert(fonction).second) {
                    dernierChangement[fonction] = i;
                }
            } else if (action == "RETIRE") {
                if (presentes.erase(fonction) != 0) {
                    dernierChangement[fonction] = i;
                }
            }
        }
    }

    long long nombreAnnonces = 0;
    cin >> nombreAnnonces;

    long long mensonges = 0;
    for (long long i = 1; i <= nombreAnnonces; ++i) {
        string hash;
        string etat;
        string fonction;
        cin >> hash >> etat >> fonction;

        const bool disponible = presentes.count(fonction) != 0;
        const bool vraie =
            etat == "A_VENIR" ? !disponible : disponible;

        if (vraie) {
            cout << "ANNONCE " << i << " VRAI\n";
            continue;
        }

        ++mensonges;
        const long long commitAnnonce = indiceCommit.at(hash);
        const auto changement = dernierChangement.find(fonction);
        const long long debutMensonge =
            changement == dernierChangement.end()
                ? commitAnnonce
                : max(commitAnnonce, changement->second);
        cout << "ANNONCE " << i << " MENT "
             << hashes[static_cast<size_t>(debutMensonge)] << '\n';
    }

    cout << "MENSONGES " << mensonges << '\n';
    return 0;
}
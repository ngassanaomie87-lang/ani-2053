#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Contournement {
    string nom;
    long long fois = 0;
    long long premiereObservation = 0;
    size_t ordreEntree = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string etiquette;
    long long nombreObservations = 0;
    cin >> etiquette >> nombreObservations;

    vector<Contournement> contournements;
    unordered_map<string, size_t> indexParNom;
    long long horsSeance = 0;

    for (long long i = 0; i < nombreObservations; ++i) {
        string instant;
        string nom;
        cin >> instant >> nom;

        const size_t deuxPoints = instant.find(':');
        if (deuxPoints == string::npos) {
            continue;
        }

        const long long minutes = stoll(instant.substr(0, deuxPoints));
        const long long secondes = stoll(instant.substr(deuxPoints + 1));
        const long long totalSecondes = minutes * 60 + secondes;
        if (totalSecondes > 20 * 60) {
            ++horsSeance;
            continue;
        }

        const auto trouve = indexParNom.find(nom);
        if (trouve == indexParNom.end()) {
            const size_t index = contournements.size();
            indexParNom.emplace(nom, index);
            contournements.push_back({nom, 1, totalSecondes, static_cast<size_t>(i)});
        } else {
            Contournement& contournement = contournements[trouve->second];
            ++contournement.fois;
            if (totalSecondes < contournement.premiereObservation) {
                contournement.premiereObservation = totalSecondes;
            }
        }
    }

    long long nombreCitations = 0;
    cin >> etiquette >> nombreCitations;
    unordered_set<string> cites;
    vector<string> ordreCitations;

    for (long long i = 0; i < nombreCitations; ++i) {
        string nom;
        cin >> nom;
        cites.insert(nom);
        ordreCitations.push_back(nom);
    }

    sort(contournements.begin(), contournements.end(),
         [](const Contournement& gauche, const Contournement& droite) {
             if (gauche.fois != droite.fois) {
                 return gauche.fois > droite.fois;
             }
             if (gauche.premiereObservation != droite.premiereObservation) {
                 return gauche.premiereObservation < droite.premiereObservation;
             }
             return gauche.ordreEntree < droite.ordreEntree;
         });

    long long nonDits = 0;
    long long totalObservationsEnSeance = 0;
    long long totalObservationsNonDites = 0;

    for (const Contournement& contournement : contournements) {
        const bool dit = cites.count(contournement.nom) != 0;
        cout << contournement.nom << ' ' << contournement.fois << ' '
             << (dit ? "DIT" : "NON_DIT") << '\n';
        totalObservationsEnSeance += contournement.fois;
        if (!dit) {
            ++nonDits;
            totalObservationsNonDites += contournement.fois;
        }
    }

    const long long partNonDite = totalObservationsEnSeance == 0
        ? 0
        : 100 * totalObservationsNonDites / totalObservationsEnSeance;
    cout << "NON_DITS " << nonDits << '\n';
    cout << "PART_NON_DITE " << partNonDite << '\n';

    bool aucunSansVu = true;
    cout << "DIT_SANS_VU";
    for (const string& nom : ordreCitations) {
        if (indexParNom.count(nom) == 0) {
            cout << ' ' << nom;
            aucunSansVu = false;
        }
    }
    if (aucunSansVu) {
        cout << " AUCUN";
    }
    cout << '\n';
    cout << "HORS_SEANCE " << horsSeance << '\n';

    return 0;
}
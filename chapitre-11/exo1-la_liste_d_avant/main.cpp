#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string label;
    long long nombreAvant = 0;
    cin >> label >> nombreAvant;

    vector<string> avant;
    avant.reserve(static_cast<size_t>(nombreAvant));
    unordered_set<string> clesAvant;
    clesAvant.reserve(static_cast<size_t>(nombreAvant));

    for (long long i = 0; i < nombreAvant; ++i) {
        string cle;
        cin >> cle;
        avant.push_back(cle);
        clesAvant.insert(cle);
    }

    long long nombreCorrige = 0;
    cin >> label >> nombreCorrige;
    unordered_set<string> corrige;
    corrige.reserve(static_cast<size_t>(nombreCorrige));

    for (long long i = 0; i < nombreCorrige; ++i) {
        string cle;
        cin >> cle;
        corrige.insert(cle);
    }

    long long nombreApres = 0;
    cin >> label >> nombreApres;
    unordered_set<string> apres;
    apres.reserve(static_cast<size_t>(nombreApres));

    for (long long i = 0; i < nombreApres; ++i) {
        string cle;
        cin >> cle;
        apres.insert(cle);
    }

    long long vraies = 0;
    long long fausses = 0;
    long long oubliees = 0;

    for (size_t i = 0; i < avant.size(); ++i) {
        const string& cle = avant[i];
        string categorie;

        if (corrige.count(cle) != 0) {
            categorie = "DEVENUE_FAUSSE";
            ++fausses;
        } else if (apres.count(cle) != 0) {
            categorie = "ENCORE_VRAIE";
            ++vraies;
        } else {
            categorie = "OUBLIEE";
            ++oubliees;
        }

        cout << i + 1 << ' ' << cle << ' ' << categorie << '\n';
    }

    long long nouvelles = 0;
    for (const string& cle : apres) {
        if (clesAvant.count(cle) == 0) {
            ++nouvelles;
        }
    }

    const long long nombreManquesReels = vraies + oubliees;
    const long long partOubliee =
        nombreManquesReels == 0 ? 0 : 100 * oubliees / nombreManquesReels;

    cout << "VRAIES " << vraies << '\n';
    cout << "FAUSSES " << fausses << '\n';
    cout << "OUBLIEES " << oubliees << '\n';
    cout << "NOUVELLES " << nouvelles << '\n';
    cout << "PART_OUBLIEE " << partOubliee << '\n';

    return 0;
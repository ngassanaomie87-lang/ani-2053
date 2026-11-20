#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> operationsAttendue;
    vector<int> traitsPresents;
    int prochainTrait = 0;
    int nombreOperations = 0;
    int justes = 0;
    int riens = 0;
    int trahis = 0;
    int nombreAnnulations = 0;

    string ligne;
    while (getline(cin, ligne)) {
        if (ligne.empty()) {
            continue;
        }

        istringstream entree(ligne);
        string operation;
        entree >> operation;

        if (operation == "ANNULE") {
            ++nombreAnnulations;

            string attendu = "AUCUNE";
            if (!operationsAttendue.empty()) {
                attendu = operationsAttendue.back();
                operationsAttendue.pop_back();
            }

            string obtenu = "RIEN";
            if (!traitsPresents.empty()) {
                obtenu = "TRAIT " + to_string(traitsPresents.back());
                traitsPresents.pop_back();
            }

            string verdict;
            if (obtenu == attendu || (attendu == "AUCUNE" && obtenu == "RIEN")) {
                verdict = "JUSTE";
                ++justes;
            } else if (obtenu == "RIEN") {
                verdict = "RIEN";
                ++riens;
            } else {
                verdict = "TRAHI";
                ++trahis;
            }

            cout << "ANNULE " << nombreAnnulations << " ATTENDU " << attendu
                 << " OBTENU " << obtenu << ' ' << verdict << '\n';
        } else if (operation == "TRAIT") {
            ++prochainTrait;
            ++nombreOperations;
            operationsAttendue.push_back("TRAIT " + to_string(prochainTrait));
            traitsPresents.push_back(prochainTrait);
        } else if (operation == "EFFACE") {
            ++nombreOperations;
            operationsAttendue.push_back(operation);
            traitsPresents.clear();
        } else if (operation == "RANGE") {
            ++nombreOperations;
            operationsAttendue.push_back(operation);
        } else if (operation == "DEPOSE" || operation == "DEPLACE" ||
                   operation == "SUPPRIME" || operation == "MIROIR") {
            string nom;
            entree >> nom;
            ++nombreOperations;
            operationsAttendue.push_back(operation + " " + nom);
        }
    }

    cout << "JUSTES " << justes << '\n';
    cout << "RIENS " << riens << '\n';
    cout << "TRAHIS " << trahis << '\n';
    cout << "ANNULABLES " << prochainTrait << " SUR " << nombreOperations << '\n';

    return 0;
}
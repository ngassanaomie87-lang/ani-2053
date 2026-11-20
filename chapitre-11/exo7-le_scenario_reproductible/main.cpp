#include <algorithm>
#include <charconv>
#include <iostream>
#include <limits>
#include <string>
#include <system_error>
#include <vector>

using namespace std;

struct Action {
    long long tour;
    string premier;
    string deuxieme;
    string troisieme;
};

static vector<string> decouper(const string& texte, char separateur) {
    vector<string> champs;
    size_t debut = 0;

    while (true) {
        const size_t fin = texte.find(separateur, debut);
        if (fin == string::npos) {
            champs.push_back(texte.substr(debut));
            break;
        }
        champs.push_back(texte.substr(debut, fin - debut));
        debut = fin + 1;
    }

    return champs;
}

static bool lireEntier(const string& texte, long long& valeur) {
    if (texte.empty()) {
        return false;
    }

    const char* debut = texte.data();
    const char* fin = debut + texte.size();
    if (*debut == '+') {
        ++debut;
        if (debut == fin) {
            return false;
        }
    }

    const auto resultat = from_chars(debut, fin, valeur);
    return resultat.ec == errc{} && resultat.ptr == fin;
}

static void ajouterTour(vector<long long>& tours, long long tour) {
    if (tour > 0) {
        tours.push_back(tour);
    }
}

static void ajouterToursActions(vector<long long>& tours, const vector<Action>& actions,
                                bool avecArret, long long arret) {
    for (const Action& action : actions) {
        if (action.tour > 0 && (!avecArret || action.tour < arret)) {
            ajouterTour(tours, action.tour);
        }
    }
}

static void ajouterToursCaptures(vector<long long>& tours, const vector<long long>& captures,
                                 bool avecArret, long long arret) {
    for (long long capture : captures) {
        if (capture > 0 && (!avecArret || capture <= arret)) {
            tours.push_back(capture - 1);
        }
    }
}

static long long compterJamaisActions(const vector<Action>& actions,
                                      bool avecArret, long long arret) {
    long long jamais = 0;
    for (const Action& action : actions) {
        if (action.tour <= 0 || (avecArret && action.tour >= arret)) {
            ++jamais;
        }
    }
    return jamais;
}

static long long compterJamaisCaptures(const vector<long long>& captures,
                                       bool avecArret, long long arret) {
    long long jamais = 0;
    for (long long capture : captures) {
        if (capture <= 0 || (avecArret && capture > arret)) {
            ++jamais;
        }
    }
    return jamais;
}

static void afficherActions(const vector<Action>& actions, long long tour,
                            const string& type, int nombreChamps) {
    for (const Action& action : actions) {
        if (action.tour != tour) {
            continue;
        }

        cout << "TOUR " << tour << ' ' << type;
        if (nombreChamps >= 1) {
            cout << ' ' << action.premier;
        }
        if (nombreChamps >= 2) {
            cout << ' ' << action.deuxieme;
        }
        if (nombreChamps >= 3) {
            cout << ' ' << action.troisieme;
        }
        cout << '\n';
    }
}

static bool lireActionDeuxPoints(const string& valeur, size_t nombreMinimal,
                                 Action& action, vector<string>& champs) {
    champs = decouper(valeur, ':');
    return champs.size() >= nombreMinimal && lireEntier(champs[0], action.tour);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> captures;
    vector<Action> panoramiques;
    vector<Action> zooms;
    vector<Action> depots;
    vector<Action> clics;
    vector<Action> mouvements;
    vector<Action> rangements;
    vector<Action> grilles;
    long long ignores = 0;
    long long arret = 0;
    bool sortieValide = false;

    string ligne;
    while (getline(cin, ligne)) {
        if (!ligne.empty() && ligne.back() == '\r') {
            ligne.pop_back();
        }

        const size_t egal = ligne.find('=');
        if (egal == string::npos) {
            ++ignores;
            continue;
        }

        const string nom = ligne.substr(0, egal);
        const string valeur = ligne.substr(egal + 1);

        if (nom == "NK_AGENT_SHOT") {
            for (const string& element : decouper(valeur, ',')) {
                long long tour = 0;
                if (!lireEntier(element, tour) || captures.size() >= 8) {
                    ++ignores;
                } else {
                    captures.push_back(tour);
                }
            }
        } else if (nom == "NK_AGENT_EXIT") {
            if (!lireEntier(valeur, arret)) {
                ++ignores;
            } else {
                sortieValide = true;
            }
        } else if (nom == "NK_AGENT_PAN" || nom == "NK_AGENT_MOVE" ||
                   nom == "NK_AGENT_ZOOM" || nom == "NK_AGENT_GRID") {
            Action action{};
            vector<string> champs;
            size_t nombreMinimal = 0;
            if (nom == "NK_AGENT_PAN" || nom == "NK_AGENT_MOVE") {
                nombreMinimal = 3;
            } else if (nom == "NK_AGENT_ZOOM") {
                nombreMinimal = 4;
            } else {
                nombreMinimal = 2;
            }

            if (!lireActionDeuxPoints(valeur, nombreMinimal, action, champs)) {
                ++ignores;
            } else if (nom == "NK_AGENT_PAN") {
                action.premier = champs[1];
                action.deuxieme = champs[2];
                panoramiques.push_back(action);
            } else if (nom == "NK_AGENT_MOVE") {
                action.premier = champs[1];
                action.deuxieme = champs[2];
                mouvements.push_back(action);
            } else if (nom == "NK_AGENT_ZOOM") {
                action.premier = champs[1];
                action.deuxieme = champs[2];
                action.troisieme = champs[3];
                zooms.push_back(action);
            } else {
                action.premier = champs[1];
                grilles.push_back(action);
            }
        } else if (nom == "NK_AGENT_PACK") {
            Action action{};
            if (!lireEntier(valeur, action.tour)) {
                ++ignores;
            } else {
                rangements.push_back(action);
            }
        } else if (nom == "NK_AGENT_CLICK") {
            for (const string& element : decouper(valeur, ';')) {
                Action action{};
                vector<string> champs;
                if (!lireActionDeuxPoints(element, 3, action, champs) || clics.size() >= 8) {
                    ++ignores;
                } else {
                    action.premier = champs[1];
                    action.deuxieme = champs[2];
                    clics.push_back(action);
                }
            }
        } else if (nom == "NK_AGENT_DROP") {
            for (const string& element : decouper(valeur, ';')) {
                const size_t premierSep = element.find(':');
                const size_t deuxiemeSep =
                    premierSep == string::npos ? string::npos : element.find(':', premierSep + 1);
                const size_t troisiemeSep =
                    deuxiemeSep == string::npos ? string::npos : element.find(':', deuxiemeSep + 1);

                Action action{};
                if (troisiemeSep == string::npos ||
                    !lireEntier(element.substr(0, premierSep), action.tour) ||
                    depots.size() >= 8) {
                    ++ignores;
                } else {
                    action.premier = element.substr(troisiemeSep + 1);
                    action.deuxieme = element.substr(premierSep + 1, deuxiemeSep - premierSep - 1);
                    action.troisieme = element.substr(deuxiemeSep + 1, troisiemeSep - deuxiemeSep - 1);
                    depots.push_back(action);
                }
            }
        } else {
            ++ignores;
        }
    }

    const bool avecArret = sortieValide && arret > 0;
    vector<long long> tours;
    ajouterToursActions(tours, panoramiques, avecArret, arret);
    ajouterToursActions(tours, zooms, avecArret, arret);
    ajouterToursActions(tours, depots, avecArret, arret);
    ajouterToursActions(tours, clics, avecArret, arret);
    ajouterToursActions(tours, mouvements, avecArret, arret);
    ajouterToursActions(tours, rangements, avecArret, arret);
    ajouterToursActions(tours, grilles, avecArret, arret);
    ajouterToursCaptures(tours, captures, avecArret, arret);
    sort(tours.begin(), tours.end());
    tours.erase(unique(tours.begin(), tours.end()), tours.end());

    for (long long tour : tours) {
        if (tour > 0) {
            afficherActions(panoramiques, tour, "PAN", 2);
            afficherActions(zooms, tour, "ZOOM", 3);
            afficherActions(depots, tour, "DEPOT", 3);
            afficherActions(clics, tour, "CLIC", 2);
            afficherActions(mouvements, tour, "MOVE", 2);
            afficherActions(rangements, tour, "RANGE", 0);
            afficherActions(grilles, tour, "GRILLE", 1);
        }

        if (tour < numeric_limits<long long>::max()) {
            const long long tourCapture = tour + 1;
            for (long long capture : captures) {
                if (capture == tourCapture) {
                cout << "TOUR " << tour << " CAPTURE\n";
                }
            }
        }
    }

    long long jamais = 0;
    jamais += compterJamaisActions(panoramiques, avecArret, arret);
    jamais += compterJamaisActions(zooms, avecArret, arret);
    jamais += compterJamaisActions(depots, avecArret, arret);
    jamais += compterJamaisActions(clics, avecArret, arret);
    jamais += compterJamaisActions(mouvements, avecArret, arret);
    jamais += compterJamaisActions(rangements, avecArret, arret);
    jamais += compterJamaisActions(grilles, avecArret, arret);
    jamais += compterJamaisCaptures(captures, avecArret, arret);

    if (avecArret) {
        cout << "ARRET " << arret - 1 << '\n';
    } else {
        cout << "SANS_ARRET\n";
    }
    cout << "JAMAIS " << jamais << '\n';
    cout << "IGNORES " << ignores << '\n';
    return 0;
}
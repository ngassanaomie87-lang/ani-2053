#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Image {
    string name;
    double x;
    double y;
    double largeur;
    double hauteur;
    double echelle;
};

static bool touche(const Image& img, double px, double py) {
    const double demiLargeur = img.largeur * img.echelle / 2.0;
    const double demiHauteur = img.hauteur * img.echelle / 2.0;
    return fabs(px - img.x) <= demiLargeur && fabs(py - img.y) <= demiHauteur;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N = 0;
    cin >> N;

    vector<Image> images;
    images.reserve(static_cast<size_t>(N));

    for (int i = 0; i < N; ++i) {
        Image img;
        cin >> img.name >> img.x >> img.y >> img.largeur >> img.hauteur >> img.echelle;
        images.push_back(img);
    }

    int Q = 0;
    cin >> Q;

    int erreurs = 0;
    for (int i = 1; i <= Q; ++i) {
        double px = 0.0;
        double py = 0.0;
        cin >> px >> py;

        string A = "AUCUN";
        for (int j = static_cast<int>(images.size()) - 1; j >= 0; --j) {
            if (touche(images[j], px, py)) {
                A = images[j].name;
                break;
            }
        }

        string B = "AUCUN";
        for (size_t j = 0; j < images.size(); ++j) {
            if (touche(images[j], px, py)) {
                B = images[j].name;
                break;
            }
        }

        if (A != B) {
            ++erreurs;
        }

        cout << "CLIC " << i << ' ' << A << ' ' << B << '\n';
    }

    cout << "ERREURS " << erreurs << '\n';
    return 0;
}

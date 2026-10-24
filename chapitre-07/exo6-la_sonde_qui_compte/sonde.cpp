#include "NkCanvasApp.h"
#include <iostream>
#include <fstream>

class SondeApp : public NkCanvasApp {
private:
    int m_compteur = 0;

public:
    void OnInit() override {
        // Configuration de la cadence (modifier à 0 pour sans plafond si besoin, ou laisser par défaut à 60)
        Config().imagesParSeconde = 60; 
    }

    void OnRender(NkRenderTarget &target) override {
        m_compteur++;
        double tempsActuel = Time();

        // Ligne de journal affichant le numéro du passage et le temps
        std::cout << "Passage : " << m_compteur << " | Time() : " << tempsActuel << std::endl;

        // Quand Time() atteint 10 secondes, on écrit le compte dans un fichier et on quitte
        if (tempsActuel >= 10.0) {
            std::ofstream fichier("resultat_sonde.txt");
            if (fichier.is_open()) {
                fichier << "Total passages en 10 secondes : " << m_compteur << std::endl;
                fichier.close();
            }
            Quit();
        }
    }
};

int main() {
    SondeApp app;
    app.Run();
    return 0;
}
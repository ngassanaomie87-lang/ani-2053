#include <iostream>
#include "NKWindow.h"
#include "NKCanvas.h"
#include "NkThreadPool.h"
#include "NkPromise.h"
#include "NkFuture.h"
#include "NkImage.h"
#include "NkChrono.h"

// Mettez true pour tester la version Get (bloquante), false pour IsReady (non bloquante)
const bool TEST_VERSION_GET = false; 

int main() {
    NKWindow window("Exercice Futur", 800, 600);
    NKCanvas canvas(window);

    NkThreadPool pool;
    NkPromise<NkImage> promesse;
    NkFuture<NkImage> futur = promesse.GetFuture();

    int frameCount = 0;
    bool chargementLance = false;
    long long maxFrameTime = 0;
    int framesPendantChargement = 0;

    int carreX = 0;

    while (window.IsOpen()) {
        NkChrono frameChrono;

        window.PollEvents();
        canvas.Clear(0, 0, 0);

        // Dessiner le carré qui avance d'un pixel
        canvas.DrawRect(carreX, 100, 50, 50, 255, 255, 255);
        carreX = (carreX + 1) % 800;

        // Au bout de 60 trames, on lance le chargement de l'image lourde
        if (frameCount == 60 && !chargementLance) {
            chargementLance = true;
            pool.Enqueue([promesse = std::move(promesse)]() mutable {
                NkImage img;
                img.Load("grande-photo.png"); // Remplacez par le nom de votre grande image
                promesse.SetValue(nkentseu::traits::NkMove(img));
            });
        }

        // Gestion du futur selon la version choisie
        if (chargementLance && !futur.IsReady() && frameCount > 60) {
            if (TEST_VERSION_GET) {
                // Version Get : Bloque net la fenêtre
                auto img = futur.Get();
            } else {
                // Version IsReady : Ne bloque pas, on compte les trames
                framesPendantChargement++;
            }
        }

        canvas.Display();

        long long elapsed = frameChrono.Elapsed().milliseconds();
        if (frameCount > 60 && elapsed > maxFrameTime) {
            maxFrameTime = elapsed;
        }

        frameCount++;

        // Pour stopper après quelques secondes si besoin
        if (frameCount > 300) break;
    }

    std::cout << "Max frame time: " << maxFrameTime << " ms" << std::endl;
    std::cout << "Frames pendant chargement: " << framesPendantChargement << std::endl;

    return 0;
}
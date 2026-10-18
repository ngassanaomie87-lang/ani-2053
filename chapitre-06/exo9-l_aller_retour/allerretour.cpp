#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "NKNetwork/NKNetwork.h"
#include "NkChrono.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Utilisation : " << argv[0] << " [server|client]" << std::endl;
        return 1;
    }

    std::string role = argv[1];
    NkSocket::PlatformInit();
    NkConnectionManager manager;

    if (role == "server") {
        manager.StartServer(7777, 4);
        std::cout << "Serveur RTT démarré..." << std::endl;
        while (true) {
            manager.DrainAll([&](const NkPeerId& from, const char* data, size_t size) {
                // Renvoie le message exactement à l'expéditeur (Echo)
                manager.SendTo(from, data, size);
            });
            NkPlatform::SleepMilliseconds(5);
        }
    } 
    else if (role == "client") {
        manager.Connect(NkAddress("127.0.0.1", 7777));
        std::cout << "Client connecté pour les mesures RTT..." << std::endl;
        NkPlatform::SleepMilliseconds(1000); // Attente de l'établissement de la connexion

        std::vector<double> rttMeasures;
        double srtt = 100.0; // Valeur initiale du SRTT
        const double alpha = 0.125;

        for (int i = 0; i < 100; ++i) {
            NkChrono chrono;
            int messageId = i;

            // Envoi du message numéroté
            manager.Broadcast(reinterpret_cast<char*>(&messageId), sizeof(messageId));

            bool received = false;
            while (!received) {
                manager.DrainAll([&](const NkPeerId&, const char* data, size_t size) {
                    if (size == sizeof(int)) {
                        int idReçu = *reinterpret_cast<const int*>(data);
                        if (idReçu == messageId) {
                            received = true;
                        }
                    }
                });
                if (!received) {
                    NkPlatform::SleepMilliseconds(1);
                }
            }

            double elapsedMs = chrono.Elapsed().milliseconds();
            rttMeasures.push_back(elapsedMs);

            // Calcul du lissage exponentiel : SRTT = (1 - alpha) * SRTT + alpha * R
            srtt = (1.0 - alpha) * srtt + alpha * elapsedMs;
        }

        // Affichage des statistiques pour remplir rtt.txt
        double somme = std::accumulate(rttMeasures.begin(), rttMeasures.end(), 0.0);
        double moyenne = somme / rttMeasures.size();
        auto minMax = std::minmax_element(rttMeasures.begin(), rttMeasures.end());

        std::cout << "Moyenne : " << moyenne << std::endl;
        std::cout << "Min : " << *minMax.first << std::endl;
        std::cout << "Max : " << *minMax.second << std::endl;
        std::cout << "Lissée : " << srtt << std::endl;
    }

    return 0;
}
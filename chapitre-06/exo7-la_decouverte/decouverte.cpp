#include <iostream>
#include <string>
#include <vector>
#include "NKNetwork/NKNetwork.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Utilisation : " << argv[0] << " [server|client]" << std::endl;
        return 1;
    }

    std::string role = argv[1];
    NkSocket::PlatformInit();

    if (role == "server") {
        NkDiscovery::ServerInfo info;
        info.name = "MonSuperServeurLAN";
        info.maxPlayers = 8;

        std::cout << "Serveur de découverte lancé. Diffusion en cours..." << std::endl;
        while (true) {
            NkDiscovery::Broadcast(info);
            NkPlatform::SleepMilliseconds(2000); // Toutes les deux secondes
        }
    } 
    else if (role == "client") {
        std::cout << "Client en écoute des serveurs sur le réseau (6 secondes)..." << std::endl;
        
        std::vector<NkDiscovery::ServerInfo> trouves;
        // Écoute pendant 6000 millisecondes (6 secondes)
        NkDiscovery::Listen(6000, trouves);

        std::cout << "Serveurs trouvés : " << trouves.size() << std::endl;
        for (const auto& srv : trouves) {
            std::cout << " - Nom: " << srv.name 
                      << " | Adresse: " << srv.addr.ToString() 
                      << " | Joueurs max: " << srv.maxPlayers << std::endl;
        }
    }

    return 0;
}
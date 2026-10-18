#include <iostream>
#include <string>
#include <map>
#include "NKNetwork/NKNetwork.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Utilisation : " << argv[0] << " [server|client]" << std::endl;
        return 1;
    }

    std::string role = argv[1];
    NkSocket::PlatformInit();
    NkConnectionManager manager;

    if (role == "server") {
        std::map<uint64_t, std::string> tableJoueurs;

        manager.onPeerConnected = [&](const NkPeerId& peer) {
            tableJoueurs[peer.value] = "Joueur_" + std::to_string(peer.value);
            std::cout << "Connexion : " << tableJoueurs[peer.value] << " (Total: " << tableJoueurs.size() << ")" << std::endl;
        };

        manager.onPeerDisconnected = [&](const NkPeerId& peer, NkDisconnectReason reason) {
            std::string nom = tableJoueurs[peer.value];
            tableJoueurs.erase(peer.value);
            std::cout << "Deconnexion de " << nom << " | Raison : " << static_cast<int>(reason) 
                      << " (Restants: " << tableJoueurs.size() << ")" << std::endl;
        };

        manager.StartServer(7777, 4);
        std::cout << "Serveur fantome demarre..." << std::endl;

        while (true) {
            manager.DrainAll([&](const NkPeerId& from, const char*, size_t) {
                // Le serveur traite les messages reçus pour s'assurer de la présence
            });
            NkPlatform::SleepMilliseconds(10);
        }
    } 
    else if (role == "client") {
        manager.Connect(NkAddress("127.0.0.1", 7777));
        std::cout << "Client connecte au serveur..." << std::endl;

        // Si l'argument "shutdown" est passé, le client appelle Shutdown() proprement
        bool testShutdown = (argc >= 3 && std::string(argv[2]) == "shutdown");

        int compteur = 0;
        while (true) {
            std::string msg = "Ping " + std::to_string(compteur++);
            manager.Broadcast(msg.c_str(), msg.size() + 1);

            if (testShutdown && compteur == 10) {
                std::cout << "Fermeture propre avec Shutdown()..." << std::endl;
                manager.Shutdown();
                break;
            }

            NkPlatform::SleepMilliseconds(100);
        }
    }

    return 0;
}
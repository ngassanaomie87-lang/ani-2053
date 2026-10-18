#include <iostream>
#include <string>
#include "NKNetwork/NKNetwork.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Utilisation : " << argv[0] << " [server|client] [ip_serveur(optionnel)]" << std::endl;
        return 1;
    }

    std::string role = argv[1];
    NkSocket::PlatformInit();
    NkConnectionManager manager;

    manager.onPeerConnected = [](const NkPeerId& peer) {
        std::cout << "Connecte avec le pair ID : " << peer.value << std::endl;
    };

    if (role == "server") {
        manager.StartServer(7777, 8);
        std::cout << "Serveur démarré sur le port 7777..." << std::endl;

        while (true) {
            manager.DrainAll([&](const NkPeerId& from, const char* data, size_t size) {
                std::string message(data, size);
                std::cout << "Reçu de [" << from.value << "] : " << message << std::endl;
            });
            NkPlatform::SleepMilliseconds(10);
        }
    } 
    else if (role == "client") {
        std::string ip = (argc >= 3) ? argv[2] : "127.0.0.1";
        manager.Connect(NkAddress(ip.c_str(), 7777));
        std::cout << "Client en cours de connexion vers " << ip << "..." << std::endl;

        // Attente de connexion et envoi d'un message
        NkPlatform::SleepMilliseconds(1000);
        
        std::string msg = "Bonjour du client NKNetwork !";
        // Envoi du message (nécessite de récupérer un peer actif ou d'itérer)
        // Note : selon la structure exacte du kit, l'envoi s'adapte aux pairs connectés.
        // Exemple générique d'envoi :
        manager.Broadcast(msg.c_str(), msg.size() + 1);

        while (true) {
            manager.DrainAll([](const NkPeerId& from, const char* data, size_t size) {
                std::cout << "Reçu : " << std::string(data, size) << std::endl;
            });
            NkPlatform::SleepMilliseconds(10);
        }
    }

    return 0;
}
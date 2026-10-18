#include <iostream>
#include "NKNetwork/NKNetwork.h"

int main() {
    // Valeurs de l'état du joueur
    bool estVivant = true;
    uint8_t numero = 127;
    float posX = 150.25f;
    float posY = -230.50f;
    float angle = 180.0f;
    int32_t sante = 90;

    // ==========================================
    // 1. PAQUET COMPLET
    // ==========================================
    uint8_t bufferComplet[256];
    NkBitWriter writerComplet(bufferComplet, sizeof(bufferComplet));

    writerComplet.WriteU8(estVivant ? 1 : 0);
    writerComplet.WriteU8(numero);
    writerComplet.WriteF32(posX);
    writerComplet.WriteF32(posY);
    writerComplet.WriteF32(angle);
    writerComplet.WriteI32(sante);

    std::cout << "Paquet complet -> Bytes: " << writerComplet.BytesWritten() 
              << " | Bits: " << writerComplet.BitsWritten() << std::endl;

    // ==========================================
    // 2. PAQUET QUANTIFIÉ
    // ==========================================
    uint8_t bufferQuantifie[256];
    NkBitWriter writerQuantifie(bufferQuantifie, sizeof(bufferQuantifie));

    writerQuantifie.WriteBool(estVivant);
    writerQuantifie.WriteU8(numero);
    writerQuantifie.WriteF32Q(posX, -500.0f, 500.0f, 0.01f); // au centimètre près
    writerQuantifie.WriteF32Q(posY, -500.0f, 500.0f, 0.01f);
    writerQuantifie.WriteF32Q(angle, 0.0f, 360.0f, 0.5f);     // au demi-degré
    writerQuantifie.WriteInt(sante, 0, 100);

    std::cout << "Paquet quantifié -> Bytes: " << writerQuantifie.BytesWritten() 
              << " | Bits: " << writerQuantifie.BitsWritten() << std::endl;

    // ==========================================
    // 3. RELECTURE (READER)
    // ==========================================
    NkBitReader readerComplet(bufferComplet, writerComplet.BytesWritten());
    // Lecture des valeurs complètes...

    NkBitReader readerQuantifie(bufferQuantifie, writerQuantifie.BytesWritten());
    // Lecture des valeurs quantifiées...

    return 0;
}
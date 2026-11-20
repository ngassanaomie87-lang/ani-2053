#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#include "NkRef/NkRefFile.h"
#include "NKTime/NkChrono.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        return 1;
    }

    const std::size_t nombreImages = static_cast<std::size_t>(std::strtoull(argv[1], nullptr, 10));
    nkref::NkRefBoard board;
    std::vector<nkref::NkRefFileImage> images(nombreImages);
    std::vector<nkentseu::uint8> source(1000, 0x5A);

    for (std::size_t i = 0; i < nombreImages; ++i) {
        const nkentseu::math::NkVec2f position{
            static_cast<float>(i * 120),
            static_cast<float>(i * 100)
        };
        board.AddItem(position, 100, 80, nkentseu::NkString());
        images[i] = {source.data(), source.size()};
    }

    nkref::NkRefView view;
    nkentseu::NkVector<nkentseu::uint8> bytes;

    nkentseu::NkChrono chronoEcriture;
    for (int i = 0; i < 20; ++i) {
        nkref::NkRefSerialize(bytes, board, images.data(), view, false);
    }
     const double ecritureMs = chronoEcriture.Elapsed().milliseconds / 20.0;

    nkentseu::NkVector<nkref::NkRefLoadedItem> imagesRelues;
    nkentseu::NkVector<nkref::NkRefStroke> traitsRelus;
    nkref::NkRefView vueRelue;
    bool themeSombreRelu = false;
    bool grilleVisibleRelue = true;

    nkentseu::NkChrono chronoLecture;
    for (int i = 0; i < 20; ++i) {
        if (!nkref::NkRefDeserialize(bytes.Data(), bytes.Size(), imagesRelues, traitsRelus,
                                     vueRelue, themeSombreRelu, grilleVisibleRelue)) {
            return 2;
        }
    }
    const double lectureMs = chronoLecture.Elapsed().milliseconds / 20.0;

    std::cout << std::fixed << std::setprecision(3)
              << "images : " << nombreImages
              << ", octets : " << bytes.Size()
              << ", ecrire : " << ecritureMs << " ms"
              << ", relire : " << lectureMs << " ms\n";
    return 0;
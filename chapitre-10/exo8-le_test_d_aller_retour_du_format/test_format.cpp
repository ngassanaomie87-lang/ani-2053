#include "NkRef/NkRefFile.h"

#include <iostream>

using nkentseu::NkVector;
using nkentseu::uint8;
using nkentseu::usize;
using namespace nkref;

int main() {
	NkRefBoard original;
	original.AddItem({12.0f, -7.0f}, 640, 480, nkentseu::NkString());
	original.items[0].scale = 0.75f;
	original.items[0].rotationDeg = 15.0f;
	original.items[0].opacity = 0.8f;
	original.items[0].mirrorX = true;
	original.AddItem({-32.0f, 18.0f}, 320, 200, nkentseu::NkString());
	original.items[1].scale = 1.25f;
	original.items[1].rotationDeg = -30.0f;
	original.items[1].opacity = 0.6f;
	original.items[1].mirrorY = true;

	NkRefStroke pencil;
	pencil.r = 40;
	pencil.g = 120;
	pencil.b = 220;
	pencil.a = 255;
	pencil.widthWorld = 2.5f;
	pencil.points.PushBack({1.0f, 2.0f});
	pencil.points.PushBack({8.0f, -3.0f});
	original.strokes.PushBack(pencil);

	const uint8 source0[] = {0x89, 0x50, 0x4E, 0x47};
	const uint8 source1[] = {0xFF, 0xD8, 0xFF};
	const NkRefFileImage originalSources[] = {
		{source0, sizeof(source0)},
		{source1, sizeof(source1)},
	};

	NkRefView originalView;
	originalView.center = {24.5f, -16.25f};
	originalView.zoom = 1.75f;

	NkVector<uint8> firstBytes;
	NkRefSerialize(firstBytes, original, originalSources, originalView, true, false);

	NkVector<NkRefLoadedItem> loadedItems;
	NkVector<NkRefStroke> loadedStrokes;
	NkRefView loadedView;
	bool loadedDark = false;
	bool loadedShowGrid = true;
	const bool readBack = NkRefDeserialize(firstBytes.Data(), firstBytes.Size(), loadedItems, loadedStrokes,
										   loadedView, loadedDark, loadedShowGrid);

	bool identical = false;
	if (readBack) {
		NkRefBoard reconstructed;
		NkVector<NkRefFileImage> reconstructedSources;
		for (usize i = 0; i < loadedItems.Size(); ++i) {
			reconstructed.items.PushBack(loadedItems[i].item);
			reconstructedSources.PushBack({loadedItems[i].src.Data(), loadedItems[i].src.Size()});
		}
		for (usize i = 0; i < loadedStrokes.Size(); ++i) {
			reconstructed.strokes.PushBack(loadedStrokes[i]);
		}

		NkVector<uint8> secondBytes;
		NkRefSerialize(secondBytes, reconstructed, reconstructedSources.Data(), loadedView, loadedDark,
					   loadedShowGrid);
		identical = firstBytes.Size() == secondBytes.Size();
		for (usize i = 0; identical && i < firstBytes.Size(); ++i) {
			identical = firstBytes[i] == secondBytes[i];
		}
	}

	std::cout << "octets : " << firstBytes.Size() << ", relu : " << (readBack ? "OUI" : "NON")
			  << ", identiques : " << (identical ? "OUI" : "NON") << '\n';
	return 0;
}

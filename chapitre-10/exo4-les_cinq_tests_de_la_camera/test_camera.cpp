#include "NkRef/NkRefView.h"

#include <cmath>
#include <iostream>

namespace {

using nkentseu::float32;

bool Near(float32 actual, float32 expected) {
	return std::fabs(actual - expected) <= 0.001f;
}

bool aller_retour() {
	nkref::NkRefView view;
	view.center = {17.5f, -23.25f};
	view.zoom = 2.75f;
	const nkentseu::math::NkVec2f viewport{1280.0f, 720.0f};
	const nkentseu::math::NkVec2f pixel{314.25f, 611.5f};
	const nkentseu::math::NkVec2f world = view.PixelToWorld(pixel, viewport);
	const nkentseu::math::NkVec2f roundTrip = view.WorldToPixel(world, viewport);
	return Near(roundTrip.x, pixel.x) && Near(roundTrip.y, pixel.y);
}

bool point_fixe() {
	nkref::NkRefView view;
	view.center = {-41.0f, 13.5f};
	view.zoom = 1.75f;
	const nkentseu::math::NkVec2f viewport{1024.0f, 768.0f};
	const nkentseu::math::NkVec2f pixel{173.0f, 590.0f};
	const nkentseu::math::NkVec2f before = view.PixelToWorld(pixel, viewport);
	view.ZoomAtPixel(2.3f, pixel, viewport);
	const nkentseu::math::NkVec2f after = view.PixelToWorld(pixel, viewport);
	return Near(after.x, before.x) && Near(after.y, before.y);
}

bool bornes() {
	nkref::NkRefView view;
	const nkentseu::math::NkVec2f viewport{800.0f, 600.0f};
	const nkentseu::math::NkVec2f pixel{250.0f, 175.0f};
	view.ZoomAtPixel(1000.0f, pixel, viewport);
	const bool upperBound = view.zoom >= nkref::NkRefView::kZoomMin &&
							view.zoom <= nkref::NkRefView::kZoomMax;
	view.ZoomAtPixel(0.00001f, pixel, viewport);
	return upperBound && view.zoom >= nkref::NkRefView::kZoomMin &&
		   view.zoom <= nkref::NkRefView::kZoomMax;
}

bool grille() {
	nkref::NkRefView view;
	const float32 minimumPixels = 16.0f;
	float32 previous = view.GridSpacing(minimumPixels);
	const float32 zooms[] = {0.8f, 0.4f, 0.2f, 0.1f, 0.05f, nkref::NkRefView::kZoomMin};
	for (float32 zoom : zooms) {
		view.zoom = zoom;
		const float32 spacing = view.GridSpacing(minimumPixels);
		if (spacing < previous)
			return false;
		previous = spacing;
	}
	return true;
}

bool origine() {
	nkref::NkRefView view;
	const nkentseu::math::NkVec2f viewport{800.0f, 600.0f};
	view.ZoomAtPixel(3.0f, {120.0f, 90.0f}, viewport);
	view.PanByPixels(45.0f, -30.0f);
	view.Reset();
	return Near(view.center.x, 0.0f) && Near(view.center.y, 0.0f) && Near(view.zoom, 1.0f);
}

} // namespace

int main() {
	const struct {
		const char *name;
		bool (*run)();
	} tests[] = {
		{"aller_retour", aller_retour},
		{"point_fixe", point_fixe},
		{"bornes", bornes},
		{"grille", grille},
		{"origine", origine},
	};

	for (const auto &test : tests)
		std::cout << "TEST " << test.name << " : " << (test.run() ? "OK" : "ECHEC") << '\n';
	return 0;
}

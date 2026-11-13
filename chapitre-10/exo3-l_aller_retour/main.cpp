#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

double PixelToWorld(double pixel, double viewport, double center, double zoom) {
	return center + (pixel - viewport / 2.0) / zoom;
}

void PrintFixed(double value, int precision) {
	std::ostringstream formatted;
	formatted << std::fixed << std::setprecision(precision) << value;
	std::string result = formatted.str();
	if (precision == 2 && result == "-0.00")
		result = "0.00";
	std::cout << result;
}

} // namespace

int main() {
	double width, height;
	if (!(std::cin >> width >> height))
		return 0;

	double centerX = 0.0;
	double centerY = 0.0;
	double zoom = 1.0;
	std::string command;

	while (std::cin >> command) {
		if (command == "VERS_MONDE") {
			double pixelX, pixelY;
			std::cin >> pixelX >> pixelY;
			std::cout << "MONDE ";
			PrintFixed(PixelToWorld(pixelX, width, centerX, zoom), 2);
			std::cout << ' ';
			PrintFixed(PixelToWorld(pixelY, height, centerY, zoom), 2);
			std::cout << '\n';
			std::cout.flush();
		} else if (command == "VERS_PIXEL") {
			double worldX, worldY;
			std::cin >> worldX >> worldY;
			std::cout << "PIXEL ";
			PrintFixed((worldX - centerX) * zoom + width / 2.0, 2);
			std::cout << ' ';
			PrintFixed((worldY - centerY) * zoom + height / 2.0, 2);
			std::cout << '\n';
			std::cout.flush();
		} else if (command == "DEPLACE") {
			double dx, dy;
			std::cin >> dx >> dy;
			centerX -= dx / zoom;
			centerY -= dy / zoom;
		} else if (command == "ZOOM") {
			double factor, pixelX, pixelY;
			std::cin >> factor >> pixelX >> pixelY;
			const double anchorX = PixelToWorld(pixelX, width, centerX, zoom);
			const double anchorY = PixelToWorld(pixelY, height, centerY, zoom);
			zoom *= factor;
			if (zoom < 0.02)
				zoom = 0.02;
			if (zoom > 64.0)
				zoom = 64.0;
			centerX = anchorX - (pixelX - width / 2.0) / zoom;
			centerY = anchorY - (pixelY - height / 2.0) / zoom;
		} else if (command == "GRILLE") {
			double minimumPixels;
			std::cin >> minimumPixels;
			double spacing = 1.0;
			while (spacing * zoom < minimumPixels)
				spacing *= 2.0;
			while (spacing * 0.5 * zoom >= minimumPixels)
				spacing *= 0.5;
			std::cout << "GRILLE ";
			PrintFixed(spacing, 6);
			std::cout << '\n';
			std::cout.flush();
		} else if (command == "ORIGINE") {
			centerX = 0.0;
			centerY = 0.0;
			zoom = 1.0;
		}
	}

	std::cout << "CENTRE ";
	PrintFixed(centerX, 2);
	std::cout << ' ';
	PrintFixed(centerY, 2);
	std::cout << "\nZOOM ";
	PrintFixed(zoom, 2);
	std::cout << '\n';
	return 0;
}

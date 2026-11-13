#include <iostream>
#include <string>

int main() {
	long long count;
	if (!(std::cin >> count))
		return 0;

	long long restored = 0;
	long long changes = 0;

	for (long long i = 1; i <= count; ++i) {
		long long id;
		double x, y, scale, rotation, opacity;
		int mirrorX, mirrorY, selected;
		long long width, height, bytes;
		std::string sourcePath;

		if (!(std::cin >> id >> x >> y >> scale >> rotation >> opacity >> mirrorX >> mirrorY >>
			  width >> height >> selected >> sourcePath >> bytes))
			return 0;

		if (bytes == 0) {
			std::cout << "ITEM " << i << " SAUTE\n";
			continue;
		}

		++restored;
		std::cout << "ITEM " << i << ' ' << restored;

		bool hasChange = false;
		if (id != 0) {
			std::cout << " id";
			++changes;
			hasChange = true;
		}
		if (selected == 1) {
			std::cout << " selected";
			++changes;
			hasChange = true;
		}
		if (sourcePath != "-") {
			std::cout << " sourcePath";
			++changes;
			hasChange = true;
		}
		if (!hasChange)
			std::cout << " AUCUN";
		std::cout << '\n';
	}

	std::cout << "RESTENT " << restored << '\n';
	std::cout << "CHANGES " << changes << '\n';
	return 0;
}

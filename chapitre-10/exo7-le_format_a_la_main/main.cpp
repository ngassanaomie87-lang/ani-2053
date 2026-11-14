#include <cstdint>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Reader {
    vector<uint8_t> data;
    size_t pos = 0;

    bool ensure(size_t n) const {
        return pos <= data.size() && n <= data.size() - pos;
    }

    bool readHexBytes(int n) {
        for (int i = 0; i < n; ++i) {
            string token;
            if (!(cin >> token) || token.size() != 2) {
                return false;
            }

            const auto hexValue = [](char digit) -> int {
                if (digit >= '0' && digit <= '9') {
                    return digit - '0';
                }
                if (digit >= 'A' && digit <= 'F') {
                    return digit - 'A' + 10;
                }
                return -1;
            };
            const int high = hexValue(token[0]);
            const int low = hexValue(token[1]);
            if (high < 0 || low < 0) {
                return false;
            }
            data.push_back(static_cast<uint8_t>((high << 4) | low));
        }
        return true;
    }

    uint8_t readU8() {
        if (!ensure(1)) {
            throw string("TRONQUE");
        }
        return data[pos++];
    }

    uint32_t readU32() {
        if (!ensure(4)) {
            throw string("TRONQUE");
        }
        uint32_t value = 0;
        value |= static_cast<uint32_t>(readU8());
        value |= static_cast<uint32_t>(readU8()) << 8;
        value |= static_cast<uint32_t>(readU8()) << 16;
        value |= static_cast<uint32_t>(readU8()) << 24;
        return value;
    }

    float readF32() {
        const uint32_t bits = readU32();
        float value = 0.0f;
        memcpy(&value, &bits, sizeof(value));
        return value;
    }

    void skip(size_t n) {
        if (!ensure(n)) {
            throw string("TRONQUE");
        }
        pos += n;
    }
};

static string formatFloat(float value) {
    if (std::isnan(value) || std::isinf(value)) {
        value = 0.0f;
    }
    ostringstream oss;
    oss << fixed << setprecision(2) << value;
    const string formatted = oss.str();
    return formatted == "-0.00" ? "0.00" : formatted;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 0;
    if (!(cin >> T)) {
        cout << "TRONQUE\n";
        return 0;
    }

    Reader reader;
    if (!reader.readHexBytes(T)) {
        cout << "TRONQUE\n";
        return 0;
    }

    if (reader.data.size() < 4) {
        cout << "TRONQUE\n";
        return 0;
    }

    if (reader.data[0] != 0x4E || reader.data[1] != 0x4B || reader.data[2] != 0x52 || reader.data[3] != 0x46) {
        cout << "PAS_UNE_PLANCHE\n";
        return 0;
    }

    try {
        ostringstream output;
        reader.pos = 4;
        const uint32_t version = reader.readU32();
        if (version != 1u) {
            cout << "VERSION_INCONNUE " << version << '\n';
            return 0;
        }

        const float cx = reader.readF32();
        const float cy = reader.readF32();
        const float zoom = reader.readF32();

        const uint8_t themeValue = reader.readU8();
        const uint8_t grilleValue = reader.readU8();
        reader.skip(2);

        const uint32_t imageCount = reader.readU32();

        output << "VUE " << formatFloat(cx) << ' ' << formatFloat(cy) << ' ' << formatFloat(zoom) << '\n';
        output << "THEME " << (themeValue == 0 ? "CLAIR" : "SOMBRE") << '\n';
        output << "GRILLE " << (grilleValue == 0 ? "AFFICHEE" : "CACHEE") << '\n';
        output << "IMAGES " << imageCount << '\n';

        for (uint32_t i = 0; i < imageCount; ++i) {
            const float x = reader.readF32();
            const float y = reader.readF32();
            const float echelle = reader.readF32();
            const float rotation = reader.readF32();
            const float opacite = reader.readF32();

            const uint8_t miroirX = reader.readU8();
            const uint8_t miroirY = reader.readU8();
            reader.skip(2);

            const uint32_t largeur = reader.readU32();
            const uint32_t hauteur = reader.readU32();
            const uint32_t taille = reader.readU32();

            const uint32_t sourceSize = taille;
            if (!reader.ensure(sourceSize)) {
                throw string("TRONQUE");
            }
            reader.skip(sourceSize);

            const int mirrorX = (miroirX != 0 ? 1 : 0);
            const int mirrorY = (miroirY != 0 ? 1 : 0);

            output << "IMAGE " << (i + 1) << ' ' << formatFloat(x) << ' ' << formatFloat(y) << ' '
                   << formatFloat(echelle) << ' ' << formatFloat(rotation) << ' ' << formatFloat(opacite) << ' '
                   << mirrorX << ' ' << mirrorY << ' ' << largeur << ' ' << hauteur << ' ' << taille << '\n';
        }

        const uint32_t traitCount = reader.readU32();
        output << "TRAITS " << traitCount << '\n';

        for (uint32_t i = 0; i < traitCount; ++i) {
            const uint8_t r = reader.readU8();
            const uint8_t g = reader.readU8();
            const uint8_t b = reader.readU8();
            const uint8_t a = reader.readU8();

            const float epaisseur = reader.readF32();
            const uint32_t pointCount = reader.readU32();

            output << "TRAIT " << (i + 1) << ' ' << static_cast<int>(r) << ' ' << static_cast<int>(g)
                   << ' ' << static_cast<int>(b) << ' ' << static_cast<int>(a) << ' ' << formatFloat(epaisseur)
                   << ' ' << pointCount << '\n';

            for (uint32_t j = 0; j < pointCount; ++j) {
                const float px = reader.readF32();
                const float py = reader.readF32();
                output << "POINT " << formatFloat(px) << ' ' << formatFloat(py) << '\n';
            }
        }
        cout << output.str();
    } catch (const string& e) {
        if (e == "TRONQUE") {
            cout << "TRONQUE\n";
                    } else {
            cout << e << '\n';
        }
    }

    return 0;
}
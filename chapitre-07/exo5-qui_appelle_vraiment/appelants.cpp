#include "NkCanvasApp.h"
#include "Renderer/Core/NkDrawable.h"
#include "Renderer/Targets/NkRenderTarget.h"
#include "Renderer/Core/NkRenderer2D.h"

class MonObjetDessinable : public NkDrawable {
public:
    void Draw(NkRenderTarget &target, const NkRenderStates &states) const override {
        NkVertex2D sommets[4] = {
            {0.0f, 0.0f, 0.0f, 0.0f, {255, 255, 255, 255}},
            {100.0f, 0.0f, 1.0f, 0.0f, {255, 255, 255, 255}},
            {100.0f, 100.0f, 1.0f, 1.0f, {255, 255, 255, 255}},
            {0.0f, 100.0f, 0.0f, 1.0f, {255, 255, 255, 255}}
        };
        target.Draw(sommets, 4, NkPrimitiveType::NK_LINES, states);
    }
};

class MonAppliAppelants : public NkCanvasApp {
private:
    MonObjetDessinable m_objet;

public:
    void OnRender(NkRenderTarget &target) override {
        target.Clear({0, 0, 0, 255});
        target.Draw(m_objet);
    }
};
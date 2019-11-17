#include "sys_render.h"

#include "common/math_utils.h"
#include "components/cmp_basic.h"
#include "components/cmp_render.h"

#include <SDL2/SDL_render.h>

void RectRendSys::update(entt::registry& reg, SDL_Renderer& sdl_renderer)
{
    SDL_Rect rect;
    auto view = reg.view<PositionCmp, RectRendCmp>();
    for (auto ent : view) {
        auto& posCmp = view.get<PositionCmp>(ent);
        auto& rectCmp = view.get<RectRendCmp>(ent);

        rect.w = static_cast<int>(posCmp.size.x());
        rect.h = static_cast<int>(posCmp.size.y());
        rect.x = static_cast<int>(posCmp.pos.x() - rect.w / 2);
        rect.y = static_cast<int>(posCmp.pos.y() - rect.h / 2);

        { // fill
            const Color& color = rectCmp.fillColor;
            SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
            SDL_RenderFillRect(&sdl_renderer, &rect);
        }

        { // border
            const Color& color = rectCmp.borderColor;
            SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
            SDL_RenderDrawRect(&sdl_renderer, &rect);
        }
    }
}

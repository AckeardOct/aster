#include "sys_render.h"

#include "../components/cmp_basic.h"
#include "../components/cmp_render.h"

#include <SDL2/SDL_render.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

void RectRendSys::update(entt::registry& reg, SDL_Renderer& sdl_renderer)
{
    SDL_Rect rect;
    auto view = reg.view<PositionCmp, RectRendCmp>();
    for (auto ent : view) {
        auto& posCmp = view.get<PositionCmp>(ent);
        auto& rectCmp = view.get<RectRendCmp>(ent);

        rect.w = (int)posCmp.size.x;
        rect.h = (int)posCmp.size.y;
        rect.x = (int)posCmp.pos.x - rect.w / 2;
        rect.y = (int)posCmp.pos.y - rect.h / 2;

        { // fill
            const glm::ivec4& color = rectCmp.fillColor;
            SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
            SDL_RenderFillRect(&sdl_renderer, &rect);
        }

        { // border
            const glm::ivec4& color = rectCmp.borderColor;
            SDL_SetRenderDrawColor(&sdl_renderer, (u_char)color.r, (u_char)color.g, (u_char)color.b, (u_char)color.a);
            SDL_RenderDrawRect(&sdl_renderer, &rect);
        }
    }
}

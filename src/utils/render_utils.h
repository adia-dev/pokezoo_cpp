#pragma once

#include <config/config.h>

namespace RenderUtils {

void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
                    int w, int h);

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                 SDL_Color color, int x, int y);

} // namespace RenderUtils
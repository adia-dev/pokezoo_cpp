#pragma once

#include <core/config.h>

namespace RenderUtils {

void render_texture(SDL_Renderer *renderer, SDL_Texture *texture,
                    SDL_Rect src_rect, SDL_Rect dst_rect, bool repeat = false);

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                 SDL_Color color, int x, int y);

void render_grid(SDL_Renderer *renderer, int width, int height, int tile_size,
                 const SDL_Color &color = {255, 255, 255, 255});

} // namespace RenderUtils
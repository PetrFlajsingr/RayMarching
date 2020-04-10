//
// Created by petr on 4/10/20.
//

#ifndef RAYMARCHING_KEYBOARDHANDLER_H
#define RAYMARCHING_KEYBOARDHANDLER_H
#include <SDL_events.h>

class KeyboardHandler {
public:
  auto operator()(const SDL_Event &event) -> bool;

private:
  const Uint8 *keyboardStateArray = SDL_GetKeyboardState(nullptr);
};

#endif // RAYMARCHING_KEYBOARDHANDLER_H

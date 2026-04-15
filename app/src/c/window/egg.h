#pragma once
#include <pebble.h>
#include "../state/global.h"

void egg_init(State* state);

void egg_load(Window* window);

void egg_unload();

void egg_render();

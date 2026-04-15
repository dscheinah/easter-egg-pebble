#pragma once
#include <pebble.h>
#include "../state/global.h"

TextLayer* helper_text_layer_create(Layer* parent, GRect rect, GFont font, char* text);

BitmapLayer* helper_bitmap_layer_create(Layer* parent, GRect rect, GBitmap* bitmap);

Layer* helper_layer_create(Layer* parent, GRect rect, LayerUpdateProc update_proc);

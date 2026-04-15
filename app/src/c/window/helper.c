#include <pebble.h>
#include "helper.h"

TextLayer* helper_text_layer_create(Layer* parent, GRect rect, GFont font, char* text) {
  TextLayer* layer = text_layer_create(rect);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  text_layer_set_background_color(layer, GColorBlack);
  text_layer_set_text_color(layer, GColorWhite);
  layer_add_child(parent, text_layer_get_layer(layer));
  return layer;
}

BitmapLayer* helper_bitmap_layer_create(Layer* parent, GRect rect, GBitmap* bitmap) {
  BitmapLayer* layer = bitmap_layer_create(rect);
  bitmap_layer_set_bitmap(layer, bitmap);
  bitmap_layer_set_compositing_mode(layer, GCompOpSet);
  layer_add_child(parent, bitmap_layer_get_layer(layer));
  return layer;
}

Layer* helper_layer_create(Layer* parent, GRect rect, LayerUpdateProc update_proc) {
  Layer* layer = layer_create(rect);
  layer_set_update_proc(layer, update_proc);
  layer_add_child(parent, layer);
  return layer;
}

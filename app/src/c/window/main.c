#include <pebble.h>
#include "main.h"
#include "helper.h"

static State* global;

static Layer* layer;

static BitmapLayer* bitmap_layer_egg;
static BitmapLayer* bitmap_layer_clouds;
static BitmapLayer* bitmap_layer_ears;
static BitmapLayer* bitmap_layer_eyes;
static BitmapLayer* bitmap_layer_flowers;
static BitmapLayer* bitmap_layer_whiskers;

static GBitmap* bitmap_egg;
static GBitmap* bitmap_clouds;
static GBitmap* bitmap_ears;
static GBitmap* bitmap_eyes;
static GBitmap* bitmap_flowers;
static GBitmap* bitmap_whiskers;

static void render(Layer* layer, GContext* ctx) {
  srand(global->seed);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorWhite);

  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, GColorFromRGB(rand() % 155 + 50, rand() % 155 + 50, rand() % 155 + 50));
  graphics_fill_rect(ctx, bounds, 0, GCornersAll);


  graphics_context_set_fill_color(ctx, GColorFromRGB(rand() % 155 + 50, rand() % 155 + 50, rand() % 155 + 50));
  for (int i = 0; i < 10; i++) {
    GRect target = GRect(i * bounds.size.w / (rand() % 10), 0, rand() % (bounds.size.w / 10), bounds.size.h);
    graphics_draw_rect(ctx, target);
    graphics_fill_rect(ctx, GRect(target.origin.x + 2, target.origin.y, target.size.w - 4, target.size.h), 0, GCornerNone);
  }

  graphics_context_set_fill_color(ctx, GColorFromRGB(rand() % 155 + 50, rand() % 155 + 50, rand() % 155 + 50));
  for (int i = 0; i < 10; i++) {
    GRect target = GRect(0, i * bounds.size.h / (rand() % 10), bounds.size.w, rand() % (bounds.size.h / 10));
    graphics_draw_rect(ctx, target);
    graphics_fill_rect(ctx, GRect(target.origin.x, target.origin.y + 2, target.size.w, target.size.h - 4), 0, GCornerNone);
  }

  graphics_context_set_fill_color(ctx, GColorFromRGB(rand() % 155 + 50, rand() % 155 + 50, rand() % 155 + 50));
  for (int i = 0; i < 10; i++) {
    GPoint target = GPoint(rand() % bounds.size.w, rand() % bounds.size.h);
    graphics_draw_circle(ctx, target, 10);
    graphics_fill_circle(ctx, target, 9);
  }
}

void main_init(State* state) {
  global = state;
}

void main_load(Window* window) {
  window_set_background_color(window, GColorWhite);

  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  bitmap_egg = gbitmap_create_with_resource(RESOURCE_ID_EGG);
  bitmap_clouds = gbitmap_create_with_resource(RESOURCE_ID_CLOUDS);
  bitmap_ears = gbitmap_create_with_resource(RESOURCE_ID_EARS);
  bitmap_eyes = gbitmap_create_with_resource(RESOURCE_ID_EYES);
  bitmap_flowers = gbitmap_create_with_resource(RESOURCE_ID_FLOWERS);
  bitmap_whiskers = gbitmap_create_with_resource(RESOURCE_ID_WHISKERS);

  layer = helper_layer_create(window_layer, bounds, render);

  bitmap_layer_egg = helper_bitmap_layer_create(window_layer, bounds, bitmap_egg);
  bitmap_layer_clouds = helper_bitmap_layer_create(window_layer, bounds, bitmap_clouds);
  bitmap_layer_ears = helper_bitmap_layer_create(window_layer, bounds, bitmap_ears);
  bitmap_layer_eyes = helper_bitmap_layer_create(window_layer, bounds, bitmap_eyes);
  bitmap_layer_flowers = helper_bitmap_layer_create(window_layer, bounds, bitmap_flowers);
  bitmap_layer_whiskers = helper_bitmap_layer_create(window_layer, bounds, bitmap_whiskers);
}

void main_unload() {
  layer_destroy(layer);

  bitmap_layer_destroy(bitmap_layer_egg);
  bitmap_layer_destroy(bitmap_layer_clouds);
  bitmap_layer_destroy(bitmap_layer_ears);
  bitmap_layer_destroy(bitmap_layer_eyes);
  bitmap_layer_destroy(bitmap_layer_flowers);
  bitmap_layer_destroy(bitmap_layer_whiskers);

  gbitmap_destroy(bitmap_egg);
  gbitmap_destroy(bitmap_clouds);
  gbitmap_destroy(bitmap_ears);
  gbitmap_destroy(bitmap_eyes);
  gbitmap_destroy(bitmap_flowers);
  gbitmap_destroy(bitmap_whiskers);
}

void main_render() {
  layer_mark_dirty(layer);
}

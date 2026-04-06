#include <pebble.h>
#include "main.h"
#include "helper.h"

#define LEVEL_H_BAR    2
#define LEVEL_EGG_R    3
#define LEVEL_EGG_G    4
#define LEVEL_EGG_B    5
#define LEVEL_EYES     6
#define LEVEL_CLOUDS   7
#define LEVEL_DOTS     8
#define LEVEL_V_BAR    9
#define LEVEL_H_BAR_B  10
#define LEVEL_DOTS_B   11
#define LEVEL_V_BAR_B  12
#define LEVEL_EARS     13
#define LEVEL_H_BAR_G  14
#define LEVEL_DOTS_G   15
#define LEVEL_V_BAR_G  16
#define LEVEL_FLOWERS  17
#define LEVEL_H_BAR_R  18
#define LEVEL_DOTS_R   19
#define LEVEL_V_BAR_R  20
#define LEVEL_WHISKERS 21

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

static int get_color_value(int level) {
  int color_value = rand() % 155 + 50;
  if (global->level < level) {
    return 255;
  }
  return color_value;
}

static void render(Layer* layer, GContext* ctx) {
  srand(global->seed);
  bool show_h_bar = rand() % 2 && global->level >= LEVEL_H_BAR;
  bool show_v_bar = rand() % 2 && global->level >= LEVEL_V_BAR;
  bool show_dots = rand() % 2 && global->level >= LEVEL_DOTS;
  int egg_r = get_color_value(LEVEL_EGG_R);
  int egg_g = get_color_value(LEVEL_EGG_G);
  int egg_b = get_color_value(LEVEL_EGG_B);
  int h_bar_r = get_color_value(LEVEL_H_BAR_R);
  int h_bar_g = get_color_value(LEVEL_H_BAR_G);
  int h_bar_b = get_color_value(LEVEL_H_BAR_B);
  int v_bar_r = get_color_value(LEVEL_V_BAR_R);
  int v_bar_g = get_color_value(LEVEL_V_BAR_G);
  int v_bar_b = get_color_value(LEVEL_V_BAR_B);
  int dot_r = get_color_value(LEVEL_DOTS_R);
  int dot_g = get_color_value(LEVEL_DOTS_G);
  int dot_b = get_color_value(LEVEL_DOTS_B);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorWhite);

  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, GColorFromRGB(egg_r, egg_g, egg_b));
  graphics_fill_rect(ctx, bounds, 0, GCornersAll);

  if (show_v_bar) {
    graphics_context_set_fill_color(ctx, GColorFromRGB(v_bar_r, v_bar_g, v_bar_b));
    for (int i = 0; i < 10; i++) {
      GRect target = GRect(i * bounds.size.w / (rand() % 10), 0, rand() % (bounds.size.w / 10), bounds.size.h);
      graphics_draw_rect(ctx, target);
      graphics_fill_rect(ctx, GRect(target.origin.x + 2, target.origin.y, target.size.w - 4, target.size.h), 0, GCornerNone);
    }
  }

  if (show_h_bar) {
    graphics_context_set_fill_color(ctx, GColorFromRGB(h_bar_r, h_bar_g, h_bar_b));
    for (int i = 0; i < 10; i++) {
      GRect target = GRect(0, i * bounds.size.h / (rand() % 10), bounds.size.w, rand() % (bounds.size.h / 10));
      graphics_draw_rect(ctx, target);
      graphics_fill_rect(ctx, GRect(target.origin.x, target.origin.y + 2, target.size.w, target.size.h - 4), 0, GCornerNone);
    }
  }

  if (show_dots) {
    graphics_context_set_fill_color(ctx, GColorFromRGB(dot_r, dot_g, dot_b));
    for (int i = 0; i < 10; i++) {
      GPoint target = GPoint(rand() % bounds.size.w, rand() % bounds.size.h);
      graphics_draw_circle(ctx, target, 10);
      graphics_fill_circle(ctx, target, 9);
    }
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

  main_render();
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
  srand(global->seed);
  bool show_eyes = rand() % 2 && global->level > LEVEL_EYES;
  bool show_clouds = rand() % 2 && global->level > LEVEL_CLOUDS;
  bool show_ears = rand() % 2 && global->level > LEVEL_EARS;
  bool show_flowers = rand() % 2 && global->level > LEVEL_FLOWERS;
  bool show_whiskers = rand() % 2 && global->level > LEVEL_WHISKERS;

  layer_set_hidden(bitmap_layer_get_layer(bitmap_layer_eyes), !(show_eyes || global->level == LEVEL_EYES));
  layer_set_hidden(bitmap_layer_get_layer(bitmap_layer_clouds), !(show_clouds || global->level == LEVEL_CLOUDS));
  layer_set_hidden(bitmap_layer_get_layer(bitmap_layer_ears), !(show_ears || global->level == LEVEL_EARS));
  layer_set_hidden(bitmap_layer_get_layer(bitmap_layer_flowers), !(show_flowers || global->level == LEVEL_FLOWERS));
  layer_set_hidden(bitmap_layer_get_layer(bitmap_layer_whiskers), !(show_whiskers || global->level == LEVEL_WHISKERS));

  layer_mark_dirty(layer);
}

#include <pebble.h>
#include "egg.h"
#include "main.h"
#include "helper.h"

#define BUFFER_SIZE 128

static State* global;

static Layer* layer;

static TextLayer* text_layer;

static ActionBarLayer* action_bar;

static GBitmap* action_bar_icon_up;
static GBitmap* action_bar_icon_dismiss;
static GBitmap* action_bar_icon_down;

static char buffer[BUFFER_SIZE] = "";

static void render(Layer* layer, GContext* ctx) {
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(bounds.size.w + 2, bounds.size.h / 2), 10);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  global->seed--;
  egg_render();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  Layer* layer = action_bar_layer_get_layer(action_bar);
  bool hidden = !layer_get_hidden(layer);
  layer_set_hidden(layer, hidden);
  layer_set_hidden(text_layer_get_layer(text_layer), hidden);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  global->seed++;
  egg_render();
}

static void action_bar_click_config_provider(void *context) {
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 333, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 333, down_click_handler);
}

void main_init(State* state) {
  global = state;
  egg_init(state);

  int exp = global->exp / global->level;

  int done = global->quest_target_done;
  int target = global->quest_target;
  char quest[8] = "";
  char unit[4] = "";
  char counter[10] = "";
  switch (global->quest) {
    case QUEST_STEPS:
      strncpy(quest, "Steps", 8);
      break;
    case QUEST_ACTIVE:
      strncpy(quest, "Active", 8);
      strncpy(unit, "min", 4);
      done /= SECONDS_PER_MINUTE;
      target /= SECONDS_PER_MINUTE;
      break;
    case QUEST_SLEEP:
      strncpy(quest, "Sleep", 8);
      strncpy(unit, "h", 4);
      done /= SECONDS_PER_HOUR;
      target /= SECONDS_PER_HOUR;
      break;
    case QUEST_RESTFUL:
      strncpy(quest, "Restful", 8);
      strncpy(unit, "h", 4);
      done /= SECONDS_PER_HOUR;
      target /= SECONDS_PER_HOUR;
      break;
    case QUEST_STEPS_TIME:
      strncpy(quest, "Steps", 8);
      strncpy(unit, "min", 4);
      done = global->quest_duration_done / SECONDS_PER_MINUTE;
      target = global->quest_duration / SECONDS_PER_MINUTE;
      snprintf(counter, 10, "%d", (int) global->quest_target_done);
      break;
    case QUEST_ALL_TIME:
      strncpy(quest, "Health", 8);
      strncpy(unit, "h", 4);
      done = global->quest_duration_done / SECONDS_PER_HOUR;
      target = global->quest_duration / SECONDS_PER_HOUR;
      snprintf(counter, 10, "%d", (int) global->quest_target_done / 100);
      break;
  }

  snprintf(buffer, BUFFER_SIZE, "Level %d\nExp %d%%\nQuest %s\n%d/%d %s\n%s",
    global->level,
    exp,
    quest,
    done,
    target,
    unit,
    counter
  );
}

void main_load(Window* window) {
  egg_load(window);
  egg_render();

  bool hide_status = global->result != HEALTH_RESULT_QUEST;

  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  layer = helper_layer_create(window_layer, bounds, render);

  action_bar_icon_up = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ICON_UP);
  action_bar_icon_dismiss = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ICON_DISMISS);
  action_bar_icon_down = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ICON_DOWN);

  action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(action_bar, action_bar_click_config_provider);
  action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_UP, action_bar_icon_up, true);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, action_bar_icon_dismiss);
  action_bar_layer_set_icon_press_animation(action_bar, BUTTON_ID_SELECT, ActionBarLayerIconPressAnimationNone);
  action_bar_layer_set_icon_animated(action_bar, BUTTON_ID_DOWN, action_bar_icon_down, true);

  layer_set_hidden(action_bar_layer_get_layer(action_bar), hide_status);
  action_bar_layer_add_to_window(action_bar, window);

  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  int start = PBL_IF_RECT_ELSE(PBL_DISPLAY_WIDTH / 12, PBL_DISPLAY_WIDTH / 6);
  int size = 85;
  text_layer = helper_text_layer_create(window_layer, GRect(start, start, size, size), font, buffer);
  layer_set_hidden(text_layer_get_layer(text_layer), hide_status);
}

void main_unload() {
  layer_destroy(layer);

  action_bar_layer_remove_from_window(action_bar);
  action_bar_layer_destroy(action_bar);

  gbitmap_destroy(action_bar_icon_up);
  gbitmap_destroy(action_bar_icon_dismiss);
  gbitmap_destroy(action_bar_icon_down);

  text_layer_destroy(text_layer);

  egg_unload();
}

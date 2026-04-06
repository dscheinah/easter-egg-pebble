#include <pebble.h>
#include "health/health.h"
#include "state/global.h"
#include "state/state.h"
#include "window/main.h"

static Window *s_window;

State* state;

static const uint32_t durations[] = {300, 50, 50, 50, 50, 50, 50};
static const VibePattern vibes = {
  .durations = durations,
  .num_segments = ARRAY_LENGTH(durations),
};

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  state->seed--;
  main_render();
}

static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  state->seed++;
  main_render();
}

static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
}

static void prv_init(void) {
  state = state_init();
  if (health_init(state) && !quiet_time_is_active()) {
    vibes_enqueue_custom_pattern(vibes);
  }
  main_init(state);

  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_load,
    .unload = main_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

static void prv_deinit(void) {
  window_destroy(s_window);

  state_write();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}

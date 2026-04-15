#include <pebble.h>
#include "health/health.h"
#include "state/global.h"
#include "state/state.h"
#include "wakeup/wakeup.h"
#include "window/main.h"

static Window *s_window;

State* state;

static const uint32_t durations[] = {400, 100, 100, 100, 100, 100, 100};
static const VibePattern vibes = {
  .durations = durations,
  .num_segments = ARRAY_LENGTH(durations),
};

static void prv_init(void) {
  state = state_init();
  bool notify = health_init(state);
  bool wakeup = wakeup_init(state);
  if (notify && !quiet_time_is_active()) {
    vibes_enqueue_custom_pattern(vibes);
  }
  if (wakeup && !notify) {
    return;
  }
  main_init(state);

  s_window = window_create();
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

#include <pebble.h>
#include "health/health.h"
#include "state/global.h"
#include "state/state.h"
#include "wakeup/wakeup.h"
#include "window/main.h"

static Window *s_window;

State* state;

static const uint32_t durations_level[] = {400, 100, 100, 100, 100, 100, 100};
static const VibePattern vibes_level = {
  .durations = durations_level,
  .num_segments = ARRAY_LENGTH(durations_level),
};

static const uint32_t durations_quest[] = {100, 100, 100, 100, 100};
static const VibePattern vibes_quest = {
  .durations = durations_quest,
  .num_segments = ARRAY_LENGTH(durations_quest),
};

static void prv_init(void) {
  state = state_init();
  health_init(state);

  if (!quiet_time_is_active()) {
    switch (state->result) {
      case HEALTH_RESULT_LEVEL:
        vibes_enqueue_custom_pattern(vibes_level);
        break;
      case HEALTH_RESULT_QUEST:
        vibes_enqueue_custom_pattern(vibes_quest);
        break;
    }
  }

  if (wakeup_init(state) && !state->result) {
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

  wakeup_deinit();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}

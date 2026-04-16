#include <pebble.h>
#include "state.h"

#define VERSION_KEY  0
#define STATE_KEY    1
#define HEALTH_KEY   2

static Health health = {
  .steps = 0,
  .active = 0,
  .sleep = 0,
  .restful = 0,
};

static State state = {
  .seed = 0,
  .day = 0,
  .level = 1,
  .exp = 0,
  .wakeup = -1,
  .quest = 0,
  .quest_duration = 0,
  .quest_target = 0,
  .quest_duration_done = 0,
  .quest_target_done = 0,
  .quest_duration_start = 0,
  .result = HEALTH_RESULT_NONE,
};

State* state_init() {
  if (persist_exists(VERSION_KEY)) {
    persist_read_data(STATE_KEY, &state, sizeof(State));
    persist_read_data(HEALTH_KEY, &health, sizeof(Health));
  }
  state.health = &health;
  return &state;
}

void state_write() {
  persist_write_int(VERSION_KEY, 1);
  persist_write_data(STATE_KEY, &state, sizeof(State));
  persist_write_data(HEALTH_KEY, &health, sizeof(Health));
}

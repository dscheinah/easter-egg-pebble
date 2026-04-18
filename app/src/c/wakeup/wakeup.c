#include <pebble.h>
#include "wakeup.h"

#define MAX_TRY 23
#define BUFFER_SIZE 32

static State* global;

static char buffer[BUFFER_SIZE] = "";

static void app_glance(AppGlanceReloadSession* session, size_t limit, void* context) {
  if (limit < 1) {
    return;
  }

  char quest[8] = "";
  int done = 0;
  switch (global->quest) {
    case QUEST_STEPS:
      strncpy(quest, "Steps", 8);
      done = global->quest_target_done * 100 / global->quest_target;
      break;
    case QUEST_ACTIVE:
      strncpy(quest, "Active", 8);
      done = global->quest_target_done * 100 / global->quest_target;
      break;
    case QUEST_SLEEP:
      strncpy(quest, "Sleep", 8);
      done = global->quest_target_done * 100 / global->quest_target;
      break;
    case QUEST_RESTFUL:
      strncpy(quest, "Restful", 8);
      done = global->quest_target_done * 100 / global->quest_target;
      break;
    case QUEST_STEPS_TIME:
    case QUEST_ALL_TIME:
      strncpy(quest, "Timer", 8);
      done = global->quest_duration_done * 100 / global->quest_duration;
      break;
  }

  snprintf(buffer, BUFFER_SIZE, "@%d - %s %d%%", global->level, quest, done);

  const AppGlanceSlice slice = {
    .layout = {
      .icon = APP_GLANCE_SLICE_DEFAULT_ICON,
      .subtitle_template_string = buffer,
    },
    .expiration_time = APP_GLANCE_SLICE_NO_EXPIRATION,
  };
  app_glance_add_slice(session, slice);
}

static void wakeup_handler() {
  if (global->wakeup >= 0 && wakeup_query(global->wakeup, NULL)) {
    return;
  }
  time_t timeout = time(NULL) + SECONDS_PER_HOUR + (rand() % SECONDS_PER_HOUR);
  for (int try = 0; try < MAX_TRY; try++) {
    global->wakeup = wakeup_schedule(timeout, 0, true);
    if (global->wakeup >= 0) {
      break;
    }
    timeout += SECONDS_PER_MINUTE;
  }
}

bool wakeup_init(State* state) {
  global = state;

  app_glance_reload(app_glance, NULL);

  wakeup_service_subscribe(wakeup_handler);
  wakeup_handler();

  return launch_reason() == APP_LAUNCH_WAKEUP;
}

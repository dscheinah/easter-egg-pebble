#include <pebble.h>
#include "wakeup.h"

#define MAX_TRY 23
#define TIMEOUT 30000

static State* global;

static void timeout() {
  window_stack_pop_all(false);
}

static void wakeup_handler() {
  if (global->wakeup >= 0 && wakeup_query(global->wakeup, NULL)) {
    return;
  }
  time_t timeout = time(NULL) + (rand() % SECONDS_PER_DAY);
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

  wakeup_service_subscribe(wakeup_handler);
  wakeup_handler();

  if (launch_reason() != APP_LAUNCH_WAKEUP) {
    return false;
  }
  exit_reason_set(APP_EXIT_ACTION_PERFORMED_SUCCESSFULLY);
  app_timer_register(TIMEOUT, timeout, NULL);
  return true;
}

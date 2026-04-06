#include <pebble.h>
#include "health.h"

bool health_init(State* state) {
  time_t now = time(NULL);
  tm* local = localtime(&now);
  if (state->day != local->tm_mday) {
    state->health->steps = 0;
    state->health->active = 0;
    state->health->sleep = 0;
    state->health->restful = 0;
    state->day = local->tm_mday;
  }

  HealthValue steps = health_service_sum_today(HealthMetricStepCount);
  HealthValue active = health_service_sum_today(HealthMetricActiveSeconds);
  HealthValue sleep = health_service_sum_today(HealthMetricSleepSeconds);
  HealthValue restful = health_service_sum_today(HealthMetricSleepRestfulSeconds);

  unsigned int exp = state->exp
    + (abs(steps - state->health->steps) / 1000)
    + (abs(active - state->health->active) / SECONDS_PER_MINUTE / 10)
    + (abs(sleep - state->health->sleep) / SECONDS_PER_HOUR)
    + (abs(restful - state->health->restful) / SECONDS_PER_HOUR);

  if (exp != state->exp) {
    state->health->steps = steps;
    state->health->active = active;
    state->health->sleep = sleep;
    state->health->restful = restful;
    state->exp = exp;
  }

  if (state->exp >= state->level * 100) {
    state->exp -= state->level * 100;
    state->level++;
    return true;
  }
  return false;
}

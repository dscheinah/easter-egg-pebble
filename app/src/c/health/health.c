#include <pebble.h>
#include "health.h"
#include "../state/global.h"

static State* global;

static Health current = {
  .steps = 0,
  .active = 0,
  .sleep = 0,
  .restful = 0,
};

static void create_quest() {
  global->quest_duration_start = time(NULL);

  srand(global->quest_duration_start / SECONDS_PER_HOUR);

  global->quest = rand() % QUEST_COUNT + 1;
  global->quest_duration_done = 0;
  global->quest_target_done = 0;

  switch (global->quest) {
    case QUEST_STEPS:
      global->quest_target = (rand() % 100 + 10) * 100;
      break;
    case QUEST_ACTIVE:
      global->quest_target = (rand() % 120 + 1) * SECONDS_PER_MINUTE;
      break;
    case QUEST_SLEEP:
      global->quest_target = (rand() % 12 + 1) * SECONDS_PER_HOUR;
      break;
    case QUEST_RESTFUL:
      global->quest_target = (rand() % 6 + 1) * SECONDS_PER_HOUR;
      break;
    case QUEST_STEPS_TIME:
      global->quest_duration = (rand() % 3 + 1) * SECONDS_PER_HOUR;
      break;
    case QUEST_ALL_TIME:
      global->quest_duration = (rand() % 24 + 1) * SECONDS_PER_HOUR;
      break;
  }
}

static void update_target_done() {
  switch (global->quest) {
    case QUEST_STEPS:
    case QUEST_STEPS_TIME:
      global->quest_target_done += abs(current.steps - global->health->steps);
      break;
    case QUEST_ACTIVE:
      global->quest_target_done += abs(current.active - global->health->active);
      break;
    case QUEST_SLEEP:
      global->quest_target_done += abs(current.sleep - global->health->sleep);
      break;
    case QUEST_RESTFUL:
      global->quest_target_done += abs(current.restful - global->health->restful);
      break;
    case QUEST_ALL_TIME:
      global->quest_target_done += abs(current.steps - global->health->steps);
      global->quest_target_done += abs(current.active - global->health->active);
      global->quest_target_done += abs(current.sleep - global->health->sleep);
      global->quest_target_done += abs(current.restful - global->health->restful);
      break;
  }
}

static bool update_quest() {
  if (!global->quest) {
    create_quest();
    return false;
  }

  switch (global->quest) {
    case QUEST_STEPS:
      if (global->quest_target_done >= global->quest_target) {
        global->exp += global->quest_target / 100;
        create_quest();
      }
      break;
    case QUEST_ACTIVE:
      if (global->quest_target_done >= global->quest_target) {
        global->exp += global->quest_target / SECONDS_PER_MINUTE;
        create_quest();
      }
      break;
    case QUEST_SLEEP:
      if (global->quest_target_done >= global->quest_target) {
        global->exp += global->quest_target / SECONDS_PER_MINUTE / 6;
        create_quest();
      }
      break;
    case QUEST_RESTFUL:
      if (global->quest_target_done >= global->quest_target) {
        global->exp += global->quest_target / SECONDS_PER_MINUTE / 3;
        create_quest();
      }
      break;
    case QUEST_STEPS_TIME:
    case QUEST_ALL_TIME:
      if (global->quest_duration_done >= global->quest_duration) {
        global->exp += global->quest_target_done / 100;
        create_quest();
      }
      break;
  }

  if (global->exp >= global->level * 100) {
    global->exp -= global->level * 100;
    global->level++;
    return true;
  }
  return false;
}

bool health_init(State* state) {
  global = state;

  time_t now = time(NULL);
  tm* local = localtime(&now);

  if (global->day != local->tm_mday) {
    time_t end = time_start_of_today();
    time_t start = end - SECONDS_PER_DAY;

    current.steps = health_service_sum(HealthMetricStepCount, start, end),
    current.active = health_service_sum(HealthMetricActiveSeconds, start, end),
    current.sleep = health_service_sum(HealthMetricSleepSeconds, start, end),
    current.restful = health_service_sum(HealthMetricSleepRestfulSeconds, start, end),
    update_target_done();

    global->health->steps = 0;
    global->health->active = 0;
    global->health->sleep = 0;
    global->health->restful = 0;

    global->day = local->tm_mday;
  }

  current.steps = health_service_sum_today(HealthMetricStepCount),
  current.active = health_service_sum_today(HealthMetricActiveSeconds),
  current.sleep = health_service_sum_today(HealthMetricSleepSeconds),
  current.restful = health_service_sum_today(HealthMetricSleepRestfulSeconds),
  update_target_done();

  global->quest_duration_done = now - global->quest_duration_start;

  global->health->steps = current.steps;
  global->health->active = current.active;
  global->health->sleep = current.sleep;
  global->health->restful = current.restful;

  return update_quest();
}

#pragma once
#include <pebble.h>

#define QUEST_COUNT 6

#define QUEST_STEPS      1
#define QUEST_ACTIVE     2
#define QUEST_SLEEP      3
#define QUEST_RESTFUL    4
#define QUEST_STEPS_TIME 5
#define QUEST_ALL_TIME   6

#define HealthResult unsigned short

#define HEALTH_RESULT_NONE  0
#define HEALTH_RESULT_QUEST 1
#define HEALTH_RESULT_LEVEL 2

typedef struct {
  HealthValue steps;
  HealthValue active;
  HealthValue sleep;
  HealthValue restful;
} Health;

typedef struct {
  unsigned int seed;
  unsigned short day;
  unsigned short level;
  unsigned int exp;
  Health* health;
  WakeupId wakeup;
  unsigned short quest;
  unsigned short quest_duration;
  HealthValue quest_target;
  unsigned short quest_duration_done;
  HealthValue quest_target_done;
  time_t quest_duration_start;
  HealthResult result;
} State;

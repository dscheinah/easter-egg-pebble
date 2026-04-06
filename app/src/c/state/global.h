#pragma once
#include <pebble.h>

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
} State;

#ifndef PERIODIC_TIMER_BASE_H
#define PERIODIC_TIMER_BASE_H

#include <iostream>
#include <chrono>
#include <functional>

/* Duration type */
typedef std::chrono::milliseconds Duration;

/* Function handler */
typedef std::function<void()> FunHandler;

/* Timestamp. According to cppref this one is the best for measuring intervals. */
typedef std::chrono::time_point<std::chrono::system_clock> Timestamp;

/* Execution status enum */
enum class Status : std::uint8_t {
  OK = 0,
  ERROR = 1,
};

/* Timer status enum */
enum class TimerStatus : std::uint8_t {
  ACTIVE = 0,
  INACTIVE = 1,
};

class PeriodicTimerBase
{
public:
  PeriodicTimerBase() {}
  virtual ~PeriodicTimerBase() {}

  virtual Status Start() = 0;
  virtual Status Stop() = 0;
  virtual TimerStatus is_active()=0;
  virtual Status status()=0;

private:



};
#endif
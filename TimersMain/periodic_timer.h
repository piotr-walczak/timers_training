#ifndef PERIODIC_TIMER_H
#define PERIODIC_TIMER_H

#include "periodic_timer_base.h"
#include <mutex>



class PeriodicTimer : public PeriodicTimerBase
{
public:
  PeriodicTimer(Duration period, FunHandler fhandler, Duration delay = Duration(0), bool is_periodic = false);
  ~PeriodicTimer();

  virtual Status Start() override;
  virtual Status Stop() override;
  virtual TimerStatus is_active() override;
  virtual Status status() override;
  bool is_periodic() { return is_periodic_; }
  

private:
  void ThreadHandler();
  void CallFunction();
  /* Keep the delay of timer */
  Duration start_after_{};
  /* Keep the period of timer */
  Duration period_{};
  /* Function to be called */
  FunHandler handler_{ nullptr };
  /* Execution timestamp */
  Timestamp exec_time_{};
  /* Thread manager */
  std::thread worker_{};
  /* Indicate if timer is registered */
  bool registered_{ false };
  /* It is one_shot or periodic timer */
  bool is_periodic_{ false };
  /**/
  bool finished_{ false };
  /**/
  bool destroy_{ false };
  /* Timer's status */
  TimerStatus is_active_{TimerStatus::INACTIVE};
  /* Status of last operation */
  Status status_{ Status::ERROR };
  std::mutex mutex_{};
  std::condition_variable cv_{};


};
#endif // !PERIODIC_TIMER_H

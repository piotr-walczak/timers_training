#include "periodic_timer.h"

/*
Constructor of Periodic Timer
@param [in] period      Timer period time [ms]
@param [in] fhandler    Timer's callback
@param [in] delay       Timer execution time delay [ms]
@param [in] is_periodic Indicates if timer will run in one shot mode or periodic
*/
PeriodicTimer::PeriodicTimer(Duration period, FunHandler fhandler, Duration delay, bool is_periodic)
{
  period_ = period;
  start_after_ = delay;
  exec_time_ = std::chrono::system_clock::now() + delay;
  is_periodic_ = is_periodic;
  handler_ = fhandler;
  worker_ = std::thread(std::bind(&PeriodicTimer::ThreadHandler, this));
  registered_ = worker_.joinable();
  destroy_ = false;

  if (nullptr != fhandler)
  {
    status_ = Status::OK;
  }
}

/*
Destructor of Periodic Timer
Ensure that threads will be closed properly
*/
PeriodicTimer::~PeriodicTimer()
{
  cv_.notify_all();

  if (true == registered_)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    destroy_ = true;
    if (false == finished_)
    {
      cv_.wait(lock);
    }
    worker_.join();
  }

}

/*
Start method of Periodic Timer
*/
Status PeriodicTimer::Start()
{
  if (false == registered_)
  {
    return (status_ = Status::ERROR);
  }
  if (TimerStatus::INACTIVE != is_active_)
  {
    /* Timer's already active */
    return (status_ = Status::ERROR);
  }

  /* Set timer's status to active */
  is_active_ = TimerStatus::ACTIVE;
  finished_ = false;
  return (status_ = Status::OK);
}

/*
Stop method of Periodic Timer
*/
Status PeriodicTimer::Stop()
{
  if (TimerStatus::ACTIVE != is_active_)
  {
    /* Timer's inactive */
    return (status_ = Status::ERROR);
  }

  /* Set timer's status to inactive */
  is_active_ = TimerStatus::INACTIVE;
  finished_ = true;
  return (status_ = Status::OK);
}

/*
Function handler
*/
void PeriodicTimer::CallFunction()
{
  mutex_.lock();
  handler_();
  mutex_.unlock();
}

/*
Thread handler
*/
void PeriodicTimer::ThreadHandler()
{
  /* Make sure that we wont be able to start timer without start, and after destroying it. */
  while (!finished_ && !destroy_)
  {
    Timestamp current_time = std::chrono::system_clock::now();
    if (current_time >= exec_time_)
    {
      /* Call function if timer's active and time is correct */
      if (TimerStatus::ACTIVE == is_active_)
      {
        CallFunction();
        /* if periodic, sleep thread and call function again */
        if (is_periodic_) {
          std::this_thread::sleep_for(std::chrono::milliseconds(period_));
        }
        /* if one shot, or stopped, set proper flags and leave if statement*/
        else
        {
          is_active_ = TimerStatus::INACTIVE;
          finished_ = true;
        }
      }
      /* yield, so other threads won't be blocked */
      else
      {
        mutex_.lock();
        std::this_thread::yield();
        mutex_.unlock();
      }
    }
  }
  /* unlock other threads */
  cv_.notify_all();
}

TimerStatus PeriodicTimer::is_active() { return is_active_; }
Status PeriodicTimer::status() { return status_; }
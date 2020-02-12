#include "pch.h"
#include "../TimersMain/periodic_timer.h"
#include "../TimersMain/periodic_timer.cpp"


/*
Test for constructor - positive case.
Testing procedure:
Create timer/timers,
Check if all the variables are set correctly
*/
TEST(PeriodicTimerTest, Constructor_Positive)
{
  /* Test Scenario no 1: Create timer */
  {
    PeriodicTimer tim(std::chrono::milliseconds(2000), [&]() {std::cout << "."; });
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(tim.is_periodic(), false);
  }
  /* Test Scenario no 2: Create two timers */
  {
    PeriodicTimer tim(std::chrono::milliseconds(2000), [&]() {std::cout << "."; });
    PeriodicTimer tim2(std::chrono::milliseconds(2000), [&]() {std::cout << "."; }, std::chrono::milliseconds(7000));
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(tim.is_periodic(), false);
    EXPECT_EQ(Status::OK, tim2.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim2.is_active());
    EXPECT_EQ(tim2.is_periodic(), false);
  }
}

/*
Test for constructor - negative case.
Testing procedure:
Create timer without callback function,
Constructor should set status to Status::ERROR
*/
TEST(PeriodicTimerTest, Constructor_Negative)
{
  /* Test Scenario no 1: Create timer without callback function */
  {
    PeriodicTimer tim(std::chrono::milliseconds(2000), nullptr);
    EXPECT_EQ(Status::ERROR, tim.status());
  }
}

/*
Test for Start/Stop methods - positive case.
Testing procedure:
Create timer,
Check status and returns of methods
*/
TEST(PeriodicTimerTest, StartStop_Positive)
{
  /* Test scenario no 1: Start timer */
  {
    PeriodicTimer tim(std::chrono::milliseconds(1000), [&]() {});
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(Status::OK, tim.Start());
    EXPECT_EQ(TimerStatus::ACTIVE, tim.is_active());
  }
  /* Test scenario no 2: Start, then stop the timer */
  {
    PeriodicTimer tim(std::chrono::milliseconds(1000), [&]() {});
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(Status::OK, tim.Start());
    EXPECT_EQ(TimerStatus::ACTIVE, tim.is_active());
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
  }
  /* Test scenario no 3: Create timer, and stop it before it call the function.
     We expect timer status to be TimerStatus::INACTIVE, and function to be called 0 times.*/
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(100), [&]() {cnt++; }, std::chrono::milliseconds(1000), true);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(Status::OK, tim.Start());
    EXPECT_EQ(TimerStatus::ACTIVE, tim.is_active());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(cnt, 0);
  }
}


/*
Test for Start/Stop methods - negative case.
Testing procedure:
Create timer,
Call Start and Stop method twice in a row - operation should result in Status::ERROR
*/
TEST(PeriodicTimerTest, StartStop_Negative)
{
  /* Test scenario no 1: Start timer */
  {
    PeriodicTimer tim(std::chrono::milliseconds(1000), [&]() {});
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(Status::OK, tim.Start());
    EXPECT_EQ(TimerStatus::ACTIVE, tim.is_active());
    EXPECT_EQ(Status::ERROR, tim.Start());
  }
  /* Test scenario no 2: Start, then stop the timer */
  {
    PeriodicTimer tim(std::chrono::milliseconds(1000), [&]() {});
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(Status::ERROR, tim.Stop());
  }
}

/*
Test for periodic timer
Testing procedure:
Create timer with/without delay,
Start timer,
Stop timer,
Check wheter call count is correct
*/
TEST(PeriodicTimerTest, TestDelay)
{
  /* Test scenario no 1: Create periodic timer, with 0 delay, call function every 0.1s and let it run for 2 seconds. We expect function to be called between 19-21 times. */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(100), [&]() {cnt++; }, std::chrono::milliseconds(0), true);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(Status::OK, tim.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_GE(cnt, 19);
    EXPECT_LE(cnt, 21);
  }
  /* Test scenario no 2: Create periodic timer, with 1s delay, call function every 0.1s and let it run for 2 seconds. We expect function to be called between 9-11 times. */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(100), [&]() {cnt++; }, std::chrono::milliseconds(1000), true);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(Status::OK, tim.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_GE(cnt, 9);
    EXPECT_LE(cnt, 11);
  }
  /* Test scenario no 3: Create periodic timer, with 5s delay, call function every 0.1s and let it run for 2 seconds. We expect function to be called 0 times. */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(100), [&]() {cnt++; }, std::chrono::milliseconds(5000), true);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(Status::OK, tim.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_EQ(cnt, 0);
  }
  /* Test scenario no 4: Create 2 periodic timers, first one with 0s delay, second one with 1s delay, and let it run for 2 seconds. We expect same results, as for single timers. */
  {
    int cnt1 = 0, cnt2 = 0;
    PeriodicTimer tim1(std::chrono::milliseconds(100), [&]() {cnt1++; }, std::chrono::milliseconds(0), true);
    EXPECT_EQ(Status::OK, tim1.status());
    PeriodicTimer tim2(std::chrono::milliseconds(100), [&]() {cnt2++; }, std::chrono::milliseconds(1000), true);
    EXPECT_EQ(Status::OK, tim2.status());
    EXPECT_EQ(Status::OK, tim1.Start());
    EXPECT_EQ(Status::OK, tim2.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim1.Stop());
    EXPECT_EQ(Status::OK, tim2.Stop());
    EXPECT_GE(cnt1, 19);
    EXPECT_LE(cnt1, 21);
    EXPECT_GE(cnt2, 9);
    EXPECT_LE(cnt2, 11);
  }

}

/*
Test for periodic timer
Testing procedure:
Create timer without delay,
Set different periods for each test case,
Start timer,
Stop timer,
Check wheter call count is correct
*/
TEST(PeriodicTimerTest, TestPeriod)
{
  /* Test scenario no 1: Create periodic timer, with 0 delay, call function every 0.1s and let it run for 2 seconds. We expect function to be called between 19-21 times. */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(100), [&]() {cnt++; }, std::chrono::milliseconds(0), true);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(Status::OK, tim.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_GE(cnt, 19);
    EXPECT_LE(cnt, 21);
  }
  /* Test scenario no 2: Create periodic timer, with 0 delay, call function every 1s and let it run for 2 seconds. We expect function to be called between 1-3 times. */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(1000), [&]() {cnt++; }, std::chrono::milliseconds(0), true);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(Status::OK, tim.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim.Stop());
    EXPECT_GE(cnt, 1);
    EXPECT_LE(cnt, 3);
  }
  /* Test scenario no 3: Create 2 periodic timers, with 0 delay, call function every 0.1s for the first one, and 1s for the second one
  and let it run for 2 seconds. We expect same results, as for single timers.  */
  {
    int cnt1 = 0, cnt2 = 0;
    PeriodicTimer tim1(std::chrono::milliseconds(100), [&]() {cnt1++; }, std::chrono::milliseconds(0), true);
    EXPECT_EQ(Status::OK, tim1.status());
    PeriodicTimer tim2(std::chrono::milliseconds(1000), [&]() {cnt2++; }, std::chrono::milliseconds(0), true);
    EXPECT_EQ(Status::OK, tim2.status());
    EXPECT_EQ(Status::OK, tim1.Start());
    EXPECT_EQ(Status::OK, tim2.Start());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    EXPECT_EQ(Status::OK, tim1.Stop());
    EXPECT_EQ(Status::OK, tim2.Stop());
    EXPECT_GE(cnt1, 19);
    EXPECT_LE(cnt1, 21);
    EXPECT_GE(cnt2, 1);
    EXPECT_LE(cnt2, 2);
  }
}
/*
Test for one shot timer
Testing procedure:
Create timer,
Start timer,
Check wheter call count is correct and timer is stopped
*/
TEST(PeriodicTimerTest, OneShot_Positive)
{
  /* Test case no 1: Call one shot timer without the delay. We expect function to be called exactly once, and timer status equal to TimerStatus::INACTIVE at the end of the test */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(0), [&]() {cnt++; }, std::chrono::milliseconds(0), false);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(false, tim.is_periodic());
    EXPECT_EQ(Status::OK, tim.Start());
    EXPECT_EQ(TimerStatus::ACTIVE, tim.is_active());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(cnt, 1);
  }
  /* Test case no 2: Call one shot timer with 1s delay. We expect function to be called exactly once, and timer status equal to TimerStatus::INACTIVE at the end of the test.
     We expect also function to be called 0 times before 1s mark, and exactly once after 1s mark. */
  {
    int cnt = 0;
    PeriodicTimer tim(std::chrono::milliseconds(0), [&]() {cnt++; }, std::chrono::milliseconds(1000), false);
    EXPECT_EQ(Status::OK, tim.status());
    EXPECT_EQ(false, tim.is_periodic());
    EXPECT_EQ(Status::OK, tim.Start());
    EXPECT_EQ(TimerStatus::ACTIVE, tim.is_active());
    EXPECT_EQ(cnt, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(TimerStatus::INACTIVE, tim.is_active());
    EXPECT_EQ(cnt, 1);
  }
}
#include <chrono>
#include <conio.h>
#include <stdio.h>
#include "periodic_timer.h"

char f1_1[] = "            _  _               ";
char f1_2[] = "           ( `   )_            ";
char f1_3[] = "          (    )    `)         ";
char f1_4[] = "        (_   (_ .  _) _)       ";
char f2_1[] = "          _  _                 ";
char f2_2[] = "         ( `   )_              ";
char f2_3[] = "        (    )    `)           ";
char f2_4[] = "      (_   (_ .  _) _)         ";


int cnt = 0;
int cnt1 = 0;
void print_time()
{
  int s = cnt % 60, m = (cnt / 60) % 60, h = cnt / 3600;
  system("cls");
  printf("%d:%d:%d\n", h, m, s);  
  if (cnt1 % 2 == 0)
  {
    printf("%s%s\n%s%s\n%s%s\n%s%s\n", f1_1, f1_1, f1_2, f1_2, f1_3, f1_3, f1_4, f1_4);
  }
  else
  {
    printf("%s%s\n%s%s\n%s%s\n%s%s\n", f2_1, f2_1, f2_2, f2_2, f2_3, f2_3, f2_4, f2_4);
  }
}



int main()
{
  /* This timer starts counter to imitate clock */
  PeriodicTimer tim_clock(std::chrono::milliseconds(1000), [&]() {cnt++; }, std::chrono::milliseconds(0),true);
  /* This timer starts counter responsible for 'moving clouds' */
  PeriodicTimer tim_cloud(std::chrono::milliseconds(500), [&]() {cnt1++; }, std::chrono::milliseconds(0), true);
  /* This timer is responsible for refreshing the console window */
  PeriodicTimer tim_print(std::chrono::milliseconds(500), print_time, std::chrono::milliseconds(0), true);
  /* This timer delay start of 'timer_cloud'. We can either start it right away with given delay, or start it later.*/
  PeriodicTimer tim_anim(std::chrono::milliseconds(0), [&]() {tim_cloud.Start(); }, std::chrono::milliseconds(2000));
  /* This timer stops every other timers */
  PeriodicTimer tim_stop(std::chrono::milliseconds(0), [&]() {tim_cloud.Stop(); tim_clock.Stop(); tim_print.Stop(); tim_anim.Stop(); system("cls"); printf(":)"); }, std::chrono::milliseconds(6000));
  tim_stop.Start();
  tim_clock.Start();
  tim_print.Start();
  tim_anim.Start();
  
  _getch();
}
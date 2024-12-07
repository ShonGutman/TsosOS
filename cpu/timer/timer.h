#ifndef TIMER_H
#define TIMER_H

#include "headers/types.h"


/*
 * Initializes the system timer by setting up the Programmable Interval Timer (PIT)
 * to generate interrupts at a specified frequency and installing the timer handler for IRQ 0.
 * This allows the system to perform periodic tasks like multitasking or measuring time.
 */
void init_timer();

/*
 * Pauses the execution of the program for the specified number of milliseconds.
 * The function uses the system tick counter to wait until the desired time has passed.
 *
 * @param ms The number of milliseconds to sleep.
 */
void sleep(const uint32 ms);

/*
 * Retrieves the current system clock tick count.
 * This function returns the number of ticks since the system started.
 *
 * @return The current system tick count as a 32-bit unsigned integer.
 */
uint32 get_system_clock();

#endif
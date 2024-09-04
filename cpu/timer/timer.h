#ifndef TIMER_H
#define TIMER_H


/*
 * Initializes the system timer by setting up the Programmable Interval Timer (PIT)
 * to generate interrupts at a specified frequency and installing the timer handler for IRQ 0.
 * This allows the system to perform periodic tasks like multitasking or measuring time.
 */
void init_timer();

#endif
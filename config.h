#ifndef __CONFIG_H
#define __CONFIG_H

/* Which timer module to use for the schedule module
 * 0 = TIMER_A
 * 1 = TIMER_B */
#define SCHED_TIMER_B 0

/* Amount to divide SMCLK by to give a 1kHz timer */
#define SCHED_DIV 16000

/* Size of the queue for schedule tasks */
#define SCHED_QUEUE_SIZE 2

#define SRIC_DIRECTOR 0

/* Pin interrupt config */
#define PININT_NCONF 1

#endif /* __CONFIG_H */

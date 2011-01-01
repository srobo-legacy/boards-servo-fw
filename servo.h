#ifndef __SERVO_H
#define __SERVO_H
#include <stdint.h>

#define SERVO_MAX 100
#define SERVO_MIN 0
/* Returned by 'servo_get' when 'servo' is out of range */
#define SERVO_ERR 0xff

#define SERVO_MID ((SERVO_MAX+SERVO_MIN)/2)

void servo_init(void);

/* Pass a value 'pos' between SERVO_MIN and SERVO_MAX to set
 * the angle of the servo 'servo' */
void servo_set(uint8_t servo, uint8_t pos);

/* Get the angle that the servo 'servo' is currently set to as
 * a value between SERVO_MIN and SERVO_MAX */
uint8_t servo_get(uint8_t servo);

#endif /* __SERVO_H */

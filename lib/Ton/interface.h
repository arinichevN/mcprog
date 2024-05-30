#ifndef TON_H
#define TON_H

//					interface dependencies
#include "../stime_t/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct {
	stime_t interval;
	stime_t end;
} Ton;

extern int ton(Ton *self);
extern int tonr(Ton *self);
extern void ton_setInterval(Ton *self, stime_t interval);
extern stime_t ton_getInterval(Ton *self);
extern void ton_reset(Ton *self);
extern stime_t ton_timePassed(Ton *self);
extern stime_t ton_timeRest(Ton *self);

#endif

//					implementation dependencies
#include "interface.h"

int ton(Ton *self) {
	stime_t now = getCurrentTime();
	if(stime_compareM(&now, &self->end) == YES) {
		return 1;
	}
	return 0;
}

int tonr(Ton *self) {
	stime_t now = getCurrentTime();
	if(stime_compareM(&now, &self->end) == YES) {
		self->end = stime_add(&now, &self->interval);
		return 1;
	}
	return 0;
}

void ton_setInterval(Ton *self, stime_t interval) {
	self->interval = interval;
}

stime_t ton_getInterval(Ton *self){
	return self->interval;
}

void ton_reset(Ton *self) {
	stime_t now = getCurrentTime();
	self->end = stime_add(&now, &self->interval);
}

stime_t ton_timePassed(Ton *self) {
	stime_t now = getCurrentTime();
	stime_t start = stime_subtract(&self->end, &self->interval);
	stime_t out = stime_subtract(&now, &start);
	return out;
}

stime_t ton_timeRest(Ton *self) {
	stime_t now = getCurrentTime();
	stime_t out;
	if(stime_compareM(&self->end, &now) == YES) {
		out = stime_subtract(&self->end, &now);
	} else {
		stime_reset(&out);
	}
	return out;
}


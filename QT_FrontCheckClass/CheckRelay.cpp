#pragma once
#include "CheckRelay.h"
#ifdef WIN32
int gettimeofday(struct timeval* tp, void* tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif
bool sortResult(ListStruct a, ListStruct b)
{
	int x = a.iminx;
	int y = b.iminx;
	return x < y;
}
bool sortScore(ListStruct a, ListStruct b)
{
	double x = a.score;
	double y = b.score;
	return x > y;
}
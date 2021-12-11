#include"time_debug.h"

void start()
{
	a = clock();
}
void end()
{
	b = clock();
	etime += b - a;
}
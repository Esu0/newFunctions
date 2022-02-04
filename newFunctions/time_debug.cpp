#include"time_debug.h"

void start()
{
	start_time = clock();
}
void end()
{
	end_time = clock();
	etime += end_time - start_time;
}
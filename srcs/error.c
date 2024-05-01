#include "philosophers.h"

int ones_case(t_all *all, int ones_Y)
{
	if (all->qty == 1)
    {
    	pthread_mutex_unlock(&all->Y[ones_Y]);
        (usleep(all->die_time * 1000));
        printf("%llu %d died\n", get_time() - all->philo[1].start_time, 1);
        return 1;
    }
    return (0);
}
int are_alive(t_all *all)
{
	pthread_mutex_lock(&all->alive_mtx);
	if (all->alive <= 0)
    {
	    pthread_mutex_unlock(&all->alive_mtx);
        return 0;
    }
    else
    {
	    pthread_mutex_unlock(&all->alive_mtx);
        return 1;
    }
}

int error(char *message)
{
    printf("%s\n", message);
    exit(1);
} 
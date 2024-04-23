#include "philosophers.h"

 
int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

long	ato(char *arg)
{
	long	num;
	long	n;

	n = 0;
	num = 0;
	while (*arg)
	{
		if ((*arg > 47) && (*arg < 58))
			n = *arg - 48;
		else
			return ( -1);
		num = num * 10 + n;
		arg++;
	}
	return (num);
}


long long get_time()
{
	struct timeval time;
	
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}


void show_act(t_all *all, int curr, char *str)
{
	long long 		t;

	t = get_time() - all->philo[curr].start_time;
	// pthread_mutex_lock(&all->print_mtx);
	printf("%llu %d %s\n", t, curr, str);
	// pthread_mutex_unlock(&all->print_mtx);
}

int udream(t_all *all, long long time)
{
	long long start;
	int i;

	i = 0;
	start = get_time();
	while (get_time() - start < time) 
	{
		pthread_mutex_lock(&all->stop_mtx);
		if (all->stop)
		{
			pthread_mutex_unlock(&all->stop_mtx);
			return 1;
		}
		pthread_mutex_unlock(&all->stop_mtx);
		usleep(25);
	}
	return 0;
} 
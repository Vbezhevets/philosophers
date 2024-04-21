#include "philosophers.h"
#include <stdio.h>

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


void show_act(t_philo *philo, char *str)
{
	struct timeval 	time;
	long long 		t;

	// gettimeofday(&time, NULL);
	t = get_time() - philo->start_time;
	printf("%llu %d %s\n", t, philo->id, str);

}
int udream(t_all *all, unsigned long long time)
{
	// time /= 1000;
	long long start;
	int i;

	i = 0;
	start = get_time();
	while (get_time() - start < time) 
	{
		// i++;
		// printf("i run %d time\n", i);
		// printf("%llu - %llu < %llu\n", get_time(), start,  time);
		// printf("SToP %d\n", all->stop);
		// printf("MEALS LEFT %d\n",	all->philo[i].meals_left);

		if (all->stop)
			return 1;
		usleep(15);
	}
	return 0;
} 
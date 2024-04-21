#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


void Y_lock(t_all *all, int curr)
{
	int prev;
	int	first;
	int second;

	prev = (curr - 2 + all->qty) % all->qty + 1;
	first = (prev + ((prev + 1) % 2)) % (all->qty + 1);
	second = (prev + ((prev) % 2)) % (all->qty);
	pthread_mutex_lock(&all->Y[first]); 
	show_act( &all->philo[curr], "has taken a fork");
	pthread_mutex_lock(&all->Y[second]);
	show_act( &all->philo[curr], "has taken a fork"); 
}

void Y_unlock(t_all *all, int curr)
{
	int prev;
	int	first;
	int second;

	prev = (curr - 2 + all->qty) % all->qty + 1;
	second = (prev + ((prev + 1) % 2)) % (all->qty + 1);
	first = (prev + ((prev) % 2)) % (all->qty);
	pthread_mutex_unlock(&all->Y[first]); 
	pthread_mutex_unlock(&all->Y[prev + ((prev + 1) % 2)]); 
}

void personal_loop (t_all *all, int curr, int prev)
{
	while (1)
	{
		show_act(&all->philo[curr], "is thinking");
		Y_lock(all, curr);
		show_act(&all->philo[curr], "is eating");
		pthread_mutex_lock(&all->philo[curr].last_meal_mtx);
		all->philo[curr].last_meal = get_time();
		pthread_mutex_unlock(&all->philo[curr].last_meal_mtx);
		if (udream(all, all->eat_time))
			break;
		Y_unlock(all, curr);
		show_act(&all->philo[curr], "is sleeping");
		all->philo[curr].last_sleep = get_time() + all->sleep_time;
		if (udream(all, all->sleep_time))
			break;

	}
}	

void common_loop(t_all	*all, int i)
{	
	while (i <= all->qty)
	{	
 		if ((get_time() - all->philo[i].last_meal)  > all->die_time)
		{	
			show_act(&all->philo[i], "died");
			pthread_mutex_lock(&all->stop_mtx);
			all->stop = 1;
			pthread_mutex_unlock(&all->stop_mtx);
			// return;
		}
		// if (all->meals_left >= 0)
		// {
		// 	pthread_mutex_lock(&all->stop_mtx);
		// 	all->stop = 1;
		// 	pthread_mutex_unlock(&all->stop_mtx);
		// }
		i++;
		if (i > all->qty)
			i = 1;
		if (udream(all, 20))
			break;	
	}
}

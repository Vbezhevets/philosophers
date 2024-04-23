#include "philosophers.h"

 
void YY_lock(t_all *all, int curr)
{
	int prev;
	int	first_Y;
	int second_Y;

	prev = (curr - 2 + all->qty) % all->qty + 1;
	first_Y = (prev + ((prev + 1) % 2)) % (all->qty + 1);
	second_Y = (prev + (prev % 2)) % all->qty;


	// if (udream(all, 10))
	// 	return;

	pthread_mutex_lock(&all->Y[first_Y]);
	// if (!udream(all, 10))
		show_act(all, curr, "has taken a fork");
	pthread_mutex_lock(&all->Y[second_Y]);
	// if (!udream(all, 10))
		show_act(all, curr, "has taken a fork");
	pthread_mutex_lock(&all->philo[curr].last_meal_mtx);
	all->philo[curr].last_meal = get_time();
	pthread_mutex_unlock(&all->philo[curr].last_meal_mtx);
	// if (!udream(all, 10))
		show_act(all, curr, "is eating");

}

void YY_unlock(t_all *all, int curr)
{
	int prev;
	int	first_Y;
	int second_Y;

	prev = (curr - 2 + all->qty) % all->qty + 1;
	second_Y = (prev + ((prev + 1) % 2)) % (all->qty + 1);
	first_Y = (prev + (prev % 2)) % all->qty;
	pthread_mutex_unlock(&all->Y[first_Y]);
	pthread_mutex_unlock(&all->Y[second_Y]); 
}

void personal_loop (t_all *all, int curr)
{
	while (1)
	{
		show_act(all, curr, "is thinking");
		if (udream(all, 0))
			return;
		YY_lock(all, curr);
		if (udream(all, all->eat_time))
		{
			YY_unlock(all, curr);
			return;
		}
		YY_unlock(all, curr);
		all->philo[curr].meals_left--;
		if (all->philo[curr].meals_left == 0)
		{
			pthread_mutex_lock(&all->alives_mtx);
			all->alives--;
			pthread_mutex_unlock(&all->alives_mtx);
			return;
		}
		show_act(all, curr, "is sleeping");
		all->philo[curr].last_sleep = get_time() + all->sleep_time;
		if (udream(all, all->sleep_time))
			return;
	}
}	

void common_loop(t_all	*all, int i)
{	
	while (i <= all->qty)
	{	
		pthread_mutex_lock(&all->philo[i].last_meal_mtx);
 		if ((get_time() - all->philo[i].last_meal) > all->die_time 
			&& all->philo[i].meals_left != 0)
		{	
			pthread_mutex_lock(&all->stop_mtx);
			all->stop = 1;
			show_act(all, i, "died");
			pthread_mutex_unlock(&all->stop_mtx);
			pthread_mutex_unlock(&all->philo[i].last_meal_mtx);
			break;
		}
		pthread_mutex_unlock(&all->philo[i].last_meal_mtx);
		pthread_mutex_lock(&all->alives_mtx);
		if (all->alives <= 0)
		{
			pthread_mutex_unlock(&all->alives_mtx);
		 	return;
		}
		pthread_mutex_unlock(&all->alives_mtx);
		i++;
		if (i > all->qty)
			i = 1;
	}
}

void *create_philo_thread(void * point_all)
{
	t_all *all;
	int curr;

	all = (t_all *)point_all;
	pthread_mutex_lock(&all->cur_mtx);
	curr = all->curr;
	pthread_mutex_unlock(&all->cur_mtx);
	personal_loop(all, curr);
	return NULL;
}
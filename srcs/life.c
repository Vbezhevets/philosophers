	#include "philosophers.h"

 
void YY_lock(t_all *all, int curr)
{
	int prev;
	int	first_Y;
	int second_Y;

	prev = (curr - 2 + all->qty) % all->qty + 1;

	first_Y = (prev + ((prev + 1) % 2)) % all->qty + 1;
	second_Y = (prev + (prev % 2)) % all->qty + 1;


	if (udream(all, 0))
		return;

	pthread_mutex_lock(&all->Y[first_Y]);
	if (!udream(all, 0))
		show_act(all, curr, "has taken a fork");
	pthread_mutex_lock(&all->Y[second_Y]);
	if (!udream(all, 0))
		show_act(all, curr, "has taken a fork");
	pthread_mutex_lock(&all->philo[curr].last_meal_mtx);
	all->philo[curr].last_meal = get_time();
	pthread_mutex_unlock(&all->philo[curr].last_meal_mtx);
	if (!udream(all, 0))
		show_act(all, curr, "is eating");

}

void YY_unlock(t_all *all, int curr)
{
	int prev;
	int	first_Y;
	int second_Y;

	prev = (curr - 2 + all->qty) % all->qty + 1;
	second_Y = (prev + ((prev + 1) % 2)) % all->qty + 1;
	first_Y = (prev + (prev % 2)) % all->qty + 1;
	pthread_mutex_unlock(&all->Y[first_Y]);
	pthread_mutex_unlock(&all->Y[second_Y]);  
}

void *personal_loop(void *philo_point)
{
	t_philo *philo;
	t_all	*all;
	int		curr;

	philo = (t_philo *)philo_point;
	curr = philo->id;
	all = philo->mall;
	while (1)
	{
		usleep(27);
		if (udream(all, 0))
			return (NULL);
		show_act(all, curr, "is thinking");
		YY_lock(all, curr);
		if (udream(all, all->eat_time))
		{
			YY_unlock(all, curr);
			return (NULL);
		}
		YY_unlock(all, curr);
		pthread_mutex_lock(&all->philo[curr].last_meal_mtx);
		all->philo[curr].meals_left--;
		if (all->philo[curr].meals_left == 0)
		{
			pthread_mutex_unlock(&all->philo[curr].last_meal_mtx);
			pthread_mutex_lock(&all->alives_mtx);
			all->alives--;
			pthread_mutex_unlock(&all->alives_mtx);
			return (NULL);
		}
		pthread_mutex_unlock(&all->philo[curr].last_meal_mtx);

		if (udream(all, 0))
			return (NULL);
		show_act(all, curr, "is sleeping");
		all->philo[curr].last_sleep = get_time() + all->sleep_time;
		if (udream(all, all->sleep_time))
			return (NULL);
	}
	return (NULL);
}	

void common_loop(t_all	*all, int i)
{	
	long long t;

	while (i <= all->qty)
	{	
		pthread_mutex_lock(&all->philo[i].last_meal_mtx);
 		if ((get_time() - all->philo[i].last_meal) > all->die_time 
			&& all->philo[i].meals_left != 0)
		{	
			pthread_mutex_unlock(&all->philo[i].last_meal_mtx);
			pthread_mutex_lock(&all->stop_mtx);
			all->stop = 1;
			pthread_mutex_unlock(&all->stop_mtx);
			t = get_time() - all->philo[i].start_time;
			pthread_mutex_lock(&all->print_mtx);
			printf("%llu %d died\n", t, i);
			pthread_mutex_unlock(&all->print_mtx);
			// usleep(100000);
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

// void *create_philo_thread(void * point_philo)
// {
// 	t_philo *philo;
// 	int curr;

// 	philo = (t_philo *)point_philo;
// 	curr = philo->id;
// 	personal_loop(philo->mall, curr);
// 	return NULL;
// }
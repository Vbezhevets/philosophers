#include "philosophers.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

void check_and_assign(t_all *all, int argc, char **argv, int i)
{
	while (i < argc)
	{
		if (ato(argv[i]) <= 0)
		 	error("wrong arguments");
		i++;
	}
	all->qty = ato(argv[1]);
	if (all->qty > MAX_Q)
		error("too much ph-s. Good bye");
	all->die_time = ato(argv[2]);
	all->eat_time = ato(argv[3]);
	all->sleep_time = ato(argv[4]);
	i = 1;
	while (i <= all->qty)
	{
		all->philo[i].die_time = ato(argv[2]);
		all->philo[i].is_waiting = 1;
		if (argc == 6)
			all->philo[i].meals_left = ato(argv[5]); 
		else
			all->philo[i].meals_left = -1;
		i++;
	}
}


void mtx_init(t_all *all, int i)
{
	while (i <= all->qty)
	{
		if (pthread_mutex_init(&all->Y[i], NULL) != 0)
			error("error mutex creating");
		if (pthread_mutex_init(&all->philo[i].last_meal_mtx, NULL) != 0)
			error("error mutex creating");
		i++;
	}
	if (pthread_mutex_init(&all->cur_mtx, NULL) != 0)
		error("error mutex creating");
	if (pthread_mutex_init(&all->stop_mtx, NULL) != 0)
		error("error mutex creating");
}


void *create_philo_thread(void * point_all)
{
	t_all *all;
	int curr;

	all = (t_all *)point_all;
	pthread_mutex_lock(&all->cur_mtx);
	curr = all->curr;
	pthread_mutex_unlock(&all->cur_mtx);
	personal_loop(all, curr, all->philo[curr].prev);
	return NULL;
}

void start(t_all *all, int i)
{	
	all->start_time = get_time();

	all->stop = 0;
 	while (i <= all->qty)
	{	
		all->philo[i].id = i;
		all->philo[i].start_time = all->start_time;
		all->philo[i].last_meal = all->start_time;
		all->philo[i].mall = all;
		all->philo[i].last_sleep = all->start_time + all->sleep_time;
		pthread_mutex_lock(&all->cur_mtx);
		all->curr = i;
		pthread_mutex_unlock(&all->cur_mtx);
		if (pthread_create(&all->philo[i].lives, NULL, &create_philo_thread, (void *) all) != 0)
	 		error("error creating thread");
		i += 2;
        if (i > all->qty && i % 2 != 0)
			i = 2;
		usleep(30);
	}
	common_loop(all, 1);
}


void end(t_all *all, int i)
{
	while (i <= all->qty)
	{
		pthread_join(all->philo[i].lives, NULL);
		pthread_mutex_destroy(&all->Y[i]);
		pthread_mutex_destroy(&all->philo[i].last_meal_mtx);
		i++;
	}
	pthread_mutex_destroy(&all->cur_mtx);
}

int main (int argc, char *argv[])
{
	t_all  all;


	if (argc != 6 && argc != 5)
		return(printf("wrong aruments\n"), 1);
	check_and_assign(&all, argc, argv, 1);

	mtx_init(&all, 1);

	start(&all, 1);

	end(&all, 1);

}

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define MAX_Q 1024
#define MAX_INT 2147483647

typedef struct s_all t_all;

typedef  struct 	s_philo
{
	int				prev;
	int				id;
	int				is_waiting;
	int				meals_left;

	unsigned long long	last_meal;
	unsigned long long	last_sleep;
	unsigned long long	die_time;
	unsigned long long	start_time;

	t_all 			*mall;
	pthread_t		lives;
	pthread_t		thinkin;
	pthread_mutex_t	last_meal_mtx;

}					t_philo;

typedef  struct 	s_all
{	
	int				qty;
	int				debug;

	pthread_mutex_t	stop_mtx;
	int 			stop;

	pthread_mutex_t	cur_mtx;
	int				curr;

	pthread_mutex_t	alives_mtx;
	int				alives;

	pthread_mutex_t	print_mtx;


	pthread_mutex_t	Y[MAX_Q];

	t_philo			philo[MAX_Q];

	unsigned long long start_time;
	unsigned long long	die_time;
	unsigned long long	sleep_time;
	unsigned long long	eat_time;
}					t_all;

int		udream(t_all *all, long long time);
long	ato(char *arg);
void 	life_loop(t_all *all, int i);
void 	*personal_loop(void *ph_thread);
void 	common_loop(t_all	*all, int i);
void	loop(t_all *all, int i);
int		error(char *message);
int		prv(int i, t_all *all);
long long 	get_time();
void 	show_act(t_all *all, int curr, char *str);
void 	end(t_all *all, int i);
int		lr_Y_free(t_all *all, int curr);

#endif
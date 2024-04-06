#include <pthread.h>
#include "caltrain.h"


void
station_init(struct station *station)
{
	station->emptySeats=0;
	station->waitingPassengers=0;
	station->passengersOnTrain=0;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t waitTrains = PTHREAD_COND_INITIALIZER;
	pthread_cond_t waitPassengers = PTHREAD_COND_INITIALIZER;
	pthread_cond_t enterTrain = PTHREAD_COND_INITIALIZER;
	station->mutex=lock;
	station->waitTrainWithEmptySeats= waitTrains;
	station->waitPassengers= waitPassengers;
	station->getOnBoard= enterTrain;
}

void
station_load_train(struct station *station, int count)
{
	station->emptySeats=count;
	if (count>0)
		pthread_cond_signal(&(station->waitTrainWithEmptySeats));
	if (station->waitingPassengers!= 0)
		pthread_cond_wait(&(station->waitPassengers),&(station->mutex));

}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->mutex));
	station->waitingPassengers++;
	pthread_cond_wait(&(station->waitTrainWithEmptySeats),&(station->mutex));
	pthread_mutex_lock(&(station->mutex));
	station->emptySeats--;
	station->waitingPassengers--;
	station->passengersOnTrain++;
	// pthreads_mutex_unlock(&(station->mutex));
	pthread_cond_wait(&(station->getOnBoard),&(station->mutex));
}

void
station_on_board(struct station *station)
{
	station->passengersOnTrain=0;
	pthread_cond_broadcast(&(station->getOnBoard));
	if (station->emptySeats==0||station->waitingPassengers==0)
		pthread_cond_signal(&(station->waitPassengers));
}

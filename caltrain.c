#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"


void
station_init(struct station *station)
{
	station->emptySeats=0;
	station->waitingPassengers=0;
	station->passengersOnTrain=0;
	pthread_mutex_init(&(station->mutex),NULL);
	pthread_cond_init(&(station->waitTrainWithEmptySeats),NULL);
	pthread_cond_init(&(station->waitPassengers),NULL);
    pthread_cond_init(&(station->onBoard),NULL);
}

void
station_load_train(struct station *station, int count)
{
	station->emptySeats=count;
//    printf("Train arrived with %d seats, Current Waiting is %d\n",station->emptySeats,station->waitingPassengers);
    if (count>0){
        pthread_cond_broadcast(&(station->waitTrainWithEmptySeats));
        if (station->waitingPassengers!= 0) {
            pthread_cond_wait(&(station->waitPassengers),&(station->mutex));
        }
    }
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&(station->mutex));
    station->waitingPassengers++;
    pthread_cond_wait(&(station->waitTrainWithEmptySeats),&(station->mutex));
//    printf("Empty Seats %d ,,, Waiting Pass %d ,,, Walking on Train %d\n",station->emptySeats,station->waitingPassengers,
//           station->passengersOnTrain);
    while(station->emptySeats==0) {
        pthread_cond_wait(&(station->waitTrainWithEmptySeats),&(station->mutex));
    }
    station->emptySeats--;
    station->waitingPassengers--;
    station->passengersOnTrain++;
    pthread_mutex_unlock(&(station->mutex));
}

void
station_on_board(struct station *station)
{
//    printf("onEmpty Seats %d ,,, Waiting Pass %d ,,, Walking on Train %d\n",station->emptySeats,station->waitingPassengers,
//           station->passengersOnTrain);
    pthread_mutex_lock(&(station->mutex));
    station->passengersOnTrain--;
    if ( (station->passengersOnTrain==0) && (station->emptySeats==0 || station->waitingPassengers==0)) {
        pthread_cond_signal(&(station->waitPassengers));
    }
    pthread_mutex_unlock(&(station->mutex));

}

#include <pthread.h>

struct station {
	int emptySeats;
	int waitingPassengers;
	int passengersOnTrain;
	pthread_mutex_t mutex;
	pthread_cond_t waitTrainWithEmptySeats;
	pthread_cond_t waitPassengers;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);

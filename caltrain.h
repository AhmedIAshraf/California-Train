#include <pthread.h>

struct station {
	int emptySeats;
	int waitingPassengers;
	int passengersOnTrain;
	pthread_mutex_t mutex;// = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t waitTrainWithEmptySeats;// = PTHREAD_COND_INITIALIZER;
	pthread_cond_t waitPassengers;// = PTHREAD_COND_INITIALIZER;
	pthread_cond_t getOnBoard;// = PTHREAD_COND_INITIALIZER;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
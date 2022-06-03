
// Change the cars and run_time variable to the desired values.
//Directions: north: 1, south: 2, west: 3, east: 4

#include <iostream>
#include <pthread.h>
#include <queue>
#include <cstdlib> //for rand()
#include <unistd.h> // for usleep()
#include <ctime> //for clock(), clock_t, CLOCKS_PER_SEC

const int cars = 100; // # cars all together
const int run_time = 2; // seconds the program will wait for to allow all cars to pass

using namespace std;

int car_id; // global ID for each car arriving at the intersection
int direction;


// a queue for each direction.
queue<int> north_lane;
queue<int> east_lane;
queue<int> south_lane;
queue<int> west_lane;
queue<int> intersection_lane;

pthread_t threadID;

// a lock for each queue/direction.
pthread_mutex_t northLock;
pthread_mutex_t eastLock;
pthread_mutex_t southLock;
pthread_mutex_t westLock;
pthread_mutex_t intersectionQLock;

pthread_mutex_t global_id_Lock; // lock for changing the globalid (i.e. car id)
pthread_mutex_t intersectionLock; // lock for one car passing through the intersection

int getrand(int min,int max) //return a number x s.t. min <= x < max
{
	     return(rand()%(max-min)+min);
}

void init()
{
    car_id = 1; //first car will have ID = 1
    pthread_mutex_init(&northLock, NULL);
    pthread_mutex_init(&eastLock, NULL);
    pthread_mutex_init(&southLock, NULL);
    pthread_mutex_init(&westLock, NULL);
    pthread_mutex_init(&intersectionQLock, NULL);
    pthread_mutex_init(&global_id_Lock, NULL);
    pthread_mutex_init(&intersectionLock, NULL);
}

// This is a function for a car moving north.
void *north(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&northLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in north lane
    pthread_mutex_unlock(&global_id_Lock);
    north_lane.push(localID); // pushing the local car into northQ.
    pthread_mutex_unlock(&northLock);

    sleep(5); //say it takes 5 seconds to get to the intersection
    pthread_mutex_lock(&intersectionQLock);
    intersection_lane.push(localID);
    pthread_mutex_unlock(&intersectionQLock);
    start = clock(); // Now the car has arrived at intersection. Let's start the timer.

    while(1) //Checking cars properties here
    {
        if ((localID == north_lane.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
        {
            break;
        }
        else //Current car is not in front on its lane. Lets wait
        {
            usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
            continue;
        }
    }
    // Car is in the front so let's proceed to allow it to pass through intersection.
    pthread_mutex_lock(&intersectionLock); // need to lock the intersection. Function call will block until mutex is available
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    north_lane.pop();
    intersection_lane.pop();
    cout << "Car from NORTH lane with ID: " << localID << " ENTERING the intersection." << endl;
    cout << "It has been waiting at the light for: "<< duration<<" seconds."<<endl;
    if (direction == 1)
    {
        cout << "Car from NORTH lane is heading towards NORTH direction." << endl;
    }
    else if (direction == 2)
    {
        cout << "Car from NORTH lane is heading towards SOUTH direction." << endl;
    }
    else if (direction == 3)
    {
        cout << "Car from NORTH lane is heading towards WEST direction." << endl;
    }
    else
    {
        cout << "Car from NORTH lane is heading towards EAST direction." << endl;
    }
    sleep(3);
    cout << "Car from NORTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
    pthread_mutex_unlock(&intersectionLock);
}

void *east(void *null)
{

    int localID;
    double duration;
    clock_t start;

    pthread_mutex_lock(&eastLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in north lane
    pthread_mutex_unlock(&global_id_Lock);
    east_lane.push(localID); // pushing the local car into northQ.
    pthread_mutex_unlock(&eastLock);

    sleep(5); //say it takes 5 seconds to get to the intersection
    pthread_mutex_lock(&intersectionQLock);
    intersection_lane.push(localID);
    pthread_mutex_unlock(&intersectionQLock);
    start = clock();

    while(1) //Checking cars properties here
    {
        if ((localID == east_lane.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
        {
            break;
        }
        else //Current car is not in front on its lane. Lets wait
        {
            usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
            continue;
        }
    }
    // Car is in the front so let's proceed to allow it to pass through intersection.
    pthread_mutex_lock(&intersectionLock); // need to lock the intersection. Function call will block until mutex is available
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    east_lane.pop();
    intersection_lane.pop();
    cout << "Car from EAST lane with ID:  " << localID << " ENTERING the intersection." << endl;
    cout << "It has been waiting at the light for: "<<duration<<" seconds."<<endl;
    if (direction == 1)
    {
        cout << "Car from EAST lane is heading towards NORTH direction." << endl;
    }
    else if (direction == 2)
    {
        cout << "Car from EAST lane is heading towards SOUTH direction." << endl;
    }
    else if (direction == 3)
    {
        cout << "Car from EAST lane is heading towards WEST direction." << endl;
    }
    else
    {
        cout << "Car from EAST lane is heading towards EAST direction." << endl;
    }
    sleep(3);
    cout << "Car from EAST lane with ID:  " << localID << " LEAVING the intersection." << endl <<endl;
    pthread_mutex_unlock(&intersectionLock);
}

void *south(void *null)
{

    int localID;
    double duration;
    clock_t start;

    pthread_mutex_lock (&southLock); // locking the queue
    pthread_mutex_lock (&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in north lane
    pthread_mutex_unlock (&global_id_Lock);
    south_lane.push(localID); // pushing the local car into northQ.
    pthread_mutex_unlock (&southLock);

    sleep(5); //say it takes 5 seconds to get to the intersection
    pthread_mutex_lock(&intersectionQLock);
    intersection_lane.push(localID);
    pthread_mutex_unlock(&intersectionQLock);
    start = clock();

    while(1) //Checking cars properties here
    {
        if ((localID == south_lane.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
        {
            break;
        }
        else //Current car is not in front on its lane. Lets wait
        {
            usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
            continue;
        }
    }
    // Car is in the front so let's proceed to allow it to pass through intersection.
    pthread_mutex_lock(&intersectionLock); // need to lock the intersection. Function call will block until mutex is available
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    south_lane.pop();
    intersection_lane.pop();
    cout << "Car from SOUTH lane with ID: " << localID << " ENTERING the intersection." << endl;
    cout << "It has been waiting at the light for: "<<duration<< " seconds."<<endl;
    if (direction == 1)
    {
        cout << "Car from SOUTH lane is heading towards NORTH direction." << endl;
    }
    else if (direction == 2)
    {
        cout << "Car from SOUTH lane is heading towards SOUTH direction." << endl;
    }
    else if (direction == 3)
    {
        cout << "Car from SOUTH lane is heading towards WEST direction." << endl;
    }
    else
    {
        cout << "Car from SOUTH lane is heading towards EAST direction." << endl;
    }
    sleep(3);
    cout << "Car from SOUTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
    pthread_mutex_unlock(&intersectionLock);
}

void *west(void *null)
{

    int localID;
    double duration;
    clock_t start;

    pthread_mutex_lock (&westLock); // locking the queue
    pthread_mutex_lock (&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in north lane
    pthread_mutex_unlock (&global_id_Lock);
    west_lane.push(localID); // pushing the local car into northQ.
    pthread_mutex_unlock (&westLock);

    sleep(5); //say it takes 5 seconds to get to the intersection
    pthread_mutex_lock(&intersectionQLock);
    intersection_lane.push(localID);
    pthread_mutex_unlock(&intersectionQLock);
    start = clock();

    while(1) //Checking cars properties here
    {
        if ((localID == west_lane.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
        {
            break;
        }
        else //Current car is not in front on its lane. Lets wait
        {
            usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
            continue;
        }
    }
    // Car is in the front so let's proceed to allow it to pass through intersection.
    pthread_mutex_lock(&intersectionLock); // need to lock the intersection. Function call will block until mutex is available
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    west_lane.pop();
    intersection_lane.pop();
    cout << "Car from WEST lane with ID:  " << localID << " ENTERING the intersection." << endl;
    cout << "It has been waiting for: "<< duration <<" seconds."<< endl;
    if (direction == 1)
    {
        cout << "Car from WEST lane is heading towards NORTH direction." << endl;
    }
    else if (direction == 2)
    {
        cout << "Car from WEST lane is heading towards SOUTH direction." << endl;
    }
    else if (direction == 3)
    {
        cout << "Car from WEST lane is heading towards WEST direction." << endl;
    }
    else
    {
        cout << "Car from WEST lane is heading towards EAST direction." << endl;
    }
    sleep(3);
    cout << "Car from WEST lane with ID:  " << localID << " LEAVING the intersection." << endl<<endl;
    pthread_mutex_unlock(&intersectionLock);
}

int main()
{
    init();
    srand(time(0));
    int k = 0;
    
    for(int i = 0; i < cars; i++) //first car will be car with ID 1
    {
        direction = getrand(1,5);
        k = getrand(1,5);
        if(k == 1)
        {   
            pthread_create (&threadID, NULL, north, NULL);
        }
        else if (k == 2)
        {
            pthread_create (&threadID, NULL, east, NULL);
        }
        else if (k == 3)
        {
            pthread_create (&threadID, NULL, south, NULL);
        }
        else
        {
            pthread_create (&threadID, NULL, west, NULL);
        }
        sleep(1); //a car will be created every # seconds.
    }
    sleep(run_time); //sleep for sufficient times to allow for all threads to finish running.
    cout << "Finished." << endl;
    return 0;
}   
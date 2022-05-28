

// Change the cars and run_time variable to the desired values.
//Directions: north: 1, south: 2, west: 3, east: 4


#include "intra_common.hpp"

const int cars = 100; // # cars all together
const int run_time = 5; // seconds the program will wait for to allow all cars to pass

using namespace std;

int main()
{
    init();
    srand(time(0));
    int k = 0;
    direction = getrand(1,5);
    
    for(int i = 0; i < cars; i++) //first car will be car with ID 1
    {
       
        k = getrand(1,5); // outgoing direction

        int d = directions(direction, k);

        if(d == 11)
        {   
            pthread_create (&threadID, NULL, northToNorth, NULL);
        }
        else if (d == 12)
        {
            pthread_create (&threadID, NULL, northToSouth, NULL);
        }
        else if (d == 13)
        {
            pthread_create (&threadID, NULL, northToWest, NULL);
        }
        else if (d == 14)
        {
            pthread_create (&threadID, NULL, northToEast, NULL);
        }else if (d == 21)
        {
            pthread_create (&threadID, NULL, southToNorth, NULL);
        }else if (d == 22)
        {
            pthread_create (&threadID, NULL, southToSouth, NULL);
        }else if (d == 23)
        {
            pthread_create (&threadID, NULL, southToWest, NULL);
        }else if (d == 24)
        {
            pthread_create (&threadID, NULL, southToEast, NULL);
        }else if (d == 31)
        {
            pthread_create (&threadID, NULL, westToNorth, NULL);
        }else if (d == 32)
        {
            pthread_create (&threadID, NULL, westToSouth, NULL);
        }else if (d == 33)
        {
            pthread_create (&threadID, NULL, westToWest, NULL);
        }else if (d == 34)
        {
            pthread_create (&threadID, NULL, westToEast, NULL);
        }else if (d == 41)
        {
            pthread_create (&threadID, NULL, eastToNorth, NULL);
        }else if (d == 42)
        {
            pthread_create (&threadID, NULL, eastToSouth, NULL);
        }else if (d == 43)
        {
            pthread_create (&threadID, NULL, eastToWest, NULL);
        }else if (d == 44)
        {
            pthread_create (&threadID, NULL, eastToEast, NULL);
        }
        sleep(1); //a car will be created every # seconds.
    }
    sleep(run_time); //sleep for sufficient times to allow for all threads to finish running.
    cout << "Finished." << endl;
    return 0;
}   
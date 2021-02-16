// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* and microseconds */
};

//ref: https://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows
int gettimeofday(struct timeval* tp, struct timezone* tzp) {
    namespace sc = std::chrono;
    sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
    sc::seconds s = sc::duration_cast<sc::seconds>(d);
    tp->tv_sec = (long)s.count();
    tp->tv_usec = (long)sc::duration_cast<sc::microseconds>(d - s).count();

    return 0;
}

int main()
{
    const unsigned int MAX_FPS = 30;
    const unsigned int FRAME_INTERVAL = 1000 / MAX_FPS;

    struct timeval tpNow;

    // get current time
    gettimeofday(&tpNow, NULL);
    unsigned long long now =
        (unsigned long long)(tpNow.tv_sec) * 1000 +
        (unsigned long long)(tpNow.tv_usec) / 1000;

    // get future time
    unsigned long long timerFPS = now + 1000;

    // frames per second
    int fps = 0;

    // measure FPS
    while (true)
    {
        // do something that takes time
        this_thread::sleep_for(chrono::milliseconds(FRAME_INTERVAL));
        ++fps;

        // get current time
        gettimeofday(&tpNow, NULL);
        now =
            (unsigned long long)(tpNow.tv_sec) * 1000 +
            (unsigned long long)(tpNow.tv_usec) / 1000;

        // print the fps every 1 second
        if (timerFPS < now)
        {
            cout << "FPS: " << fps << endl; //should be 30 FPS

            // reset FPS
            fps = 0;

            // get future time
            timerFPS = now + 1000;
        }
    }
}

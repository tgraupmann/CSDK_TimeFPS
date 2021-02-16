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

void ShowExpected()
{
    const unsigned int MAX_FPS = 30;
    const unsigned int FRAME_INTERVAL = 1000 / MAX_FPS;

    unsigned long long now = 0;
    unsigned long long timerFPS = now + 1000;

    // frames per second
    int fps = 0;

    // measure FPS
    while (true)
    {
        // get current time
        now += FRAME_INTERVAL;

        // print the fps every 1 second
        if (timerFPS < now)
        {
            cout << "FPS: " << fps << endl; //should be 30 FPS

            // reset FPS
            fps = 0;

            // get future time
            timerFPS = now + 1000;
        }
        else
        {
            // do something that takes time
            ++fps;
        }
    }
}

void ShowUsingClock()
{
    const unsigned int MAX_FPS = 30;
    const unsigned int FRAME_INTERVAL = 1000 / MAX_FPS;

    struct timeval tpNow;

    unsigned long long now = 0;
    unsigned long long timerFPS = now + 1000;

    // frames per second
    int fps = 0;

    // measure FPS
    while (true)
    {
        // Measure time before task
        gettimeofday(&tpNow, NULL);
        unsigned long long timeStart =
            (unsigned long long)(tpNow.tv_sec) * 1000 +
            (unsigned long long)(tpNow.tv_usec) / 1000;

        // do something that takes time
        this_thread::sleep_for(chrono::milliseconds(FRAME_INTERVAL/2)); //sleep half the time ends up taking double

        // Measure time after task
        gettimeofday(&tpNow, NULL);
        unsigned long long timeEnd =
            (unsigned long long)(tpNow.tv_sec) * 1000 +
            (unsigned long long)(tpNow.tv_usec) / 1000;

        unsigned long long elapsed = (timeEnd - timeStart);
        cout << "Elapsed: " << elapsed << endl;

        // get current time
        now += elapsed;

        // print the fps every 1 second
        if (timerFPS < now)
        {
            cout << "FPS: " << fps << endl; //should be 30 FPS

            // reset FPS
            fps = 0;

            // get future time
            timerFPS = now + 1000;
        }
        else
        {
            ++fps;
        }
    }
}

// ref: https://en.cppreference.com/w/cpp/thread/yield
// for a small amount of time
void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}

void ShowOriginalLogic()
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
        auto start = std::chrono::high_resolution_clock::now();
        // you can only wait 1ms once because it can take 20 ms.
        this_thread::sleep_for(chrono::milliseconds(1));
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        //std::cout << "Slept " << elapsed.count() << " ms\n";

        // block remainder
        little_sleep(33ms - std::chrono::duration_cast<std::chrono::microseconds>(elapsed));
        
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        //std::cout << "Final frame time: " << elapsed.count() << " ms\n";

        // get current time
        gettimeofday(&tpNow, NULL);
        now =
            (unsigned long long)(tpNow.tv_sec) * 1000 +
            (unsigned long long)(tpNow.tv_usec) / 1000;

        // print the fps every 1 second
        if (timerFPS <= now)
        {
            cout << "FPS: " << fps << endl; //should be 30 FPS

            // reset FPS
            fps = 0;

            // get future time
            timerFPS = now + 1000;
        }
        else
        {
            ++fps;
        }
    }
}

int main()
{
    //ShowExpected();

    //ShowUsingClock();

    ShowOriginalLogic();

    return 0;
}

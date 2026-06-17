#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <windows.h>  // for Sleep()

#define NEARZERO 1e-12
inline bool isEqual(double x, double y) { return fabs(x - y) < NEARZERO; }

/******************************************************************************/ 
class cartCentering {
protected:
    // simulation parameters
    const double MASSCART = 2.0;
    const double FORCE_MAG = 1.0;
    const double TAU = 0.02;  // seconds between updates
    const double MAX_X = 1.5;
    const double MAX_V = 6.0;
    const double MIN_VAR_INI = -0.75;
    const double MAX_VAR_INI = 0.75;
    const double NEAR_ORIGIN = 0.01;

    const int STATE_SIZE = 2;
    const int X = 0;
    const int V = 1;

    std::vector<double> state;
    int step;
    int max_step;
    bool draw_track = false;
    std::uniform_real_distribution<> disReset;

public:
    cartCentering() {
        disReset = std::uniform_real_distribution<>(MIN_VAR_INI, MAX_VAR_INI);
        max_step = 500;
        state.resize(STATE_SIZE);
    }

    ~cartCentering() {}

    /************************************************************************/
    void reset(std::mt19937& rng) {
        step = 0;
        do {
            state[X] = disReset(rng);
            state[V] = disReset(rng);
        } while (terminal());
    }

    bool terminal() {
        if (step >= max_step) return true;
        if (fabs(state[X]) <= NEAR_ORIGIN && fabs(state[V]) <= NEAR_ORIGIN) return true;
        if (fabs(state[X]) > MAX_X) return true;
        return false;
    }

    double update(const int& action, bool animate = false) {
        double force = (action < 0) ? -FORCE_MAG : FORCE_MAG;
        double acc_t = force / MASSCART;

        state[X] += TAU * state[V];
        state[V] += TAU * acc_t;
        state[V] = bound(state[V], -MAX_V, MAX_V);
        step++;

        if (animate) draw(action);

        if (terminal()) {
            double x = (fabs(state[X]) / MAX_X) * 1.0;
            double v = (fabs(state[V]) / MAX_V) * 0.5;
            double s = ((double)step / max_step) * 0.25;
            return -(x + v + s);
        } else {
            return 0;
        }
    }

    double bound(double x, double m, double M) {
        return std::min(std::max(x, m), M);
    }

    double getCartXPos() { return state[X]; }
    double getCartXVel() { return state[V]; }
    void setDraw(bool d) { draw_track = d; }

    /************************************************************************/
    void draw(const int& action) {
        clearScreen();
        bool solved = (fabs(state[X]) <= NEAR_ORIGIN && fabs(state[V]) <= NEAR_ORIGIN);

        std::cout << "Step: " << step << std::endl;
        std::cout << "X " << std::setprecision(3) << state[X] << std::endl;
        std::cout << "V " << std::setprecision(3) << state[V] << std::endl;
        std::cout << "Action: " << ((action < 0) ? "<--" : "-->") << std::endl;
        if (terminal())
            std::cout << "Solved: " << (solved ? "YES!" : "NO") << std::endl;
        else
            std::cout << "Solved:" << std::endl;

        const int track_length = 121;
        int x = int((state[X] + MAX_X) * 40);  // map (-1.5,1.5) -> (0,120)
        std::string s(track_length, ' ');
        if (x >= 0 && x < track_length) s.replace(x, 1, "*");
        std::cout << s << std::endl;

        // draw track
        s = std::string(track_length, '_');
        s.replace(60, 1, "|");  // center position
        std::cout << s << std::endl;

        std::cout << std::flush;

        if (!terminal()) Sleep(50);  // pause 50 ms
        else Sleep(3000);            // final pause if done
    }

    void clearScreen() {
        system("cls");  // works in Windows CMD / PowerShell
    }
};
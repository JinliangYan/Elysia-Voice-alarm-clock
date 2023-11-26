#include "HomePage.h"
#include "Clock.h"

int main() {
    Clock_Init();
    HomePage_Init();

    while (1) {
        Clock_Loop();
        HomePage_Loop();
    }
    return 0;
}

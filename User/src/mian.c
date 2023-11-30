#include "HomePage.h"
#include "Clock.h"
#include "Voice.h"
#include "Key.h"
#include "Timer3.h"
#include "../../Libraries/printf_/printf.h"         //for debug

void System_Init(void);
void Debug_Init(void);
int main() {
    Debug_Init();

    System_Init();
    while (1) {
        Clock_Update();
        HomePage_Update();
    }
    return 0;
}

void System_Init(void) {
    Timer3_Init();       // Key.c required
    Key_Init();
    Clock_Init();
    HomePage_Init();
    Voice_Init(20);
}

void Debug_Init(void) {
    printf_init();
}
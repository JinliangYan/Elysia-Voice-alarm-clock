#include "HomePage.h"
#include "Clock.h"
#include "Voice.h"
#include "Key.h"
#include "Timer.h"

int main() {
    Timer_Init();
    Key_Init();
    Clock_Init();
    HomePage_Init();
    Voice_Init(20);

    while (1) {
        Clock_Update();
        HomePage_Update();
        if (Key_Get() == 1)
            Voice_Invoke();
    }
    return 0;
}

void TIM3_IRQHandler(void) {
    static uint8_t counter1, counter2;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
        counter1++;
        if (counter1 == 15) {
            counter1 = 0;
            Key_Loop();         //20ms调用一次按键驱动函数
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
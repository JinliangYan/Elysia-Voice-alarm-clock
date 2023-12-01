//
// Created by Jinliang on 11/23/2023.
//

#include "Voice.h"
#include "Clock.h"
#include "DFPLAYER_MINI.h"

#define LOG_TAG "VOICE"
#include "elog.h"

#define USE_HARDWARE_DECODING
//#define USE_SOFTWARE_DECODING

/* 定义语音类别和对应数目 */
/* 天气 */
#define VOICE_WEATHER_BASE                        1
#define VOICE_WEATHER_RAIN                        (VOICE_WEATHER_BASE + 0)
#define VOICE_WEATHER_RAIN_NUM                    2    // Number of rainy weather responses
#define VOICE_WEATHER_SUNNY                       (VOICE_WEATHER_BASE + 1)
#define VOICE_WEATHER_SUNNY_NUM                   2    // Number of sunny weather responses
#define VOICE_WEATHER_COOL_DOWN                   (VOICE_WEATHER_BASE + 2)
#define VOICE_WEATHER_COOL_DOWN_NUM               1    // Number of cool-down weather responses

/* 情景 */
#define VOICE_SCENE_BASE                          11
#define VOICE_SCENE_REST_TIME                     (VOICE_SCENE_BASE + 0)
#define VOICE_SCENE_REST_TIME_NUM                 7    // Number of responses for rest time scene
#define VOICE_SCENE_TASK_SET                      (VOICE_SCENE_BASE + 1)
#define VOICE_SCENE_TASK_SET_NUM                  1    // Number of responses for task set scene
#define VOICE_SCENE_TASK_ACCOMPLISHED             (VOICE_SCENE_BASE + 2)
#define VOICE_SCENE_TASK_ACCOMPLISHED_NUM         4    // Number of responses for task accomplished scene
#define VOICE_SCENE_GREETING                      (VOICE_SCENE_BASE + 3)
#define VOICE_SCENE_GREETING_NUM                  9    // Number of responses for greeting scene
#define VOICE_SCENE_WAKE_UP                       (VOICE_SCENE_BASE + 4)
#define VOICE_SCENE_WAKE_UP_NUM                   2    // Number of responses for wake-up scene
#define VOICE_SCENE_HANG_OUT                      (VOICE_SCENE_BASE + 5)
#define VOICE_SCENE_HANG_OUT_NUM                  5    // Number of responses for hang-out scene
#define VOICE_SCENE_FAILURE                       (VOICE_SCENE_BASE + 6)
#define VOICE_SCENE_FAILURE_NUM                   1    // Number of responses for failure scene

/* 互动 */
#define VOICE_INTERACTION_BASE                    21
#define VOICE_INTERACTION_CHAT                    (VOICE_INTERACTION_BASE + 0)
#define VOICE_INTERACTION_CHAT_NUM                84   // Number of chat responses
#define VOICE_INTERACTION_EAT                     (VOICE_INTERACTION_BASE + 1)
#define VOICE_INTERACTION_EAT_NUM                 5    // Number of responses for eating interaction
#define VOICE_INTERACTION_LIFT                    (VOICE_INTERACTION_BASE + 2)
#define VOICE_INTERACTION_LIFT_NUM                2    // Number of responses for lifting interaction
#define VOICE_INTERACTION_NEW_CLOTHES             (VOICE_INTERACTION_BASE + 3)
#define VOICE_INTERACTION_NEW_CLOTHES_NUM         7    // Number of responses for new clothes interaction
#define VOICE_INTERACTION_SHAKE                   (VOICE_INTERACTION_BASE + 4)
#define VOICE_INTERACTION_SHAKE_NUM               8    // Number of responses for shaking interaction
#define VOICE_INTERACTION_THANKS                  (VOICE_INTERACTION_BASE + 5)
#define VOICE_INTERACTION_THANKS_NUM              3    // Number of responses for thanks interaction

/* 时间 */
#define VOICE_TIME_BASE                           31
#define VOICE_TIME_MORNING_GREETING               (VOICE_TIME_BASE + 0)
#define VOICE_TIME_MORNING_GREETING_NUM           7    // Number of morning greeting responses
#define VOICE_TIME_EVENING                        (VOICE_TIME_BASE + 1)
#define VOICE_TIME_EVENING_NUM                    5    // Number of evening responses
#define VOICE_TIME_MIDNIGHT                       (VOICE_TIME_BASE + 2)
#define VOICE_TIME_MIDNIGHT_NUM                   2    // Number of midnight responses

/* 季节 */
#define VOICE_SEASON_BASE                         41
#define VOICE_SEASON_WINTER                       (VOICE_SEASON_BASE + 0)
#define VOICE_SEASON_WINTER_NUM                   2    // Number of responses for winter season
#define VOICE_SEASON_AUTUMN                       (VOICE_SEASON_BASE + 1)
#define VOICE_SEASON_AUTUMN_NUM                   1    // Number of responses for autumn season
#define VOICE_SEASON_SUMMER                       (VOICE_SEASON_BASE + 2)
#define VOICE_SEASON_SUMMER_NUM                   1    // Number of responses for summer season

/* 杂项 */
#define VOICE_MISC_BASE                           81
#define VOICE_MISC_CHARACTER_BIRTHDAY             (VOICE_MISC_BASE + 0)
#define VOICE_MISC_CHARACTER_BIRTHDAY_NUM         2    // Number of responses for character birthday
#define VOICE_MISC_BIRTHDAY                       (VOICE_MISC_BASE + 1)
#define VOICE_MISC_BIRTHDAY_NUM                   2    // Number of responses for general birthday
#define VOICE_MISC_MONDAY                         (VOICE_MISC_BASE + 2)
#define VOICE_MISC_MONDAY_NUM                     1    // Number of responses for Monday

/* 默认 */
#define VOICE_DEFAULT                             VOICE_INTERACTION_CHAT
#define VOICE_DEFAULT_NUM                         VOICE_INTERACTION_CHAT_NUM

/*音乐*/
#define VOICE_MUSIC_RESOURCE                      80
#define VOICE_MUSIC_NUM                           84

#define VOICE_VOLUME_MAX  (30)

static Voice_StatusDef Voice_Status;
static uint8_t Voice_Volume;

static int8_t Voice_MusicNow = 1;

static void Voice_Say(uint8_t category, uint8_t number) {
    if (Voice_Status == VOICE_OFF)
        return;
#ifdef USE_HARDWARE_DECODING
    DF_PlayFromFolder(category, number);
    log_i("DF_PlayFromFolder(%d, %d) Invoked", category, number);
#endif
#ifdef USE_STDPERIPH_DRIVER

#endif
}

static uint8_t Voice_Random(uint8_t number) {
    return (Clock_Second + Clock_Minute + Clock_Hour) % number + 1;
}

void Voice_Weather(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = Voice_Random(VOICE_INTERACTION_CHAT_NUM);;
//    Replace天气判断 with your actual weather condition check
//    if (/*天气判断*/) {
//        scene = VOICE_WEATHER_RAIN; // Or VOICE_WEATHER_SUNNY, etc.
//        number = Voice_Random(VOICE_WEATHER_NUM);
//    }
    Voice_Say(scene, number);
}

void Voice_Scene(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = Voice_Random(VOICE_INTERACTION_CHAT_NUM);;
    if (Clock_IsSleepTime()) {
        scene = VOICE_TIME_MIDNIGHT;
        number = Voice_Random(VOICE_TIME_MIDNIGHT_NUM);
    }
    else if (Clock_IsGetUpTime()) {
        scene = VOICE_SCENE_WAKE_UP;
        number = Voice_Random(VOICE_SCENE_WAKE_UP);
    }
    // else if (/*任务完成*/) {
    //     scene = VOICE_SCENE_MISSION_ACCOMPLISHED;
    //     number = Voice_Random(VOICE_SCENE_NUM);
    // }
    Voice_Say(scene, number);
}

void Voice_Chat(void) {
    uint8_t number = Voice_Random(VOICE_INTERACTION_CHAT_NUM);
    Voice_Say(VOICE_INTERACTION_CHAT, number);
}

void Voice_DayOfTime(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = Voice_Random(VOICE_INTERACTION_CHAT_NUM);;
    if (Clock_TimeOfDay == CLOCK_MORNING) {
        scene = VOICE_TIME_MORNING_GREETING;
        number = Voice_Random(VOICE_TIME_MORNING_GREETING_NUM);
    } else if (Clock_TimeOfDay == CLOCK_AFTERNOON) {
        // Handle afternoon
    } else if (Clock_TimeOfDay == CLOCK_DUSK) {
        // Handle dusk
    } else if (Clock_TimeOfDay == CLOCK_EVENING) {
        scene = VOICE_TIME_EVENING;
        number = Voice_Random(VOICE_TIME_EVENING_NUM);
    } else if (Clock_TimeOfDay == CLOCK_MIDNIGHT) {
        scene = VOICE_TIME_MIDNIGHT;
        number = Voice_Random(VOICE_TIME_MIDNIGHT_NUM);
    }
    Voice_Say(scene, number);
}

void Voice_Season(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = Voice_Random(VOICE_INTERACTION_CHAT_NUM);;
    if (Clock_Season == CLOCK_WINTER) {
        scene = VOICE_SEASON_WINTER;
        number = Voice_Random(VOICE_SEASON_WINTER_NUM);
    }
    Voice_Say(scene, number);
}

void Voice_BirthDay(uint8_t meOrAlysia) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = Voice_Random(VOICE_INTERACTION_CHAT_NUM);;
    if (meOrAlysia == 0) {
        scene = VOICE_MISC_CHARACTER_BIRTHDAY;
        number = Voice_Random(VOICE_MISC_CHARACTER_BIRTHDAY_NUM);
    } else {
        scene = VOICE_MISC_BIRTHDAY;
        number = Voice_Random(VOICE_MISC_BIRTHDAY_NUM);
    }
    Voice_Say(scene, number);
}

void Voice_Invoke(void) {
    // 随机选择要触发的函数
    int randomFunction = Voice_Random(5); // 选择 0 到 5 之间的随机数

    // 根据随机数调用相应的函数
    switch (randomFunction) {
        case 0:
            Voice_Weather();
            break;
        case 1:
            Voice_Scene();
            break;
        case 2:
            Voice_Chat();
            break;
        case 3:
            Voice_DayOfTime();
            break;
        case 4:
            Voice_Season();
            break;
        case 5:
            Voice_BirthDay(0); // 传递 0 表示触发角色生日
            Voice_BirthDay(1);
            break;
            // 添加其他函数触发的 case，如果有的话
        default:
            Voice_Chat();
    }
}
void Voice_ON(void) {
    Voice_Status = VOICE_ON;
}

void Voice_OFF(void) {
    Voice_Status = VOICE_OFF;
}

void Voice_SetVolume(uint16_t volume) {
    Voice_Volume = volume;
    DF_SetVolume(volume);
}

void Voice_VolumeIncrease(void) {
    if (Voice_Volume == VOICE_VOLUME_MAX)
        return;
    Voice_Volume++;
    DF_SetVolume(Voice_Volume);
}

void Voice_VolumeDecrease(void) {
    if (Voice_Volume == 0)
        return;
    Voice_Volume--;
    DF_SetVolume(Voice_Volume);
}

///////////////////////////////////////////////////////////////
////////////////////////// MUSIC //////////////////////////////
void Voice_MusicPlay(void) {
    log_i("Voice_MusicPlay invoked");
    DF_PlayFromFolder(VOICE_MUSIC_RESOURCE, Voice_MusicNow);
}

void Voice_MusicPause(void) {
    log_i("Voice_MusicPause invoked");
    DF_Pause();
}

void Voice_MusicContinue(void) {
    log_i("Voice_MusicContinue invoked");
    DF_Continue();
}

void Voice_MusicNext(void) {
    log_i("Voice_MusicNext invoked");
    Voice_MusicNow++;
    if (Voice_MusicNow > VOICE_MUSIC_NUM) Voice_MusicNow = 1;
    DF_PlayFromFolder(VOICE_MUSIC_RESOURCE, Voice_MusicNow);
}

void Voice_MusicPrevious(void) {
    log_i("Voice_MusicPrevious invoked");
    Voice_MusicNow--;
    if (Voice_MusicNow < 1) Voice_MusicNow = VOICE_MUSIC_NUM;
    DF_PlayFromFolder(VOICE_MUSIC_RESOURCE, Voice_MusicNow);
}

///////////////////////////////////////////////////////////////

void Voice_Init(uint8_t volume) {
#ifdef USE_HARDWARE_DECODING
    DF_Init(volume);
    Voice_Status = VOICE_ON;
#endif
#ifdef USE_STDPERIPH_DRIVER
    // Initialize other components if needed
#endif
}
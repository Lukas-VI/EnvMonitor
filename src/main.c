/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "system_data.h"
#include "sensor_task.h"
#include "monitor_task.h"
#include "config.h"

// 能够跨平台的时间相关函数定义~
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    // 调取系统时钟
    static unsigned long get_tick_ms(void)
    {
        return (unsigned long)GetTickCount64();
    }

    // 延时
    static void sleep_ms(unsigned long ms)
    {
        Sleep((DWORD)ms);
    }
#else
    #include <time.h>

    static unsigned long get_tick_ms(void)
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    }

    static void sleep_ms(unsigned long ms)
    {
        struct timespec req = { ms / 1000, (ms % 1000) * 1000000L };
        nanosleep(&req, NULL);
    }
#endif

int main(void)
{
    SystemContext ctx;

    system_init(&ctx);
    srand((unsigned)time(NULL));

    printf("Hello, Welcome to %s!\n", PROJECT_NAME);
    printf("===== 环境监测系统启动 =====\n\n");

    // 初始化采样时间
    unsigned long last_sample  = (unsigned long)(-TASK_SAMPLE_PERIOD_MS);
    unsigned long last_monitor = (unsigned long)(-TASK_MONITOR_PERIOD_MS);

    // 循环采样
    while (ctx.alarm.alarm_cnt < MAX_ALARM_CNT)
    {
        unsigned long now = get_tick_ms();

        // 通过计算上次采样与当前时间的差值决定采样时机
        if (now - last_sample >= TASK_SAMPLE_PERIOD_MS)
        {
            sensor_task(&ctx);
            last_sample = now;
        }

        if (now - last_monitor >= TASK_MONITOR_PERIOD_MS)
        {
            monitor_task(&ctx);
            last_monitor = now;
        }

        sleep_ms(10);  // 防止反复循环CPU爆掉
    }

    printf("\n===== 系统退出：累计告警 %d 次，达到上限 =====\n",
           ctx.alarm.alarm_cnt);
    return 0;
}
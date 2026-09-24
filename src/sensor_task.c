/* sensor_task.c */
#include <stdlib.h>
#include "sensor_task.h"
#include "config.h"

// 生成随机数据
static float rand_float(float min, float max)
{
    return min + (max - min) * ((float)rand() / RAND_MAX);
}

void sensor_task(SystemContext *ctx)
{
    // 模拟采集并判断阈值
    ctx->env.temp = rand_float(TEMP_ABS_MIN, TEMP_ABS_MAX);
    ctx->env.hum  = rand_float(HUM_ABS_MIN, HUM_ABS_MAX);

    if (ctx->env.temp > TEMP_ALARM_HIGH ||
        ctx->env.temp < TEMP_ALARM_LOW  ||
        ctx->env.hum  > HUM_ALARM_HIGH)
    {
        ctx->alarm.alarm_flag = 1;   // 锁存
    }
}
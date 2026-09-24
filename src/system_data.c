/* system_data.c */
#include "system_data.h"

// 初始化系统上下文
void system_init(SystemContext *ctx)
{
    ctx->env.temp = 25.0f;
    ctx->env.hum  = 50.0f;
    ctx->alarm.alarm_flag = 0;
    ctx->alarm.alarm_cnt  = 0;
}
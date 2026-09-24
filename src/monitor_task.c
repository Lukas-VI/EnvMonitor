/* monitor_task.c */
#include <stdio.h>
#include "monitor_task.h"

void monitor_task(SystemContext *ctx)
{
    printf("=====环境监测报告=====\n");
    printf("当前温度：%.1f℃，当前湿度：%.1f%%RH\n",
           ctx->env.temp, ctx->env.hum);

    if (ctx->alarm.alarm_flag == 1)
    {
        printf("状态：【告警】监测到环境参数异常！\n");
        ctx->alarm.alarm_cnt++;          // 计数
        ctx->alarm.alarm_flag = 0;       // 置0清除锁存
    }
    else
    {
        printf("状态：正常\n");
    }

    printf("累计告警次数：%d\n", ctx->alarm.alarm_cnt);
    printf("======================\n\n");
}
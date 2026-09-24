/* system_data.h */
#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

// 注:因为要求的全局变量很不优雅, 所以在我的实现中采用结构体指针构建系统上下文的方法替代

typedef struct {
    float temp;         // 当前温度 ℃
    float hum;          // 当前湿度 %RH
} EnvData;

typedef struct {
    int alarm_flag;     // 锁存标志
    int alarm_cnt;      // 累计告警次数
} AlarmState;

typedef struct {
    EnvData    env;     // 环境数据
    AlarmState alarm;   // 告警状态
} SystemContext;

void system_init(SystemContext *ctx);

#endif
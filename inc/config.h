/* config.h */
#ifndef CONFIG_H
#define CONFIG_H

#define PROJECT_NAME "EnvMonitor"

/* ---- 任务周期 ---- */
#define TASK_SAMPLE_PERIOD_MS    1000
#define TASK_MONITOR_PERIOD_MS   2000

/* ---- 采集范围  ---- */
#define TEMP_ABS_MIN   (-10.0f)
#define TEMP_ABS_MAX   ( 50.0f)
#define HUM_ABS_MIN    (  0.0f)
#define HUM_ABS_MAX    (100.0f)

/* ---- 告警阈值 ---- */
#define TEMP_ALARM_HIGH   (40.0f)
#define TEMP_ALARM_LOW    ( 0.0f)
#define HUM_ALARM_HIGH    (80.0f)

/* ---- 系统终止条件 ---- */
#define MAX_ALARM_CNT     5

#endif
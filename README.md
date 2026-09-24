# C 模拟环境监视器（EnvMonitor）

基于 C 语言实现的软件模拟嵌入式多任务系统：模拟温湿度周期性采集与告警监测，支持跨平台编译（Windows / Linux）。

## 一、项目简介

本项目用"时间片轮询"方式模拟嵌入式多任务系统，包含两个独立任务：

| 任务 | 周期 | 职责 |
|------|------|------|
| 采集任务 | 1 秒 | 模拟采集温湿度，更新全局状态，判定异常并锁存告警标志 |
| 监测任务 | 2 秒 | 读取状态，打印监测报告，上报并清零告警标志，累计告警次数 |

系统终止条件：累计告警次数 `alarm_cnt >= 5` 时自动退出。

## 二、目录结构

```
EnvMonitor/
├─ .vscode/                      # VSCode 配置
│  ├─ tasks.json                 # 编译任务
│  ├─ launch.json                # 调试配置
│  └─ c_cpp_properties.json      # 头文件智能提示
├─ inc/                          # 头文件 .h
│  ├─ config.h                   # 常量配置（周期、阈值、上限）
│  ├─ system_data.h              # 系统数据结构定义
│  ├─ sensor_task.h              # 任务1：采集任务接口
│  └─ monitor_task.h             # 任务2：监测任务接口
├─ src/                          # 源码 .c
│  ├─ main.c                     # 主循环 + 调度器
│  ├─ system_data.c              # 状态初始化
│  ├─ sensor_task.c              # 采集任务实现
│  └─ monitor_task.c             # 监测任务实现
└─ bin/                          # 编译输出 exe（自动生成）
```

## 三、使用方法

1. 复制本文件夹，重命名为你的项目名
2. VSCode → 文件 → 打开文件夹 → 选中重命名后的文件夹
3. 写代码
4. `Ctrl+F5` 编译并运行，`F5` 编译并断点调试，`Ctrl+Shift+B` 只编译

### 命令行编译（Windows / MinGW）

```powershell
$src = (Get-ChildItem '.\src\*.c').FullName
gcc -g $src -I '.\inc' -o '.\bin\EnvMonitor.exe' -Wall
.\bin\EnvMonitor.exe
```

### 命令行编译（Linux / macOS）

```bash
gcc -g src/*.c -I inc -o bin/EnvMonitor -Wall
./bin/EnvMonitor
```

## 四、系统状态与全局变量

所有状态通过结构体 `SystemContext` 打包，避免散落的全局变量：

```c
typedef struct {
    float temp;             // 最新采集温度 ℃
    float hum;              // 最新采集湿度 %RH
} EnvData;

typedef struct {
    int alarm_flag;         // 告警锁存标志：0=无未处理异常，1=有异常未上报
    int alarm_cnt;          // 累计告警次数
} AlarmState;

typedef struct {
    EnvData    env;         // 环境数据
    AlarmState alarm;       // 告警状态
} SystemContext;
```

## 五、阈值与规则

| 判定项 | 条件 |
|--------|------|
| 温度过高 | `temp > 40.0 ℃` |
| 温度过低 | `temp < 0.0 ℃` |
| 湿度过高 | `hum > 80.0 %RH` |

## 六、运行样例

```
===== 环境监测系统启动 =====

=====环境监测报告=====
当前温度：28.5℃，当前湿度：62.3%RH
状态：正常
累计告警次数：0
======================

=====环境监测报告=====
当前温度：42.1℃，当前湿度：55.0%RH
状态：【告警】监测到环境参数异常！
累计告警次数：1
======================

=====环境监测报告=====
当前温度：33.2℃，当前湿度：70.1%RH
状态：正常
累计告警次数：1
======================

=====环境监测报告=====
当前温度：43.5℃，当前湿度：82.0%RH
状态：【告警】监测到环境参数异常！
累计告警次数：2
======================

===== 系统退出：累计告警 5 次，达到上限 =====
```

## 七、架构设计要点

### 1. 时间片轮询调度

主循环计时通过计算系统时间戳的差实现, 让两个任务互不干扰, 也不会积累误差：

```c
if (now - last_sample >= TASK_SAMPLE_PERIOD_MS) {
    sensor_task(&ctx);
    last_sample = now;
}
```

### 2. 职责分离

| 操作 | 执行者 | 时机 |
|------|--------|------|
| 置 `alarm_flag = 1` | 采集任务 | 检测到异常时 |
| 读 `alarm_flag` | 监测任务 | 打印报告时 |
| 清 `alarm_flag = 0` | 监测任务 | 上报后 |
| 累加 `alarm_cnt` | 监测任务 | 上报时 |

方法对flag的操作职责明确以避免混乱

### 3. 常量集中管理

所有魔数定义在 `config.h` 中进行了宏定义：

```c
#define TASK_SAMPLE_PERIOD_MS   1000
#define TASK_MONITOR_PERIOD_MS  2000
#define TEMP_ALARM_HIGH         (40.0f)
#define TEMP_ALARM_LOW          ( 0.0f)
#define HUM_ALARM_HIGH          (80.0f)
#define MAX_ALARM_CNT           5
```

## 八、跨平台兼容

`main.c` 中的时间函数用 `#if defined(_WIN32)` 做了平台分支：

| 平台 | 计时 | 睡眠 | 头文件 |
|------|------|------|--------|
| Windows | `GetTickCount64()` | `Sleep(ms)` | `<windows.h>` |
| Linux/macOS | `clock_gettime()` | `nanosleep()` | `<time.h>` |

因此在 Windows + MinGW 和 Linux + GCC 下均可直接编译，无需改代码。


## 九、许可

MIT
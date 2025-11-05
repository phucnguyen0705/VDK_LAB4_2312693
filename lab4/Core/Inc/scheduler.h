#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stdint.h"

#define SCH_MAX_TASKS   40
#define NO_TASK_ID      0
#define CYCLE           10
void SCH_Init(void);
void SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
void SCH_Delete_Task(uint8_t TASK_ID);

typedef struct {
    void   (*pTask)(void);
    uint32_t Delay;
    uint32_t Period;
    uint8_t  RunMe;
    uint32_t TaskID;
} sTask;
void task0(void);
void task1(void);
void task2(void);
void task3(void);
void task4(void);
extern sTask SCH_task_global[SCH_MAX_TASKS];

#endif

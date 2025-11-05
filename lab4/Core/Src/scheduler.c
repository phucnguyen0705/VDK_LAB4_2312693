
#include "scheduler.h"
#include "main.h"
#include "scheduler.h"
#include "main.h"


sTask SCH_tasks_global[SCH_MAX_TASKS];

void SCH_Delete_Task(const uint8_t TASK_INDEX)
{
    SCH_tasks_global[TASK_INDEX].pTask  = 0;
    SCH_tasks_global[TASK_INDEX].Delay  = 0;
    SCH_tasks_global[TASK_INDEX].Period = 0;
    SCH_tasks_global[TASK_INDEX].RunMe  = 0;
}

void SCH_Init(void)
{
    uint8_t i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_Delete_Task(i);
    }
    SCH_Add_Task(task0, 0, 500);
    SCH_Add_Task(task1, 0, 1000);
    SCH_Add_Task(task2, 0, 1500);
    SCH_Add_Task(task3, 0, 2000);
    SCH_Add_Task(task4, 0, 2500);
}

void SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD)
{
    DELAY  = DELAY  / CYCLE;
    PERIOD = PERIOD / CYCLE;

    uint8_t index, indexPre;

    for (index = 0; index < SCH_MAX_TASKS; index++) {
        if (SCH_tasks_global[index].pTask == 0) break;
    }

    if (index == 0) {
        SCH_tasks_global[index].pTask  = pFunction;
        SCH_tasks_global[index].Delay  = DELAY;
        SCH_tasks_global[index].Period = PERIOD;
        SCH_tasks_global[index].RunMe  = 0;
        return;
    }

    if (index == SCH_MAX_TASKS) return;

    for (int i = 0; i <= (int)index; i++) {
        if (DELAY >= SCH_tasks_global[i].Delay && SCH_tasks_global[i].pTask) {
            DELAY -= SCH_tasks_global[i].Delay;
        } else {
            indexPre = i;
            SCH_tasks_global[i].Delay -= DELAY;
            break;
        }
        indexPre = i;
    }

    for (int i = (int)index; i > (int)indexPre; i--) {
        SCH_tasks_global[i] = SCH_tasks_global[i-1];
    }

    SCH_tasks_global[indexPre].pTask  = pFunction;
    SCH_tasks_global[indexPre].Delay  = DELAY;
    SCH_tasks_global[indexPre].Period = PERIOD;
    SCH_tasks_global[indexPre].RunMe  = 0;
}

void SCH_Update_ListTask(uint8_t index)
{
    for (uint8_t i = index; i < SCH_MAX_TASKS - 1; i++) {
        SCH_tasks_global[i] = SCH_tasks_global[i+1];
    }
    SCH_Delete_Task(SCH_MAX_TASKS - 1);
}

void SCH_Dispatch_Tasks(void)
{
    uint8_t index = 0;
    sTask temp;

    for (index = 0; index < SCH_MAX_TASKS; index++) {
        if (SCH_tasks_global[index].RunMe) {
            (*SCH_tasks_global[index].pTask)();
            SCH_tasks_global[index].RunMe -= 1;

            temp = SCH_tasks_global[index];

            SCH_Update_ListTask(index);

            if (temp.Period) {
                SCH_Add_Task(temp.pTask, temp.Period * CYCLE, temp.Period * CYCLE);
            }
        }
    }
}

void SCH_Update(void)
{
    if (SCH_tasks_global[0].Delay == 0) {
        SCH_tasks_global[0].RunMe = 1;
    } else {
        SCH_tasks_global[0].Delay -= 1;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        SCH_Update();
    }
}
void task0(void) { HAL_GPIO_TogglePin(l0_GPIO_Port, l0_Pin); }
void task1(void) { HAL_GPIO_TogglePin(l1_GPIO_Port, l1_Pin); }
void task2(void) { HAL_GPIO_TogglePin(l2_GPIO_Port, l2_Pin); }
void task3(void) { HAL_GPIO_TogglePin(l3_GPIO_Port, l3_Pin); }
void task4(void) { HAL_GPIO_TogglePin(l4_GPIO_Port, l4_Pin); }

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <memory>
#include "TaskControlBlock.h"
#include "Scheduler.h"
#include "CPU.h"

class Simulator {
private:
    CPU cpu;
    Scheduler scheduler;
    int time_quantum;
    
    // Lista de todas as tarefas que existirão na simulação
    std::vector<std::shared_ptr<TaskControlBlock>> all_tasks;
    int terminated_task_count = 0;

    void admit_new_tasks();
    void print_simulation_tick_details();
    void print_final_report();

public:
    Simulator(int quantum);
    void create_task(int id, int arrival_time, int burst_time);
    void run();
};

#endif // SIMULATOR_H

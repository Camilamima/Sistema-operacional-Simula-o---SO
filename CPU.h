#ifndef CPU_H
#define CPU_H

#include <memory>
#include "TaskControlBlock.h"
// Variável externa para o relógio, será definida no Simulator.cpp
extern int global_clock;
class CPU {
private:
    std::shared_ptr<TaskControlBlock> current_task;
    int time_slice_remaining; // Contador para o quantum do Round-Robin

public:

    
    // Associa uma tarefa à CPU para execução
    void dispatch(std::shared_ptr<TaskControlBlock> task, int quantum) {
        current_task = task;
        if (current_task) {
            current_task->state = TaskState::RUNNING;
            time_slice_remaining = quantum;
        }
    }

    // Simula a execução de um tick de clock
    // Retorna true se a tarefa terminou ou se o quantum expirou
    bool execute_tick() {
        if (!current_task) {
            return false; // CPU ociosa
        }

        // Se é a primeira vez que a tarefa executa, marca o start_time
        if (current_task->start_time == -1) {
            current_task->start_time = global_clock; // Assumindo uma variável global_clock
        }

        current_task->remaining_burst_time--;
        time_slice_remaining--;

        // Verifica se a tarefa terminou
        if (current_task->remaining_burst_time == 0) {
            current_task->state = TaskState::TERMINATED;
            return true; 
        }

        // Verifica se o quantum expirou
        if (time_slice_remaining == 0) {
            return true; // Preempção!
        }

        return false; // Continua executando
    }

    std::shared_ptr<TaskControlBlock> get_current_task() {
        return current_task;
    }

    bool is_idle() const {
        return current_task == nullptr;
    }
};



#endif // CPU_H

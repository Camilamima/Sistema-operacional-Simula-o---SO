#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <deque>
#include <memory>
#include "TaskControlBlock.h"

class Scheduler {
private:
    // Usamos std::deque como uma fila FIFO (First-In, First-Out)
    std::deque<std::shared_ptr<TaskControlBlock>> ready_queue;

public:
    // Adiciona uma tarefa à fila de prontos
    void add_to_ready_queue(std::shared_ptr<TaskControlBlock> task) {
        task->state = TaskState::READY;
        ready_queue.push_back(task);
    }

    // Pega a próxima tarefa da fila para executar
    std::shared_ptr<TaskControlBlock> get_next_task() {
        if (ready_queue.empty()) {
            return nullptr; // Nenhuma tarefa pronta
        }
        
        std::shared_ptr<TaskControlBlock> next_task = ready_queue.front();
        ready_queue.pop_front();
        return next_task;
    }

    // Verifica se há tarefas prontas
    bool has_ready_tasks() const {
        return !ready_queue.empty();
    }
    
    // Incrementa o tempo de espera de todas as tarefas na fila de prontos
    void update_waiting_times() {
        for (const auto& task : ready_queue) {
            task->waiting_time++;
        }
    }
};

#endif // SCHEDULER_H

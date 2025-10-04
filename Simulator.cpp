#include <iostream>
#include <iomanip>
#include "Simulator.h"

// Definição da variável global do relógio
int global_clock = 0;

Simulator::Simulator(int quantum) : time_quantum(quantum) {}

void Simulator::create_task(int id, int arrival_time, int burst_time) {
    all_tasks.push_back(std::make_shared<TaskControlBlock>(id, arrival_time, burst_time));
}

// Verifica se novas tarefas chegaram no tick atual e as coloca na fila de prontos
void Simulator::admit_new_tasks() {
    for (const auto& task : all_tasks) {
        if (task->arrival_time == global_clock) {
            std::cout << "[Clock " << global_clock << "] Task " << task->id << " arrived and is now READY." << std::endl;
            scheduler.add_to_ready_queue(task);
        }
    }
}

void Simulator::run() {
    std::cout << "--- Simulation Starting ---" << std::endl;
    std::cout << "Time Quantum: " << time_quantum << " ticks." << std::endl << std::endl;

    // Loop principal da simulação
    while (terminated_task_count < all_tasks.size()) {
        // 1. Admitir novas tarefas que chegam neste tick
        admit_new_tasks();

        // 2. Se a CPU está ociosa, escalonar uma nova tarefa
        if (cpu.is_idle()) {
            if (scheduler.has_ready_tasks()) {
                auto next_task = scheduler.get_next_task();
                std::cout << "[Clock " << global_clock << "] CPU is idle. Dispatching Task " << next_task->id << "." << std::endl;
                cpu.dispatch(next_task, time_quantum);
            }
        }

        // 3. Executar um tick na CPU
        print_simulation_tick_details(); // Mostra o estado antes de executar
        
        if (!cpu.is_idle()) {
            bool needs_reschedule = cpu.execute_tick();

            // 4. Lidar com o resultado da execução
            if (needs_reschedule) {
                auto finished_task = cpu.get_current_task();
                
                if (finished_task->state == TaskState::TERMINATED) {
                    finished_task->finish_time = global_clock + 1; // Termina no início do próximo ciclo
                    terminated_task_count++;
                    std::cout << "[Clock " << global_clock << "] Task " << finished_task->id << " TERMINATED." << std::endl;
                } else { // Quantum expirou
                    std::cout << "[Clock " << global_clock << "] Quantum for Task " << finished_task->id << " expired. Moving to READY queue." << std::endl;
                    scheduler.add_to_ready_queue(finished_task);
                }
                
                // CPU fica ociosa, pronta para a próxima tarefa no próximo tick
                cpu.dispatch(nullptr, 0);
            }
        } else {
             std::cout << "[Clock " << global_clock << "] CPU is IDLE." << std::endl;
        }

        // 5. Atualizar métricas e avançar o relógio
        scheduler.update_waiting_times();
        global_clock++;
    }

    std::cout << std::endl << "--- Simulation Finished ---" << std::endl;
    print_final_report();
}

void Simulator::print_simulation_tick_details() {
    if (cpu.is_idle()) return;
    auto task = cpu.get_current_task();
    std::cout << "[Clock " << global_clock << "] Running Task: " << task->id 
              << " (Remaining: " << task->remaining_burst_time << ")" << std::endl;
}

void Simulator::print_final_report() {
    std::cout << std::endl << "------ Final Report ------" << std::endl;
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(10) << "Arrival"
              << std::setw(10) << "Burst"
              << std::setw(10) << "Finish"
              << std::setw(15) << "Turnaround"
              << std::setw(15) << "Waiting Time"
              << std::setw(15) << "Response Time" << std::endl;
    
    float total_turnaround = 0;
    float total_waiting = 0;

    for (const auto& task : all_tasks) {
        std::cout << std::left << std::setw(5) << task->id
                  << std::setw(10) << task->arrival_time
                  << std::setw(10) << task->total_burst_time
                  << std::setw(10) << task->finish_time
                  << std::setw(15) << task->get_turnaround_time()
                  << std::setw(15) << task->waiting_time
                  << std::setw(15) << task->get_response_time() << std::endl;
        total_turnaround += task->get_turnaround_time();
        total_waiting += task->waiting_time;
    }
    
    std::cout << "\nAverage Turnaround Time: " << total_turnaround / all_tasks.size() << std::endl;
    std::cout << "Average Waiting Time: " << total_waiting / all_tasks.size() << std::endl;
}

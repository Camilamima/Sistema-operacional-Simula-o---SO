#ifndef TCB_H
#define TCB_H

// Enum para representar os possíveis estados de uma tarefa
enum class TaskState {
    NEW,        // Tarefa recém-criada, ainda não admitida pelo escalonador
    READY,      // Pronta para executar, aguardando na fila de prontos
    RUNNING,    // Atualmente executando na CPU
    WAITING,    // Aguardando por um evento (ex: E/S), não implementado nesta versão simples
    TERMINATED  // Execução concluída
};

struct TaskControlBlock {
    int id;                 // Identificador único da tarefa
    TaskState state;        // Estado atual da tarefa

    // Características da tarefa (definidas na criação)
    int arrival_time;       // Instante de chegada no sistema (em ticks)
    int total_burst_time;   // Duração total de execução necessária (em ticks)

    // Métricas de simulação (atualizadas durante a execução)
    int remaining_burst_time; // Tempo de CPU que ainda falta
    int start_time = -1;      // Tick em que a tarefa começou a executar pela primeira vez
    int finish_time = -1;     // Tick em que a tarefa terminou
    int waiting_time = 0;     // Tempo total gasto na fila de prontos
    
    // Construtor para facilitar a criação
    TaskControlBlock(int p_id, int p_arrival_time, int p_burst_time)
        : id(p_id),
          state(TaskState::NEW),
          arrival_time(p_arrival_time),
          total_burst_time(p_burst_time),
          remaining_burst_time(p_burst_time) {}

    // Calcula o Turnaround Time: tempo total no sistema
    int get_turnaround_time() const {
        if (finish_time != -1) {
            return finish_time - arrival_time;
        }
        return -1; // Ainda não terminou
    }

    // Calcula o Response Time: tempo da chegada até o início da execução
    int get_response_time() const {
        if (start_time != -1) {
            return start_time - arrival_time;
        }
        return -1; // Ainda não começou a executar
    }
};

#endif // TCB_H

#include "Simulator.h"

int main() {
    // Cria o simulador com um quantum de 4 ticks
    Simulator os_simulator(4);

    // Cria as tarefas para a simulação (id, tempo de chegada, duração)
    // Conforme o requisito 1.4, a simulação deve seguir estes tempos.
    os_simulator.create_task(1, 0, 8);
    os_simulator.create_task(2, 1, 4);
    os_simulator.create_task(3, 2, 9);
    os_simulator.create_task(4, 3, 5);

    // Executa a simulação
    os_simulator.run();

    return 0;
}

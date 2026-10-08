#include <pthread.h>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

struct Process
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
};

struct SchedulerData
{
    std::vector<Process> *processes;
    int quantum;
};

void *round_robin(void *arg)
{
    SchedulerData *data = static_cast<SchedulerData *>(arg);
    std::vector<Process> &processes = *data->processes;
    std::queue<Process *> q;
    std::vector<bool> added(processes.size(), false);
    Process *prev = nullptr, *curr = nullptr;
    int time = 0;
    int used_quantum = 0;
    int finished = 0;
    int context_switches = 0;
    while (finished < processes.size())
    {
        for (int i = 0; i < processes.size(); i++)
        {
            if (!added[i] && processes[i].arrival_time == time)
            {
                q.push(&processes[i]);
                added[i] = true;
            }
        }
        if (curr == nullptr && !q.empty())
        {
            curr = q.front();
            q.pop();
            used_quantum = 0;
            if (prev != nullptr && prev != curr)
                context_switches++;
            prev = curr;
        }
        if (curr == nullptr)
        {
            std::cout << "t=" << time << ": CPU idle\n";
            time++;
            continue;
        }
        std::cout << "t=" << time << ": P" << curr->pid << " runs\n";
        curr->remaining_time--;
        used_quantum++;
        time++;
        for (int i = 0; i < processes.size(); i++)
        {
            if (!added[i] && processes[i].arrival_time == time)
            {
                q.push(&processes[i]);
                added[i] = true;
            }
        }
        if (curr->remaining_time == 0)
        {
            curr->completion_time = time;
            finished++;
            curr = nullptr;
            used_quantum = 0;
            continue;
        }
        if (used_quantum == data->quantum)
        {
            q.push(curr);
            curr = nullptr;
            used_quantum = 0;
        }
    }
    double total_waiting = 0;
    std::cout << "\nPID\tAT\tBT\tCT\tWT\n";
    for (const Process &process : processes)
    {
        int waiting_time = process.completion_time - process.arrival_time - process.burst_time;
        total_waiting += waiting_time;
        std::cout << process.pid << '\t' << process.arrival_time << '\t' << process.burst_time << '\t' << process.completion_time << '\t' << waiting_time << '\n';
    }
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average waiting time: " << total_waiting / processes.size() << '\n';
    std::cout << "Context switches: " << context_switches << '\n';
    return nullptr;
}

int main()
{
    int n;
    std::cout << "Enter number of processes: ";
    std::cin >> n;
    std::vector<Process> processes(n);
    std::cout << "Enter PID, arrival time and burst time:\n";
    for (Process &process : processes)
    {
        std::cin >> process.pid >> process.arrival_time >> process.burst_time;
        process.remaining_time = process.burst_time;
        process.completion_time = 0;
    }
    int quantum;
    std::cout << "Enter time quantum: ";
    std::cin >> quantum;
    SchedulerData data{&processes, quantum};
    pthread_t scheduler_thread;
    pthread_create(&scheduler_thread, nullptr, round_robin, &data);
    pthread_join(scheduler_thread, nullptr);
    return 0;
}
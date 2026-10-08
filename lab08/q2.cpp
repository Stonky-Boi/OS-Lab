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
    int priority;
    int remaining_time;
    int completion_time;
};

struct Compare
{
    bool operator()(const Process *a, const Process *b) const
    {
        if (a->priority != b->priority)
            return a->priority > b->priority;
        if (a->arrival_time != b->arrival_time)
            return a->arrival_time > b->arrival_time;
        return a->pid > b->pid;
    }
};

struct SchedulerData
{
    std::vector<Process> *processes;
};

void *preemptive_priority(void *arg)
{
    SchedulerData *data = static_cast<SchedulerData *>(arg);
    std::vector<Process> &processes = *data->processes;
    std::priority_queue<Process *, std::vector<Process *>, Compare> pq;
    std::vector<bool> added(processes.size(), false);
    Process *prev = nullptr, *curr = nullptr;
    int time = 0;
    int finished = 0;
    int context_switches = 0;
    while (finished < processes.size())
    {
        for (int i = 0; i < processes.size(); i++)
        {
            if (!added[i] && processes[i].arrival_time == time)
            {
                pq.push(&processes[i]);
                added[i] = true;
            }
        }
        if (curr != nullptr && !pq.empty() && pq.top()->priority < curr->priority)
        {
            pq.push(curr);
            curr = nullptr;
        }
        if (curr == nullptr && !pq.empty())
        {
            curr = pq.top();
            pq.pop();
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
        time++;
        if (curr->remaining_time == 0)
        {
            curr->completion_time = time;
            finished++;
            curr = nullptr;
        }
    }
    double total_waiting = 0;
    std::cout << "\nPID\tAT\tBT\tPR\tCT\tWT\n";
    for (const Process &process : processes)
    {
        int waiting_time = process.completion_time - process.arrival_time - process.burst_time;
        total_waiting += waiting_time;
        std::cout << process.pid << '\t' << process.arrival_time << '\t' << process.burst_time << '\t' << process.priority << '\t' << process.completion_time << '\t' << waiting_time << '\n';
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
    std::cout << "Enter PID, arrival time, burst time and priority:\n";
    for (Process &process : processes)
    {
        std::cin >> process.pid >> process.arrival_time >> process.burst_time >> process.priority;
        process.remaining_time = process.burst_time;
        process.completion_time = 0;
    }
    SchedulerData data{&processes};
    pthread_t scheduler_thread;
    pthread_create(&scheduler_thread, nullptr, preemptive_priority, &data);
    pthread_join(scheduler_thread, nullptr);
    return 0;
}
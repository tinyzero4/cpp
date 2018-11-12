#include <iostream>
#include <vector>
#include <map>

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo &GetPersonTasksInfo(const string &person) const {
        return board.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string &person) {
        board[person][TaskStatus::NEW]++;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count) {
        if (task_count == 0)
            return {};

        map<TaskStatus, TaskStatus> nextState = {
                {TaskStatus::NEW,         TaskStatus::IN_PROGRESS},
                {TaskStatus::IN_PROGRESS, TaskStatus::TESTING},
                {TaskStatus::TESTING,     TaskStatus::DONE},
        };

        TasksInfo changed;
        TasksInfo left;
        map<TaskStatus, int> &queue = board[person];

        for (auto &status : {TaskStatus::NEW, TaskStatus::IN_PROGRESS, TaskStatus::TESTING}) {
            if (queue.count(status) && queue.at(status) > 0) {
                if (task_count == 0) {
                    left[status] = queue[status];
                    continue;
                }
                auto res = processTasks(queue, status, task_count);
                task_count -= res.first;
                changed[nextState[status]] = res.first;
                if (res.second) left[status] = res.second;
                queue[status] = res.second;
            }
        }

        for (auto &status : changed) {
            queue[status.first] += status.second;
        }

        return make_tuple(changed, left);
    }

    pair<int, int> processTasks(TasksInfo &tasks, TaskStatus status, int task_count) {
        auto count = static_cast<uint32_t>(tasks[status]);
        uint32_t processed = task_count <= count ? task_count : count;
        uint32_t to_process = count - processed;
        return {processed, to_process};
    }

private:
    map<string, TasksInfo> board;
};
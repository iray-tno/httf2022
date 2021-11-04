#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <array>
#include <queue>
#include <numeric>
#include <map>
#include <set>
#include <memory>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
// #include <sys/time.h>

#ifdef LOCAL
// #define MEASURE_TIME
#define DEBUG
#else
#define NDEBUG
// #define DEBUG
#endif
#include <cassert>

using namespace std;

constexpr bool PRINT_EST = true;
constexpr bool PRINT_DEBUG = false;

/**
 * 逆トポロジカルソートの順番で走査し、クリティカルパスを考慮したコストを計算する
 */
vector<int> calcSumScores(int taskSize, vector<pair<int, int>> &taskRelations, vector<int> &scores) {
    vector<vector<int>> graph(taskSize);
    vector<int> indegree(taskSize, 0);
    for (const auto& r : taskRelations) {
        graph[r.second].push_back(r.first);
        indegree[r.first] += 1;
    }

    vector<int> taskSumScores(graph.size(), 0);

    queue<int> zeroIndegreeVertexes;
    for (int i = 0; i < graph.size(); i++) {
        if (indegree[i] == 0) zeroIndegreeVertexes.push(i);
    }

    while (zeroIndegreeVertexes.empty() == false) {
        int v = zeroIndegreeVertexes.front(); zeroIndegreeVertexes.pop();
        taskSumScores[v] += scores[v];

        for (auto& u : graph[v]) {
            taskSumScores[u] += taskSumScores[v];
            indegree[u] -= 1;
            if (indegree[u] == 0) zeroIndegreeVertexes.push(u);
        }
    }

    return taskSumScores;
}

struct Result {
    int taskId;
    // vector<int> taskRequirements;
    int duration;
};

typedef vector<int> Skillset;
struct Portfolio {
    int memberId;
    vector<Result> results;
    Skillset skillset;

    static int calcDuration(int skills, const vector<int> &task, const Skillset &skillset) {
        int duration = 0;
        for (int i = 0; i < skills; ++i) {
            duration += max(0, task[i] - skillset[i]);
        }
        duration = max(1, duration);
        return duration;
    }

    static int calcDiff(int skills, const vector<vector<int>> &tasks, const Skillset &skillset, const vector<Result> &results) {
        int diff = 0;
        for (auto& r : results) {
            int estDuration = calcDuration(skills, tasks[r.taskId], skillset);
            diff += abs(estDuration - r.duration);
        }
        return diff;
    }

    void updateSkillset(int skills, vector<vector<int>> &tasks) {
        Skillset current = skillset;
        int currentDiff = calcDiff(skills, tasks, current, results);

        int loop = 0;
        while (loop < 1000) {
            bool updated = false;
            for (int i = 0; i < skills; ++i) {
                for (int j = 0; j < 2; ++j) {
                    bool positive = j == 0;
                    if (current[i] == 0 && !positive) continue;

                    current[i] += positive ? +1 : -1;
                    int nextDiff = calcDiff(skills, tasks, current, results);
                    if (nextDiff < currentDiff) { // 採用
                        currentDiff = nextDiff;
                        updated = true;
                        goto CONTINUE;
                    }

                    current[i] -= positive ? +1 : -1; // 不採用なので元に戻す
                }
            }
            CONTINUE:
            if (!updated) break;
            loop++;
        }
        skillset = current;

        printSkillset();
        return;
    }

    void printSkillset() {
        if (!PRINT_EST) return;
        if (memberId == 0) return;
        cout << "#s " << memberId;
        for (auto& s : skillset) cout << " " << s;
        cout << endl;
    }
};

vector<Portfolio> initPortfolios(int members, int skills) {
    vector<Portfolio> portfolios(members + 1);

    for (int i = 0; i < portfolios.size(); ++i) {
        portfolios[i] = { i, vector<Result>(0), Skillset(skills, 5) };
    }

    for (auto& p : portfolios) p.printSkillset();
    return portfolios;
}

struct Assignment {
    int taskId;
    int memberId;
    int startAt;
    int estFinishAt;
};

int main() {
    int n, m, k, r;

    cin >> n >> m >> k >> r;

    vector<vector<int>> tasks(n + 1, vector<int>(k, 0));
    vector<int> taskScores(n + 1, 0);
    vector<pair<int, int>> taskRelations(r, pair<int, int>());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            cin >> tasks[i + 1][j];
            taskScores[i + 1] += tasks[i + 1][j];
        }
    }

    for (int i = 0; i < r; ++i) cin >> taskRelations[i].first >> taskRelations[i].second;

    // 隣接リスト
    vector<vector<int>> graph(tasks.size());
    vector<int> indegree(tasks.size(), 0);
    for (const auto& r : taskRelations) {
        int first = r.first, second = r.second;
        graph[first].push_back(second);
        indegree[second] += 1;
    }

    vector<int> taskSumScores = calcSumScores(tasks.size(), taskRelations, taskScores);

    if (PRINT_DEBUG) {
        for (int i = 0; i < tasks.size(); ++i) {
            cerr << i << ": " << taskScores[i] << " " << taskSumScores[i] << endl;
        }
    }

    priority_queue<pair<int, int>> readyTasks;
    for (int taskId = 1; taskId < indegree.size(); taskId++) {
        if (indegree[taskId] == 0) readyTasks.push(make_pair(taskSumScores[taskId], taskId));
    }

    vector<Portfolio> portfolios = initPortfolios(m, k);

    int startedTasks = 0, finishedTasks = 0;
    set<int> availableMembers;
    for (int i = 0; i < m; ++i) availableMembers.insert(i + 1);

    map<int, Assignment> assignedTaskMap;
    int day = 0;
    while (finishedTasks < tasks.size()) {
        if (availableMembers.size() <= readyTasks.size()) {
            // メンバーのほうが少ない場合、メンバーごとに最大効率のタスクを選んでアサインする
            cout << availableMembers.size();

            set<int> taskChoices;
            int maxChoices = min(max((int)availableMembers.size() * 2, 2), (int)readyTasks.size());
            for (int i = 0; i < maxChoices; ++i) {
                int taskId = readyTasks.top().second; readyTasks.pop();
                taskChoices.insert(taskId);
            }
            for (auto& memberId : availableMembers) {
                Skillset skillset = portfolios[memberId].skillset;
                int maxFlow = accumulate(skillset.begin(), skillset.end(), 0);

                int bestTaskId = -1;
                int bestEffectiveness = -1;
                int bestTaskPriority = -1;
                for (auto& taskId : taskChoices) {
                    int estDuration = Portfolio::calcDuration(k, tasks[taskId], skillset);
                    int effectiveness = - estDuration;
                    // int effectiveness = - estDuration * 1000 + taskSumScores[taskId];
                    // int effectiveness = taskScores[taskId] - estDuration * maxFlow;
                    int priority = taskSumScores[taskId];

                    if (bestTaskId == -1 || bestEffectiveness < effectiveness || bestEffectiveness == effectiveness && bestTaskPriority < priority) {
                        bestTaskId = taskId;
                        bestEffectiveness = effectiveness;
                        bestTaskPriority = priority;
                    }
                }
                int taskId = bestTaskId;
                int estDuration = Portfolio::calcDuration(k, tasks[taskId], portfolios[memberId].skillset);

                assignedTaskMap[memberId] = { taskId, memberId, day, day + estDuration};
                
                taskChoices.erase(taskId);

                startedTasks++;
                cout << " " << memberId << " " << taskId;
                if (PRINT_DEBUG) {
                    cerr << "!assigned: " << taskId << " to " << memberId << " score " << taskSumScores[taskId] << endl;
                }
            }
            cout << endl;
            
            for (auto& taskId : taskChoices) {
                readyTasks.push(make_pair(taskSumScores[taskId], taskId));
            }
            availableMembers.clear(); // メンバーごとにループしてアサインしたのですべて使われる
        } else {
            // タスクのほうが少ない場合、タスクごとにごとに早く終わるものを
            cout << readyTasks.size();
            int taskSize = readyTasks.size();
            for (int i = 0; i < taskSize; ++i) {
                int taskId = readyTasks.top().second; readyTasks.pop();
                int bestMemberId = -1;
                int bestDuration = -1;
                for (auto& memberId : availableMembers) {
                    Portfolio portfolio= portfolios[memberId];
                    int estDuration = Portfolio::calcDuration(k, tasks[taskId], portfolio.skillset);

                    if (bestMemberId == -1 || estDuration < bestDuration) {
                        bestMemberId = memberId;
                        bestDuration = estDuration;
                    }
                }

                int memberId = bestMemberId;
                availableMembers.erase(memberId);

                int estDuration = Portfolio::calcDuration(k, tasks[taskId], portfolios[memberId].skillset);
                
                assignedTaskMap[memberId] = { taskId, memberId, day, day + estDuration};
                startedTasks++;
                cout << " " << memberId << " " << taskId;
                if (PRINT_DEBUG) {
                    cerr << "#assigned: " << taskId << " to " << memberId << " score " << taskSumScores[taskId] << endl;
                }
            }
            cout << endl;
            // readyTasks.clear();
        }

        day++;

        int finishedMembers;
        cin >> finishedMembers;
        if (finishedMembers < 0) break;

        for (int i = 0; i < finishedMembers; ++i) {
            int finishedMemberId;
            cin >> finishedMemberId;
            int finishedTaskId = assignedTaskMap[finishedMemberId].taskId;
            int duration = day - assignedTaskMap[finishedMemberId].startAt;
            int estDuration = assignedTaskMap[finishedMemberId].estFinishAt - assignedTaskMap[finishedMemberId].startAt;

            if (PRINT_DEBUG) {
                cerr << finishedMemberId << " finished task " << finishedTaskId << " by " << duration << " estimated as " << estDuration << endl;
            }
            portfolios[finishedMemberId].results.push_back({ finishedTaskId, duration });
            portfolios[finishedMemberId].updateSkillset(k, tasks);

            availableMembers.insert(finishedMemberId);
            for (auto& taskId : graph[finishedTaskId]) {
                indegree[taskId] -= 1;
                if (indegree[taskId] == 0) readyTasks.push(make_pair(taskSumScores[taskId], taskId));
            }
            finishedTasks++;
        }
    }

    return 0;
}

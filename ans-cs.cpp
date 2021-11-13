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



using namespace std;

constexpr bool PRINT_EST = false;
constexpr bool PRINT_DEBUG = false;

unsigned int randxor() {
    static unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned int t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

int randInt(int from, int to) {
    return randxor() % (to - from) + from;
}

/**
 * 逆トポロジカルソートの順番で走査し、クリティカルパスを考慮したコストを計算する
 */
vector<int> calcSumDists(int taskSize, vector<pair<int, int>> &taskRelations) {
    vector<vector<int>> graph(taskSize);
    vector<int> indegree(taskSize, 0);
    for (const auto& r : taskRelations) {
        graph[r.second].push_back(r.first);
        indegree[r.first] += 1;
    }

    vector<int> taskSumDists(graph.size(), 1);

    queue<int> zeroIndegreeVertexes;
    for (int i = 0; i < graph.size(); i++) {
        if (indegree[i] == 0) zeroIndegreeVertexes.push(i);
    }

    while (zeroIndegreeVertexes.empty() == false) {
        int v = zeroIndegreeVertexes.front(); zeroIndegreeVertexes.pop();

        for (auto& u : graph[v]) {
            taskSumDists[u] = max(taskSumDists[u], taskSumDists[v] + 1);
            indegree[u] -= 1;
            if (indegree[u] == 0) zeroIndegreeVertexes.push(u);
        }
    }

    return taskSumDists;
}

typedef vector<int> Skillset;

struct Result {
    int taskId;
    // vector<int> taskRequirements;
    int duration;
};

struct Portfolio {
    int memberId;
    vector<Result> results;
    Skillset skillset;

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
    private:
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
};

class Portfolios {
    public:
    int members;
    int skills;
    vector<Portfolio> portfolios;
    vector<vector<int>> estimatedDurations;
    vector<vector<int>> tasks;

    Portfolios(int members_, int skills_, vector<vector<int>> _tasks) {
        members = members_;
        skills = skills_;
        portfolios = vector<Portfolio>(members + 1);

        for (int i = 0; i < portfolios.size(); ++i) {
            portfolios[i] = { i, vector<Result>(0), Skillset(skills, 5) };
        }
        for (auto& p : portfolios) p.printSkillset();

        tasks = _tasks;
        estimatedDurations = vector<vector<int>>(tasks.size(), vector<int>(members + 1, 0));

        for (int i = 0; i < members; ++i) {
            updateMembersEstimation(i + 1);
        }
    }

    void updatePortfolioAndEstimation(Assignment assignment, int day) {
        updatePortfolioAndEstimation(assignment.memberid, assignment.taskId, day - aasignment.startAt)
    }
    void updatePortfolioAndEstimation(int memberId, int finishedTaskId, int duration) {
        portfolios[memberId].results.push_back({ finishedTaskId, duration });
        portfolios[memberId].updateSkillset(skills, tasks);
        updateMembersEstimation(memberId);
    }

    void updateMembersEstimation(int memberId) {
        if (memberId == 0) return;
        for (int i = 1; i < tasks.size(); ++i) {
            estimatedDurations[i][memberId] = calcDuration(tasks[i], portfolios[memberId].skillset);
        }
    }

    int calcDuration(const vector<int> &task, const Skillset &skillset) {
        int duration = 0;
        for (int i = 0; i < skills; ++i) {
            duration += max(0, task[i] - skillset[i]);
        }
        duration = max(1, duration);
        return duration;
    }

    Skillset getSkillset(int memberId) { return portfolios[memberId].skillset; }
    Portfolio getPortfolio(int memberId) { return portfolios[memberId]; }
    int getEstimation(int memberId, int taskId) {
        return estimatedDurations[taskId][memberId];
    }
};

struct Assignment {
    int taskId;
    int memberId;
    int startAt;
    int estFinishAt;
};

template<typename S>
auto select_random(const S &s, size_t n) {
  auto it = std::begin(s);
  std::advance(it,n);
  return it;
}

constexpr int MAX_TURN = 100;

struct State {
    int currentDay;
    set<int> availableMembers;
    vector<int> indegrees;
    set<int> readyTasks;
    long long score;
};

State chokudai_search(State firstState) {
    vector<priority_queue<State>> states(MAX_TURN, priority_queue<int>());
    for (int i = 0; i <= MAX_TURN, ++i) states[i] = priority_queue<State>();
    states[0].push(firstState);
    int width = 1;
    while (TimeCheck()) {
        for (int t = 0; t < MAX_TURN; ++t) {
            for (int i = 0; i < width; ++i) {
                if (state.front() == null) break;
                State current = states[t].front(); states[t].pop();
                for (auto& nextState : current.genNextState()) {
                    states[t + 1].push(nextState)
                }
            }
        }
    }
    State bestState = states[MAX_TURN].pop();
    return bestState;
}


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

    vector<long long> taskSumCosts = calcSumCosts(tasks.size(), taskRelations, taskScores);
    vector<int> taskDists = calcSumDists(tasks.size(), taskRelations);

    set<int> readyTasks;
    vector<set<int>> taskQueues(m + 1);
    for (int taskId = 1; taskId < indegree.size(); taskId++) {
        if (indegree[taskId] == 0) {
            int pickedMemberId = randInt(1, m + 1);
            readyTasks.insert(taskId);
            taskQueues[pickedMemberId].insert(taskId);
        }
    }

    Portfolios portfolios(m, k, tasks);

    int startedTasks = 0, finishedTasks = 0;
    set<int> availableMembers;
    for (int i = 0; i < m; ++i) availableMembers.insert(i + 1);

    map<int, Assignment> assignedTaskMap;

    int day = 0;
    while (finishedTasks < tasks.size()) {
        int finishedMembers;
        cin >> finishedMembers;

        if (finishedMembers < 0) break; // -1で終了

        for (int i = 0; i < finishedMembers; ++i) {
            int finishedMemberId;
            cin >> finishedMemberId;
            portfolios.updatePortfolioAndEstimation(assignedTaskMap[finishedMemberId], day);

        }

        // タスクキューを整理
        vector<int> sumDurations(m+1, 0);
        vector<int> sumDists(m+1, 0);
        int sumAllDists = 0;
        for (int memberId = 1; memberId < m+1; ++memberId) {
            if (0 < assignedTaskMap.count(memberId)) {
                int startTaskAt = assignedTaskMap[memberId].startAt;
                int taskId = assignedTaskMap[memberId].taskId;
                int estimatedDays = portfolios.getEstimation(memberId, taskId);
                sumDurations[memberId] = max(1, startTaskAt + estimatedDays - day);
            }
            for (auto& taskId : taskQueues[memberId]) {
                sumDurations[memberId] += portfolios.getEstimation(memberId, taskId);
                sumDists[memberId] += taskDists[taskId];
                sumAllDists += taskDists[taskId];
            }
        }
        int averageSumDist = sumAllDists / m;
        
        int COEF_A = 100000;
        int COEF_B = 1;
        int COEF_C = 1000;
        int distsScore = 0;
        int loop = 0;
        int LOOP_MAX = 10000;
        int R = 1000000;
        for (int memberId = 1; memberId < m+1; ++memberId) {
            distsScore += abs(averageSumDist - sumDists[memberId]);
        }
        cerr << "sumAllDists" << sumAllDists << " averageSumDist " << averageSumDist << " distsScore" << distsScore << endl;
        int score = *max_element(sumDurations.begin(), sumDurations.end()) * COEF_A
             + accumulate(sumDurations.begin(), sumDurations.end(), 0) * COEF_B
             + distsScore * COEF_C;
        int beforeMaxDuration = *max_element(sumDurations.begin(), sumDurations.end());

    
        cerr << "before distsScore: " << distsScore << endl;
        if (PRINT_DEBUG) {
            cerr << "before score: " << score << " min:" << beforeMaxDuration << ", sum: " << accumulate(sumDurations.begin(), sumDurations.end(), 0) << ", seq:";
            for (auto& dur : sumDurations) cerr << " " << dur;
            cerr << endl;
        }

        while(loop < LOOP_MAX) {
            loop++;

            // int pickFromMemberId = randInt(1, m + 1);
            int pickFromMemberId = -1;
            int largestDuration = -1;
            for (int i = 1; i < m+1; ++i) {
                int duration = sumDurations[i];
                if (pickFromMemberId == -1 || largestDuration < duration) {
                    largestDuration = duration;
                    pickFromMemberId = i;
                }
            }
            int pickToMemberId = randInt(1, m + 1);
            if (taskQueues[pickFromMemberId].size() == 0) continue;
            int pickedTaskIndex = randInt(0, taskQueues[pickFromMemberId].size());

            int pickedTaskId = *select_random(taskQueues[pickFromMemberId], pickedTaskIndex);

            sumDurations[pickFromMemberId] -= portfolios.getEstimation(pickFromMemberId, pickedTaskId);
            sumDurations[pickToMemberId] += portfolios.getEstimation(pickToMemberId, pickedTaskId);
        
            sumDists[pickFromMemberId] -= taskDists[pickedTaskId];
            sumDists[pickToMemberId] += taskDists[pickedTaskId];

            int nextDistsScore = 0;
            for (int memberId = 1; memberId < m+1; ++memberId) {
                nextDistsScore += abs(averageSumDist - sumDists[memberId]);
            }
            int nextScore = *max_element(sumDurations.begin(), sumDurations.end()) * COEF_A
                 + accumulate(sumDurations.begin(), sumDurations.end(), 0) * COEF_B
                 + nextDistsScore * COEF_C;

            bool apply = score >= nextScore;

            if (!apply) {
                apply = R*(LOOP_MAX-loop)>LOOP_MAX*(randxor()%R);
            }
            if (apply) {
                // 確定
                taskQueues[pickFromMemberId].erase(pickedTaskId);
                taskQueues[pickToMemberId].insert(pickedTaskId);
                score = nextScore;
                distsScore = nextDistsScore;
            } else {
                // UNDO
                sumDurations[pickFromMemberId] += portfolios.getEstimation(pickFromMemberId, pickedTaskId);
                sumDurations[pickToMemberId] -= portfolios.getEstimation(pickToMemberId, pickedTaskId);

                sumDists[pickFromMemberId] += taskDists[pickedTaskId];
                sumDists[pickToMemberId] -= taskDists[pickedTaskId];
            }
        }
            
        int afterMaxDuration = *max_element(sumDurations.begin(), sumDurations.end());
        cerr << "after distsScore: " << distsScore << endl;
        if (PRINT_DEBUG) {
            cerr << "after score: " << score << " min:" << afterMaxDuration << ", sum: " << accumulate(sumDurations.begin(), sumDurations.end(), 0) << ", seq:";
            for (auto& dur : sumDurations) cerr << " " << dur;
            cerr << endl;
        }
    }

    return 0;
}

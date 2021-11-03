#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <array>
#include <queue>
#include <numeric>
#include <map>
#include <memory>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/time.h>

#ifdef LOCAL
// #define MEASURE_TIME
#define DEBUG
#else
#define NDEBUG
// #define DEBUG
#endif
#include <cassert>

using namespace std;

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

    // for (int i = 0; i < tasks.size(); ++i) {
    //     cerr << i << ": " << taskScores[i] << " " << taskSumScores[i] << endl;
    // }

    priority_queue<pair<int, int>> readyTasks;
    for (int taskId = 1; taskId < indegree.size(); taskId++) {
        if (indegree[taskId] == 0) readyTasks.push(make_pair(taskSumScores[taskId], taskId));
    }

    int startedTasks = 0, finishedTasks = 0;
    queue<int> availableMembers;
    for (int i = 0; i < m; ++i) availableMembers.push(i + 1);

    map<int, int> assignedTaskMap;
    while (finishedTasks < tasks.size()) {
        int startingTasks = min(readyTasks.size(), availableMembers.size());

        cout << startingTasks;
        for (int i = 0; i < startingTasks; ++i) {
            int memberId = availableMembers.front(); availableMembers.pop();
            int taskId = readyTasks.top().second; readyTasks.pop();

            assignedTaskMap[memberId] = taskId;
            startedTasks++;
            cout << " " << memberId << " " << taskId;
            // cerr << "assigned: " << taskId << " to " << memberId << " score " << taskSumScores[taskId] << endl;
        }
        cout << endl;

        int finishedMembers;
        cin >> finishedMembers;
        if (finishedMembers < 0) break;

        for (int i = 0; i < finishedMembers; ++i) {
            int finishedMemberId;
            cin >> finishedMemberId;
            int finishedTaskId = assignedTaskMap[finishedMemberId];

            availableMembers.push(finishedMemberId);
            for (auto& taskId : graph[finishedTaskId]) {
                indegree[taskId] -= 1;
                if (indegree[taskId] == 0) readyTasks.push(make_pair(taskSumScores[taskId], taskId));
            }
            finishedTasks++;
        }
    }

    return 0;
}

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
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

const int NN = 1110;
std::map<int, int> was;
int num[NN];
std::vector<long double> g[NN];
int main() {
    int N;
    scanf("%d", &N);
    int m = 0;
    for (int i = 0; i < N; i++) {
        int ts, id;
        long double v, ac, an, ner, mch;
        std::cin >> ts >> id >> v >> ac >> an >> ner >> mch;
        if (!was.count(id)) {
            was[id] = ++m;
            num[m] = id;
        }
        g[was[id]].push_back(ner);
    }
    std::vector<std::pair<long double, int>> ans;
    for (int i = 1; i <= m; i++) {
        if (g[i].size() == 1)continue;
        long double mn = g[i][0], mx = g[i][1];
        for (int j = 1; j < g[i].size(); j++) {
            if(mn > g[i][j]) mn = g[i][j];
            if(mx < g[i][j]) mx = g[i][j];
        }
        ans.push_back({mx - mn, i});
    }
    if (ans.empty()) {
        printf("-1");
        return 0;
    }
    sort(ans.begin(), ans.end());
    std::vector<int> fin;
    for (int i = 0, s = std::min((int) ans.size(), 3); i < s; i++) {
        fin.push_back(num[ans[i].second]);
    }
    sort(fin.begin(), fin.end());
    for (int i : fin) {
        printf("%d ", i);
    }
}
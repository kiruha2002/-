#include <iostream>
#include <map>
#include <algorithm>

const int SIZE = 1e6 + 100;
std::string s[SIZE];
bool difference(const std::string &x, const std::string &y) {
    if (x.substr(6, 4) < y.substr(6, 4))return true;
    if (x.substr(6, 4) > y.substr(6, 4))return false;
    if (x.substr(3, 2) < y.substr(3, 2))return true;
    if (x.substr(3, 2) > y.substr(3, 2))return false;
    return !(x.substr(0, 2) > y.substr(0, 2));
}
int main() {
    std::map<std::string, int> values;
    int N;
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        std::cin >> s[i];
        int input;
        std::cin >> input;
        if (values[s[i]] < input) values[s[i]] = input;
    }
    std::sort(s + 1, s + N + 1, difference);
    for (int i = 2; i <= N; i++) {
        values[s[i]] = std::max(values[s[i]], values[s[i - 1]]);
    }
    int M;
    scanf("%d", &M);
    for (int i = 0; i < M; i++) {
        std::string x;
        std::cin >> x;
        int l = 1, r = N, ans = 0;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (difference(s[mid], x)) {
                ans = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        if (!ans) {
            printf("0\n");
        } else {
            printf("%d\n", values[s[ans]]);;
        }
    }
}
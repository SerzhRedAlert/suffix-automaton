#include <bits/stdc++.h>
using namespace std;

struct node {
    array<int, 26> nx;
    int suf, pre;

    node() {
        nx.fill(-1);
        suf = -1, pre = -1;
    }
};

vector<node> s = {node()};

int add(int a, const int x) {
    const int b = int(s.size());
    s.emplace_back();
    s[b].pre = a, s[b].suf = 0;
    for (; a != -1; a = s[a].suf) {
        if (s[a].nx[x] == -1) {
            s[a].nx[x] = b;
            continue;
        }
        int c = s[a].nx[x];
        if (s[c].pre == a) {
            s[b].suf = c;
            break;
        }
        const int d = int(s.size());
        s.emplace_back();
        s[d].nx = s[c].nx;
        s[d].pre = a, s[d].suf = s[c].suf;
        s[b].suf = d, s[c].suf = d;
        for (; a != -1 && s[a].nx[x] == c; a = s[a].suf) {
            s[a].nx[x] = d;
        }
        break;
    }
    return b;
}

void solve() {
    int last = 0;
    char type;
    while (cin >> type) {
        if (type == '?') {
            string cur;
            cin >> cur;
            int v = 0;
            for (const char i : cur) {
                const int c = (i > 'Z' ? i - 'a' : i - 'A');
                v = s[v].nx[c];
                if (v == -1) break;
            }
            cout << (v == -1 ? "NO\n" : "YES\n");
        } else {
            string cur;
            cin >> cur;
            for (const char i : cur) {
                const int c = (i > 'Z' ? i - 'a' : i - 'A');
                last = add(last, c);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const int alpha = 26;
int last = 0;

struct node {
    array<int, alpha> nx;
    int link, len, first_pos;
    bool is_clone;

    node() {
        nx.fill(-1);
        link = -1, len = 0, first_pos = -1, is_clone = false;
    }
};

vector<node> s = {node()};
vector<vector<int>> tree = {{}};

int make_v() {
    s.emplace_back();
    tree.emplace_back();
    return int(s.size()) - 1;
}

void add(int pos, int c) {
    int cur = make_v();
    s[cur].len = s[last].len + 1;
    s[cur].first_pos = pos;
    int p = last;
    while (p != -1 && s[p].nx[c] == -1) {
        s[p].nx[c] = cur;
        p = s[p].link;
    }
    if (p == -1) {
        s[cur].link = 0;
    } else {
        int q = s[p].nx[c];
        if (s[p].len + 1 == s[q].len) {
            s[cur].link = q;
        } else {
            int ns = make_v();
            s[ns] = s[q], s[ns].len = s[p].len + 1;
            s[ns].is_clone = true;
            while (p != -1 && s[p].nx[c] == q) {
                s[p].nx[c] = ns;
                p = s[p].link;
            }
            s[q].link = s[cur].link = ns;
        }
    }
    last = cur;
}

void build() {
    for (int i = 1; i < int(s.size()); ++i) {
        tree[s[i].link].emplace_back(i);
    }
}

int walk(const string &p) {
    int v = 0;
    for (auto ch : p) {
        int c = ch - 'a';
        if (c < 0 || c >= alpha) {
            return -1;
        }
        v = s[v].nx[c];
        if (v == -1) {
            return -1;
        }
    }
    return v;
}

vector<int> find(const string &p) {
    int v = walk(p);
    if (v == -1) return {};
    vector<int> res;
    stack<int> st;
    st.emplace(v);
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        if (!s[u].is_clone) {
            int start = s[u].first_pos - int(p.size()) + 1;
            res.emplace_back(start);
        }
        for (auto to : tree[u]) {
            st.emplace(to);
        }
    }
    sort(res.begin(), res.end());
    return res;
}

void solve() {
    string text;
    cin >> text;
    s.reserve(2 * text.size() + 1), tree.reserve(2 * text.size() + 1);
    for (int i = 0; i < int(text.size()); ++i) {
        add(i, text[i] - 'a');
    }
    build();
    int q;
    cin >> q;
    while (q--) {
        string cur;
        cin >> cur;
        auto res = find(cur);
        cout << res.size() << " ";
        for (auto i : res) cout << i + 1 << " ";
        cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}

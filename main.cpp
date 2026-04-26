#include <bits/stdc++.h>
using namespace std;

vector<int> Pi_f(const string &s) {
    int n = int(s.size());
    vector<int> Pi(n);
    for (int i = 1; i < n; ++i) {
        int j = Pi[i - 1];
        while (s[i] != s[j] && j != 0) {
            j = Pi[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        Pi[i] = j;
    }
    return Pi;
}

void prefix_function() {
    string s, t;
    cin >> t;
    int q;
    cin >> q;
    while (q--) {
        cin >> s;
        string s1 = s + '#' + t;
        auto Pi = Pi_f(s1);
        vector<int> ans;
        for (int i = 0; i < int(t.size()); ++i) {
            if (Pi[s.size() + i + 1] == int(s.size())) {
                ans.emplace_back(i - int(s.size()) + 2);
            }
        }
        cout << ans.size() << " ";
        for (auto i : ans) cout << i << " ";
        cout << "\n";
    }
}

struct Aho {
    struct node {
        int suf, sup, p;
        vector<int> ind, go;
        char c;
        bool t;

        node() {
            suf = -1, sup = -1, p = -1;
            go.resize(26, -1);
            c = 0, t = false;
        }
    };

    vector<node> t;
    int sz = 0;

    Aho() {
        t = {node()};
    }

    void insert(const string &s) {
        int v = 0;
        for (auto c : s) {
            int i = c - 'a';
            if (t[v].go[i] == -1) {
                t[v].go[i] = int(t.size());
                t.emplace_back();
                t.back().p = v;
                t.back().c = c;
            }
            v = t[v].go[i];
        }
        t[v].t = true;
        t[v].ind.emplace_back(sz++);
    }

    int suf(int v) {
        if (!v || !t[v].p) {
            t[v].suf = 0;
        } else if (t[v].suf == -1) {
            t[v].suf = go(suf(t[v].p), t[v].c);
        }
        return t[v].suf;
    }

    int go(int v, char c) {
        int i = c - 'a';
        if (t[v].go[i] == -1) {
            if (!v) {
                t[v].go[i] = 0;
            }
            t[v].go[i] = go(suf(v), c);
        }
        return t[v].go[i];
    }

    int sup(int v) {
        if (!v || t[v].t) {
            t[v].sup = v;
        } else if (t[v].sup == -1) {
            t[v].sup = sup(suf(v));
        }
        return t[v].sup;
    }
};

void aho_korasik() {
    Aho k;
    string t;
    int n;
    cin >> t >> n;
    vector<vector<int> > ans(n);
    vector<int> sz(n);
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        sz[i] = int(s.size());
        k.insert(s);
    }
    int v = 0;
    for (int i = 0; i < int(t.size()); ++i) {
        v = k.go(v, t[i]);
        int u = k.sup(v);
        while (u) {
            for (auto j : k.t[u].ind) {
                ans[j].emplace_back(i);
            }
            u = k.sup(k.suf(u));
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << ans[i].size() << " ";
        for (auto j : ans[i]) {
            cout << j - sz[i] + 2 << " ";
        }
        cout << "\n";
    }
}

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

void suff_auto() {
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
    //Выберите метод решения
    //A) Префикс функция
    //B) Ахо-Корасик
    //C) Суффиксный автомат
    char c;
    cin >> c;
    if (c == 'A') {
        prefix_function();
    }
    if (c == 'B') {
        aho_korasik();
    }
    if (c == 'C') {
        suff_auto();
    }
    return 0;
}

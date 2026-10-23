
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static int solve(const string& a,
                 const string& b,
                 vector<vector<int>>& memo,
                 int i,
                 int j)
{
    if (i == 0 || j == 0) return 0;

    int& res = memo[i][j];
    if (res != -1) return res;          // قبلاً محاسبه شده

    if (a[i - 1] == b[j - 1])
        res = 1 + solve(a, b, memo, i - 1, j - 1);
    else
        res = max(solve(a, b, memo, i - 1, j),
                   solve(a, b, memo, i,     j - 1));
    return res;
}

static string reconstruct(const string& a,
                          const string& b,
                          const vector<vector<int>>& memo)
{
    string out;
    size_t i = a.size(), j = b.size();

    while (i && j)
    {
        if (a[i - 1] == b[j - 1]) {      // حرف مشترک
            out.push_back(a[i - 1]);
            --i; --j;
        }
        else if (memo[i - 1][j] >= memo[i][j - 1])
            --i;                         // حرکت به بالا
        else
            --j;                         // حرکت به چپ
    }
    reverse(out.begin(), out.end());
    return out;
}

int main()
{


    string A, B;
    cin >> A >> B;

    vector<vector<int>> memo(A.size() + 1,vector<int>(B.size() + 1, -1));

    int length = solve(A, B, memo, static_cast<int>(A.size()),
                                   static_cast<int>(B.size()));
    string lcs = reconstruct(A, B, memo);

    cout << lcs;
    return 0;
}

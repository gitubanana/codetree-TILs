#include <iostream>
#include <unordered_map>
#define NUM first
#define CNT second

using t_umap = std::unordered_map<int, int>;

const int MAX_SIZE = 1000;

t_umap elemCnt;
int arr[MAX_SIZE];

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int size, goal;

    std::cin >> size >> goal;
    for (int i = 0; i < size; ++i)
    {
        int &cur = arr[i];

        std::cin >> cur;
        ++elemCnt[cur];
    }

    int ans = 0;
    for (int i = 0; i < size; ++i)
    {
        const int &a = arr[i];

        --elemCnt[a];
        for (int j = 0; j < i; ++j)
        {
            const int &b = arr[j];
            int diff = goal - (a + b);

            ans += elemCnt[diff];
        }
    }

    std::cout << ans << '\n';
    return 0;
}
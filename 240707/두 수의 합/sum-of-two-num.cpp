#include <iostream>
#include <map>
#define NUM first
#define CNT second

using t_map = std::map<int, int>;

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int size, goal;
    t_map map;

    std::cin >> size >> goal;
    while (size--)
    {
        int elem;

        std::cin >> elem;
        ++map[elem];
    }

    int ans = 0;
    t_map::iterator left = map.begin();
    t_map::iterator right = map.end();
    --right;

    if (map.size() == 1)
    {
        ans = left->CNT * (left->CNT) / 2;
    }

    while (left != right)
    {
        int sum = left->NUM + right->NUM;

        if (sum == goal)
        {
            ans += left->CNT * right->CNT;
            ++left;
        }
        else if (sum > goal)
        {
            --right;
        }
        else
        {
            ++left;
        }
    }

    std::cout << ans << '\n';
    return 0;
}
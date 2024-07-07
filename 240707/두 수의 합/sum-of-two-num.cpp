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


    const int &halfGoalCnt = map[goal / 2];
    int ans = halfGoalCnt * (halfGoalCnt - 1) / 2;
    t_map::iterator left = map.begin();
    t_map::iterator right = map.end();
    --right;

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
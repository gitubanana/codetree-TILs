#include <iostream>
#include <vector>

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int size;
    std::vector<int> toCheck;

    std::cin >> size;
    while (size--)
    {
        int checkPeople;

        std::cin >> checkPeople;
        toCheck.push_back(checkPeople);
    }

    int boss, employee;
    int64_t ans = toCheck.size();

    std::cin >> boss >> employee;
    for (const int &cur : toCheck)
    {
        int withoutBoss = cur - boss;

        if (withoutBoss > 0)
        {
            ans += withoutBoss / employee
                    + (withoutBoss % employee != 0);
        }
    }

    std::cout << ans << '\n';
    return 0;
}
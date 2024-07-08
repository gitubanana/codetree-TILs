#include <iostream>

const int MAX_SIZE = 1'000'000;

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int size;
    int boss, employee;
    int toCheck[MAX_SIZE];

    std::cin >> size;
    for (int i = 0; i < size; ++i)
    {
        std::cin >> toCheck[MAX_SIZE];
    }

    int64_t ans = 0;
    std::cin >> boss >> employee;
    for (int i = 0; i < size; ++i)
    {
        int &cur = toCheck[i];
        int withoutBoss = cur - boss;

        ++ans;
        if (withoutBoss > 0)
        {
            ans += withoutBoss / employee
                    + (withoutBoss % employee != 0);
        }
    }

    std::cout << ans << '\n';
    return 0;
}
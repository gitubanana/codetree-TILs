#include <iostream>
#include <vector>

const int MAX_SIZE = 1'000'000;

int toCheck[MAX_SIZE];

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int size;

    std::cin >> size;
    for (int i = 0; i < size; ++i)
    {
        std::cin >> toCheck[i];
    }

    int boss, employee;
    int64_t ans = size;

    std::cin >> boss >> employee;
    for (int i = 0; i < size; ++i)
    {
        int &cur = toCheck[i];
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
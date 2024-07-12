#include <iostream>
#include <algorithm>

struct t_work
{
    int day;
    int pay;
};

const int MAX_SIZE = 15;

int limitDay;
int dp[MAX_SIZE + 1];
t_work works[MAX_SIZE];

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    std::cin >> limitDay;
    for (int i = 0; i < limitDay; ++i)
    {
        std::cin >> works[i].day >> works[i].pay;
    }

    int maxPrev = 0;
    for (int curDay = 0; curDay < limitDay; ++curDay)
    {
        t_work &curWork = works[curDay];
        int nextDay = curDay + curWork.day;

        maxPrev = std::max(
            maxPrev,
            dp[curDay]
        );

        if (nextDay > limitDay)
            continue ;

        dp[nextDay] = std::max(
            dp[nextDay],
            maxPrev + curWork.pay
        );
    }

    std::cout << *std::max_element(dp, dp + limitDay + 1) << '\n';
    return 0;
}
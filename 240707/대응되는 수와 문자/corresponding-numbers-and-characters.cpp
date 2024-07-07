#include <iostream>
#include <cstring>
#include <unordered_map>

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int strCnt, findCnt;
    std::unordered_map<int, std::string> iMap;
    std::unordered_map<std::string, int> sMap;

    std::cin >> strCnt >> findCnt;
    for (int i = 1; i <= strCnt; ++i)
    {
        std::string str;

        std::cin >> str;
        sMap[str] = i;
        iMap[i] = str;
    }

    while (findCnt--)
    {
        std::string toFind;
        std::cin >> toFind;

        int num = std::atoi(toFind.c_str());

        if (num)
        {
            std::cout << iMap[num] << '\n';
        }
        else
        {
            std::cout << sMap[toFind] << '\n';
        }
    }

    return 0;
}
#include <iostream>
#include <unordered_map>

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int strCnt;
    int ans = 0;
    std::unordered_map<std::string, int> umap;

    std::cin >> strCnt;
    while (strCnt--)
    {
        std::string str;

        std::cin >> str;
        ans = std::max(ans, ++umap[str]);
    }

    std::cout << ans << '\n';
    return 0;
}
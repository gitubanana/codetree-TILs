#include <iostream>
#include <unordered_map>

using t_umap = std::unordered_map<int, int>;

int main()
{
    std::cin.tie(0)->sync_with_stdio(0);

    int ans = 0;
    int size, goal;
    t_umap umap;

    std::cin >> size >> goal;
    while (size--)
    {
        int elem;

        std::cin >> elem;
        ans += umap[goal - elem];
        ++umap[elem];
    }

    std::cout << ans << '\n';
    return 0;
}
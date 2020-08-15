#include <iostream>
#include <vector>
#include <DynamicString>

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        std::vector<String> list;
        list.reserve(argc - 1);
        list.assign(argv + 1, argv + argc);
        for(auto iter = list.rbegin(); iter != list.rend(); ++iter)
        {
            String& str = *iter;
            for (int i = str.length() - 1; i >= 0; --i)
                std::cout << str[i];
             std::cout << ' ';
        }
        std::cout << '\n';
    }
    else
    {
        std::cout << String("No input arguments") << '\n';
    }
   
    return 1;
}
// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

struct Folder;

struct File
{
    std::string name;
    int size;
};

struct Folder
{
    std::string name;
    Folder* parent = nullptr;
    std::vector<Folder> folders;
    std::vector<File> files;
    int size;
};

std::vector<std::string> tokenize(std::string const& s)
{
    std::vector<std::string> tokens;

    auto it = s.cbegin();
    while (it != s.cend())
    {
        auto const delim = std::find(it, s.cend(), ' ');
        tokens.emplace_back(it, delim);
        it = delim == s.cend() ? delim : std::next(delim);
    }

    return tokens;
}

Folder* descend(Folder* currFolder, std::string_view name)
{
    auto it = std::find_if(currFolder->folders.begin(),
                           currFolder->folders.end(),
                           [&](auto&& each) { return each.name == name; });

    return &*it;
}

int computeSize(Folder& folder)
{
    folder.size = std::accumulate(folder.files.cbegin(),
                                  folder.files.cend(),
                                  std::accumulate(folder.folders.begin(),
                                                  folder.folders.end(),
                                                  0,
                                                  [](int const sum, Folder& each) {
                                                      return sum + computeSize(each);
                                                  }),
                                  [](int const sum, File const& each) {
                                      return sum + each.size;
                                  });

    return folder.size;
}

int smallestFolderOver(Folder& folder, int const best, int const idealSize)
{
    return std::accumulate(folder.folders.begin(),
                           folder.folders.end(),
                           folder.size >= idealSize ? std::min(best, folder.size)
                                                    : best,
                           [&](int acc, Folder& each) {
                               return smallestFolderOver(each, acc, idealSize);
                           });
}

int main()
{
    Folder root{"/"};

    std::string line;
    Folder* currFolder = &root;
    while (std::getline(std::cin, line))
    {
        std::vector<std::string> const tokens = tokenize(line);
        if (tokens[0] == "$")
        {
            if (tokens[1] == "cd")
            {
                if (tokens[2] == "/")
                {
                    currFolder = &root;
                }
                else if (tokens[2] == "..")
                {
                    currFolder = currFolder->parent;
                }
                else
                {
                    currFolder = descend(currFolder, tokens[2]);
                }
            }
        }
        else if (tokens[0] == "dir")
        {
            currFolder->folders.push_back({tokens[1], currFolder});
        }
        else
        {
            int size = std::stoi(tokens[0]);
            currFolder->files.push_back({tokens[1], size});
        }
    }

    computeSize(root);

    int const idealSize = root.size - 40000000;
    std::cout << smallestFolderOver(root, root.size, idealSize) << "\n";
}

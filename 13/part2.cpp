// g++ -std=c++17 part1.cpp && ./a.out < test
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Item
{
    enum class Type
    {
        integer,
        list
    };

    Type type;
    bool isDivider = false;
    int value;               // for `integer` types
    std::vector<Item> items; // for `list` types
};

std::vector<std::string> lex(std::string_view const s)
{
    std::vector<std::string> tokens;

    auto b = s.cbegin();
    while (b != s.cend())
    {
        if (*b == '[' || *b == ']')
        {
            tokens.push_back(std::string(1, *b));
            ++b;
        }
        else if (::isdigit(*b))
        {
            auto const e = std::find_if_not(std::next(b), s.cend(), ::isdigit);
            tokens.push_back(std::string{b, e});
            b = e;
        }
        else // skip commas
        {
            ++b;
        }
    }

    return tokens;
}

Item parse(std::vector<std::string>::const_iterator& it)
{
    if (*it == "[")
    {
        Item list;
        list.type = Item::Type::list;

        ++it;
        while (*it != "]")
            list.items.push_back(parse(it));
        ++it;

        return list;
    }
    else
    {
        Item integer;
        integer.type = Item::Type::integer;
        integer.value = std::stoi(*it++);
        return integer;
    }
}

void print(Item const& item)
{
    if (item.type == Item::Type::integer)
    {
        std::cout << item.value;
    }
    else
    {
        std::cout << "[";
        bool first = true;
        for (auto const& item : item.items)
        {
            if (!first)
                std::cout << ",";
            print(item);
            first = false;
        }
        std::cout << "]";
    }
}

Item parse(std::string_view const s)
{
    std::vector<std::string> const tokens = lex(s);
    auto it = tokens.cbegin();
    Item item = parse(it);
    return item;
}

int compare(Item lhs, Item rhs)
{
    auto const wrap = [](Item outer) {
        Item item;
        item.type = Item::Type::list;
        item.items.push_back(outer);
        return item;
    };

    if (lhs.type == Item::Type::integer && rhs.type == Item::Type::integer)
    {
        if (lhs.value < rhs.value)
        {
            return -1;
        }
        else if (rhs.value < lhs.value)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (lhs.type != Item::Type::list || rhs.type != Item::Type::list)
    {
        if (lhs.type != Item::Type::list)
        {
            lhs = wrap(lhs);
        }
        else
        {
            rhs = wrap(rhs);
        }
    }

    auto bl = lhs.items.cbegin();
    auto const el = lhs.items.cend();
    auto br = rhs.items.cbegin();
    auto const er = rhs.items.cend();

    while (bl != el && br != er)
    {
        int const result = compare(*bl, *br);
        if (result)
        {
            return result;
        }
        else
        {
            ++bl;
            ++br;
        }
    }

    if (bl == el && br == er)
    {
        return 0;
    }
    else if (bl == el)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

int main()
{
    std::vector<Item> items;
    std::string s;
    while (std::getline(std::cin, s))
    {
        if (!s.empty())
            items.push_back(parse(s));
    }

    items.push_back(parse("[[2]]"));
    items.back().isDivider = true;
    items.push_back(parse("[[6]]"));
    items.back().isDivider = true;

    std::sort(items.begin(), items.end(), [](Item const& lhs, Item const& rhs) {
        return compare(lhs, rhs) < 0;
    });

    int product = 1;
    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i].isDivider)
            product *= (i + 1);
    }

    std::cout << product << "\n";
}

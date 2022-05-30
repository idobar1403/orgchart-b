#include "sources/OrgChart.hpp"
#include <iostream>
#include <string.h>
using namespace ariel;

int main()
{
    OrgChart<string> org;
    string left_node = "";
    string right_node = "";
    int choice = 0;
    std::cout << "Hello!\n";
    std::cout << "To enter root select 1\n";
    std::cout << "To enter sub select 2\n";
    std::cout << "To print the chart select 3\n";
    std::cout << "To iterate over the chart select 4\n";
    std::cout << "To exit enter any other number\n";
    std::cin >> choice;
    std::cout<<endl;
    while (choice > 0 && choice < 5)
    {
        if (choice == 1)
        {
            std::cout << "Enter root\n";
            std::cin >> left_node;
            org.add_root(left_node);
        }
        else if (choice == 2)
        {
            std::cout << "Enter first name\n";
            cin >> left_node;
            std::cout << "Enter second name\n";
            cin >> right_node;
            org.add_sub(left_node, right_node);
        }
        else if (choice == 3)
        {
            std::cout << org << endl;
        }
        else if (choice == 4)
        {
            std::cout << "To level order select 1\n";
            std::cout << "To reverse order select 2\n";
            std::cout << "To preorder select 3\n";
            std::cin >> choice;
            if (choice == 1)
            {
                for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it)
                {
                    std::cout << (*it) << " ";
                }
            }
            else if (choice == 2)
            {
                for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it)
                {
                    std::cout << (*it) << " ";
                }
            }
            else if (choice == 3)
            {
                for (auto it = org.begin_preorder(); it != org.end_preorder(); ++it)
                {
                    std::cout << (*it) << " ";
                }
            }
            std::cout<<endl;
        }
        else
        {
            std::cout<<"Bye\n";
            break;
        }
        std::cout << "To enter root select 1\n";
        std::cout << "To enter sub select 2\n";
        std::cout << "To print the chart select 3\n";
        std::cout << "To iterate over the chart select 4\n";
        std::cout << "To exit enter any other number\n";
        std::cin >> choice;
        std::cout<<endl;
    }
}
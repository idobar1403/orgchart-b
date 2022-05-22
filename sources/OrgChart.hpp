#pragma once

#include <iostream>
#include <stdexcept>
#include <stack>
#include <vector>
#include <queue>
using namespace std;
namespace ariel
{
    template <typename T = std::string>
    class OrgChart
    {
    private:
        struct Node
        {
            T data;
            vector<Node *> childs;

            Node(const T &data) : data(data) {}
        };
        Node *root;

        bool is_empty()
        {
            return this->root == nullptr;
        }
        Node *search_node(Node *node, T data)
        {
            if (data == node->data)
            {
                return node;
            }
            stack<Node *> s;
            queue<Node *> q;
            Node *curr = this->root;
            s.push(curr);
            while (!s.empty())
            {
                curr = s.top();
                s.pop();
                if (curr->childs.size() > 0)
                {
                    for (auto c : curr->childs)
                    {
                        q.push(c);
                        s.push(c);
                    }
                }
            }
            Node *a = nullptr;
            while (!q.empty())
            {
                a = q.front();
                if (data == a->data)
                {
                    return a;
                }
                q.pop();
            }

            return nullptr;
        }

    public:
        enum order_type
        {
            LevelOrder,
            PreOrder,
            ReverseOrder
        };
        OrgChart()
        {
            this->root = nullptr;
        }
        ~OrgChart()
        {
            stack<Node *> st1;
            stack<Node *> st2;
            if (this->root->childs.size() > 0)
            {
                for (size_t i = this->root->childs.size() - 1; i > 0; i--)
                {
                    st1.push(this->root->childs.at(i));
                    st2.push(this->root->childs.at(i));
                }
                st1.push(this->root->childs.at(0));
                st2.push(this->root->childs.at(0));
            }
            while (!st1.empty())
            {
                Node *curr_node = st1.top();
                st1.pop();
                if (curr_node->childs.size() > 0)
                {
                    for (size_t i = curr_node->childs.size() - 1; i > 0; i--)
                    {
                        st1.push(curr_node->childs.at(i));
                        st2.push(curr_node->childs.at(i));
                    }
                    st1.push(curr_node->childs.at(0));
                    st2.push(curr_node->childs.at(0));
                }
            }
            while (!st2.empty())
            {
                Node *curr = st2.top();
                st2.pop();
                cout << "delete: " << curr->data << endl;
                delete curr;
            }
            cout << "delete root " << endl;
            delete this->root;
        }

        OrgChart(OrgChart<T> &other) noexcept
        {
            this->root = other.root;
        }
        OrgChart(OrgChart<T> &&other) noexcept
        {
            this->root = other.root;
            other.root = nullptr;
        }
        OrgChart &operator=(OrgChart<T> &&other) noexcept
        {
            this->root = other.root;
            other.root = nullptr;
        }
        OrgChart &operator=(OrgChart<T> other) noexcept
        {
            this->root = other.root;
            other.root = nullptr;
        }
        OrgChart<T> &add_root(const T &data)
        {
            if (this->root == nullptr)
            {
                this->root = new Node(data);
            }
            else
            {
                this->root->data = data;
            }
            return *this;
        }
        OrgChart<T> &add_sub(const T &data1, const T &data2)
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("OrgChart as no root");
            }
            if (data1.length() == 0 || data2.length() == 0)
            {
                throw std::invalid_argument("can't add empty data to chart");
            }
            Node *node1 = this->search_node(this->root, data1);
            // Node *node2 = this->search_node(this->root, data2);
            if (node1 == nullptr)
            {
                throw std::invalid_argument("can't add this nodes");
            }
            node1->childs.push_back(new Node(data2));

            // if (node2 == nullptr)
            // {
            //     node1->childs.push_back(new Node(data2));
            // }
            // else
            // {
            //     Node *check = this->search_node(node2, data1);
            //     if (check == nullptr)
            //     {
            //         node1->childs.push_back(node2);
            //     }
            //     else
            //     {
            //         throw std::invalid_argument("can't add higher node to be sub node");
            //     }
            // }

            return *this;
        }
        friend ostream &operator<<(ostream &os, const OrgChart &chart)
        {
            os << "chart" << endl;
            return os;
        }

        class iterator
        {
        private:
            Node *curr_node;
            stack<Node *> st;
            std::queue<Node *> que;
            order_type order;

        public:
            iterator(order_type order, Node *node) : order(order), curr_node(node)
            {
                if (this->curr_node != nullptr)
                {
                    if (!this->curr_node->childs.empty())
                    {

                        if (this->order == order_type::LevelOrder)
                        {
                            for (size_t i = 0; i < this->curr_node->childs.size(); i++)
                            {
                                this->que.push(this->curr_node->childs.at(i));
                            }
                        }
                    }
                    else if (this->order == order_type::PreOrder)
                    {
                        if (this->curr_node->childs.size() > 0)
                        {
                            for (size_t i = this->curr_node->childs.size() - 1; i > 0; i--)
                            {
                                this->st.push(this->curr_node->childs.at(i));
                            }
                            this->st.push(this->curr_node->childs.at(0));
                        }
                    }
                    else
                    {
                        Node *n = this->curr_node;
                        this->que.push(n);
                        while (!this->que.empty())
                        {
                            n = this->que.front();
                            this->que.pop();
                            this->st.push(n);
                            if (n->childs.size() > 0)
                            {
                                for (size_t i = n->childs.size() - 1; i > 0; i--)
                                {
                                    this->que.push(n->childs.at(i));
                                }
                                this->que.push(n->childs.at(0));
                            }
                        }
                        this->curr_node = st.top();
                        st.pop();
                    }
                }
               
            }
            iterator &operator++()
            {
                if (this->order == order_type::LevelOrder)
                {
                    if (this->que.empty())
                    {
                        this->curr_node = nullptr;
                        return *this;
                    }
                    this->curr_node = this->que.front();
                    this->que.pop();
                    for (size_t i = 0; i < this->curr_node->childs.size(); i++)
                    {
                        this->que.push(this->curr_node->childs.at(i));
                    }
                }
                else if (this->order == order_type::PreOrder)
                {
                    if (this->st.empty())
                    {
                        this->curr_node = nullptr;
                        return *this;
                    }
                    this->curr_node = this->st.top();
                    this->st.pop();
                    if (this->curr_node->childs.size() > 0)
                    {
                        for (size_t i = this->curr_node->childs.size() - 1; i > 0; i--)
                        {
                            this->st.push(this->curr_node->childs.at(i));
                        }
                        this->st.push(this->curr_node->childs.at(0));
                    }
                }
                else
                {
                    if (this->st.empty())
                    {
                        this->curr_node = nullptr;
                        return *this;
                    }
                    this->curr_node = this->st.top();
                    this->st.pop();
                }
                return *this;
            }
            T &operator*() const
            {
                return curr_node->data;
            }
            T *operator->() const
            {
                return &(curr_node->data);
            }
            bool operator==(const iterator &other)
            {
                return this->curr_node == other.curr_node;
            }
            bool operator!=(const iterator &other)
            {
                return this->curr_node != other.curr_node;
            }
        };
        iterator begin_level_order()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::LevelOrder, this->root);
        }
        iterator end_level_order()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::LevelOrder, nullptr);
        }
        iterator begin_reverse_order()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::ReverseOrder, this->root);
        }
        iterator reverse_order()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::ReverseOrder, nullptr);
        }
        iterator begin_preorder()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::PreOrder, this->root);
        }
        iterator end_preorder()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::PreOrder, nullptr);
        }
        iterator begin()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::LevelOrder, this->root);
        }
        iterator end()
        {
            if(this->root==nullptr){
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::LevelOrder, nullptr);
        }
    };
}
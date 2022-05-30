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
        // search the nodes in the chart
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
            // iterate over all the nodes
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
            // check if the node is exist in the nodes of the chart by comparing the data
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
        static string print_tree(Node *node, bool last, string childs)
        {
            string ans;
            ans += childs + " └──" + node->data + "\n";
            if (last)
            {
                childs += "   ";
            }
            else
            {
                childs += "-|";
            }
            for (size_t i = 0; i < node->childs.size(); i++)
            {
                ans += print_tree(node->childs.at(i), i == node->childs.size() - 1, childs);
            }
            return ans;
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
            // level order in order to destruct the nodes
            if (root != nullptr)
            {
                stack<Node *> st;
                queue<Node *> que;
                que.push(root);
                Node *curr = nullptr;
                // take every node and push his childs to the queue and
                // every pop from the queue push the node to the stack
                while (!que.empty())
                {
                    curr = que.front();
                    st.push(curr);
                    que.pop();
                    for (auto it = curr->childs.rbegin(); it != curr->childs.rend(); ++it)
                    {
                        que.push(*it);
                    }
                }
                while (!st.empty())
                {
                    curr = st.top();
                    st.pop();
                    delete curr;
                }
            }
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
        // add root node
        OrgChart<T> &add_root(const T &data)
        {
            if(data.length()==0|| data == " "){
                 throw std::invalid_argument("bad char");
            }
            // check for good input in the root data
            for (size_t i = 0; i < data.length(); i++)
            {
                if (isprint(data.at(i)) == 0)
                {
                    throw std::invalid_argument("bad char");
                }
            }
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
        // add nodes to the chart
        OrgChart<T> &add_sub(const T &data1, const T &data2)
        {
            if(data1.length()==0||data2.length()==0||data1==" " || data2 == " "){
                 throw std::invalid_argument("bad char");
            }
            // check if the chars in data1 are good 
            for (size_t i = 0; i < data1.length(); i++)
            {
                if (isprint(data1.at(i)) == 0)
                {
                    throw std::invalid_argument("bad char");
                }
            }
            // check if the chars in data2 are good 
            for (size_t i = 0; i < data2.length(); i++)
            {
                if (isprint(data2.at(i)) == 0)
                {
                    throw std::invalid_argument("bad char");
                }
            }
            if (this->root == nullptr)
            {
                throw std::invalid_argument("OrgChart as no root");
            }
            if (data1.length() == 0 || data2.length() == 0)
            {
                throw std::invalid_argument("can't add empty data to chart");
            }
            // search if the first name exist
            Node *node1 = this->search_node(this->root, data1);
            // error if not
            if (node1 == nullptr)
            {
                throw std::invalid_argument("can't add this nodes");
            }
            // create the new node2 and add him as child to node1
            node1->childs.push_back(new Node(data2));
            return *this;
        }
        friend ostream &operator<<(ostream &os, const OrgChart &chart)
        {
            os << print_tree(chart.root, true, "");
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
            iterator(order_type order = order_type::LevelOrder, Node *node = nullptr) : order(order), curr_node(node)
            {
                if (this->curr_node != nullptr)
                {
                    // level order
                    if (this->order == order_type::LevelOrder)
                    {
                        if (!this->curr_node->childs.empty())
                        {
                            if (!this->curr_node->childs.empty())
                            {
                                // iterate over the childs
                                for (auto &child : this->curr_node->childs)
                                {
                                    this->que.push(child);
                                }
                            }
                        }
                    }
                    // pre order
                    else if (this->order == order_type::PreOrder)
                    {
                        if (this->curr_node->childs.size() > 0)
                        {
                            // iterate over the childs and push them to stack in order to get the reverse order of every row
                            for (size_t i = this->curr_node->childs.size() - 1; i > 0; i--)
                            {
                                this->st.push(this->curr_node->childs.at(i));
                            }
                            this->st.push(this->curr_node->childs.at(0));
                        }
                    }
                    // reverse level order
                    else
                    {
                        Node *n = this->curr_node;
                        this->que.push(n);
                        while (!this->que.empty())
                        {
                            // push the current node to the stack
                            n = this->que.front();
                            this->que.pop();
                            this->st.push(n);
                            if (n->childs.size() > 0)
                            {
                                // push the current node childs to the queue
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
                if (!this->st.empty() || !this->que.empty())
                {
                    if (this->order == order_type::LevelOrder)
                    {
                        // just go to the next childs and put the other childs to the queue
                        if (this->que.empty())
                        {
                            this->curr_node = nullptr;
                            return *this;
                        }
                        this->curr_node = this->que.front();
                        this->que.pop();
                        if (this->curr_node != nullptr)
                        {
                            // iterator over the current childs of the current node 
                            if (!this->curr_node->childs.empty())
                            {
                                for (auto &child : this->curr_node->childs)
                                {
                                    this->que.push(child);
                                }
                            }
                        }
                    }
                    else if (this->order == order_type::PreOrder)
                    {
                        if (this->st.empty())
                        {
                            this->curr_node = nullptr;
                            return *this;
                        }
                        // go to the next child and put the childs in to the stack
                        this->curr_node = this->st.top();
                        this->st.pop();
                        if (this->curr_node->childs.size() > 0)
                        {
                            // for the preorder insert the childs in reverse order
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
                        // just pop the last node from the stack
                        this->curr_node = this->st.top();
                        this->st.pop();
                    }
                    return *this;
                }
                // if there are no nodes to iterate to
                this->curr_node = nullptr;
                return *this;
            }
            T &operator*() const
            {
                // get the data of the node
                return curr_node->data;
            }
            T *operator->() const
            {
                // get pointer to the node
                return &(curr_node->data);
            }
            bool operator==(const iterator &other)
            {
                // compare two nodes
                return this->curr_node == other.curr_node;
            }
            bool operator!=(const iterator &other)
            {
                // compare two nodes
                return this->curr_node != other.curr_node;
            }
        };
        iterator begin_level_order()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator{order_type::LevelOrder, this->root};
        }
        iterator end_level_order()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator{order_type::LevelOrder, nullptr};
        }
        iterator begin_reverse_order()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator{order_type::ReverseOrder, this->root};
        }
        iterator reverse_order()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator{order_type::ReverseOrder, nullptr};
        }
        iterator begin_preorder()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator{order_type::PreOrder, this->root};
        }
        iterator end_preorder()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator{order_type::PreOrder, nullptr};
        }
        iterator begin()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::LevelOrder, this->root);
        }
        iterator end()
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("empty chart");
            }
            return iterator(order_type::LevelOrder, nullptr);
        }
    };
}
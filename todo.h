#ifndef SINGLY_LINKED_LISvoid_H
#define SINGLY_LINKED_LISvoid_H

#include <string>

struct TodoItem {
    unsigned int id;
    std::string title;
    bool complete;
};

struct Node {
    TodoItem data;
    Node *next;
    Node(TodoItem data) : data(data), next(nullptr) {}
};

class TodoList {
private:
    Node *head = nullptr;
    Node *tail = nullptr;
    unsigned int length = 0;

public:
    TodoList();
    TodoList(Node *head);
    TodoList(const TodoItem &data);
    ~TodoList();

    bool append(const TodoItem &data);
    bool pop_back();
    bool pop();

    TodoItem *top() const;
    TodoItem *bot() const;

    TodoItem *search_id(const unsigned int id) const;
    bool delete_id(const unsigned int id);

    void clear();
    unsigned int size() const noexcept;
    bool is_empty() const noexcept;

    class Iterator {
    public:
        Node *current;
        Iterator(Node *start);
        Iterator &operator++();
        bool operator!=(const Iterator &other) const;
        TodoItem &operator*() const;
    };

    Iterator begin() const;
    Iterator end() const;
};

#endif  // !SINGLY_LINKED_LISvoid_H

#include "todo.h"

TodoList::TodoList() : head(nullptr), tail(nullptr), length(0) {};

TodoList::TodoList(Node *head) : head(head), tail(head), length(1) {};

TodoList::TodoList(const TodoItem &data) {
    Node *_head = new Node(data);
    head = _head;
    tail = _head;
    length = 1;
}

TodoList::~TodoList() { clear(); }

// O(1)
bool TodoList::append(const TodoItem &data) {
    Node *node = new Node(data);

    if (is_empty()) {
        head = tail = node;
        length++;
        return true;
    }

    tail->next = node;
    tail = node;
    length++;
    return true;
}

// O(1)
bool TodoList::pop() {
    length--;
    if (head->next == nullptr && length == 1 && head == tail) {
        delete head;
        head = tail = nullptr;
        return true;
    }

    Node *temp = head;
    head = head->next;
    delete temp;
    temp = nullptr;
    return true;
}

// O(n)
bool TodoList::pop_back() {
    length--;
    if (head->next == nullptr && length == 1 && head == tail) {
        delete tail;
        tail = head = nullptr;
        return true;
    }

    Node *curr = head;
    while (curr->next != tail) {
        curr = curr->next;
    }
    delete tail;
    curr->next = nullptr;
    tail = curr;
    return true;
}

void TodoList::clear() {
    if (!is_empty()) {
        Node *temp = nullptr;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
            temp = nullptr;
            length--;
        }
    }
}

unsigned int TodoList::size() const noexcept { return length; }

bool TodoList::is_empty() const noexcept {
    return length == 0 && head == nullptr;
}

TodoItem *TodoList::top() const {
    if (is_empty()) {
        return nullptr;
    }
    return &head->data;
}

TodoItem *TodoList::bot() const {
    if (is_empty()) {
        return nullptr;
    }
    return &tail->data;
}

TodoItem *TodoList::get(const unsigned int index) const {
    if (is_outbound(index) || is_empty()) {
        return nullptr;
    }
    if (index == 0) {
        return top();
    }
    if (index == length - 1) {
        return bot();
    }
    Node *curr = get_node_at(index);
    return &curr->data;
}

TodoItem *TodoList::search_id(const unsigned int id) const {
    if (is_empty()) {
        return nullptr;
    }
    Node *current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

bool TodoList::delete_id(const unsigned int id) {
    if (is_empty()) {
        return false;
    }
    if (head->data.id == id) {
        pop();
        return true;
    }
    if (tail->data.id == id) {
        pop_back();
        return true;
    }
    Node *current = head;
    while (current->next != nullptr) {
        if (current->next->data.id == id) {
            Node *temp = current->next;
            current->next = temp->next;
            delete temp;
            temp = nullptr;
            length--;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool TodoList::is_outbound(unsigned int index) const {
    return index > length - 1;
}

Node *TodoList::get_node_at(unsigned int index) const {
    unsigned int count = 0;
    Node *current = head;
    while (count < index && current != nullptr) {
        current = current->next;
        count++;
    }
    return current;
}

// fungsi Iterator agar bisa di loop secara otomatis
typename TodoList::Iterator TodoList::begin() const { return Iterator(head); }
typename TodoList::Iterator TodoList::end() const { return Iterator(nullptr); }

TodoList::Iterator::Iterator(Node *start) : current(start) {}

typename TodoList::Iterator &TodoList::Iterator::operator++() {
    if (current) {
        current = current->next;
    }
    return *this;
}

bool TodoList::Iterator::operator!=(
    const typename TodoList::Iterator &other) const {
    return current != other.current;
}

TodoItem &TodoList::Iterator::operator*() const { return current->data; }

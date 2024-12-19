#include "fileio.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "todo.h"

BinaryFileHanlder::BinaryFileHanlder(const std::string& filename)
    : m_filename(filename) {
    m_file.open(m_filename, std::ios::binary | std::ios::in | std::ios::out |
                                std::ios::app);
    if (!m_file.is_open()) {
        throw std::runtime_error("Tidak bisa membuka file database");
    }
}

BinaryFileHanlder::~BinaryFileHanlder() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

void BinaryFileHanlder::read(TodoList& output) {
    m_file.clear();
    m_file.seekg(0, std::ios::beg);

    while (m_file.peek() != EOF) {
        TodoItem item;

        m_file.read(reinterpret_cast<char*>(&item.id), sizeof(item.id));

        size_t title_length;
        m_file.read(reinterpret_cast<char*>(&title_length),
                    sizeof(title_length));

        item.title.resize(title_length);
        m_file.read(item.title.data(), title_length);

        m_file.read(reinterpret_cast<char *>(&item.complete), sizeof(item.complete));

        output.append(item);
    }
}

void BinaryFileHanlder::write(const TodoList& input) {
    m_file.close();
    m_file.open(m_filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!m_file.is_open()) {
        throw std::runtime_error("Gagal membuka file");
    }

    for (TodoItem item : input) {
        m_file.write(reinterpret_cast<char*>(&item.id), sizeof(item.id));

        size_t title_length = item.title.size();
        m_file.write(reinterpret_cast<char*>(&title_length),
                     sizeof(title_length));
        m_file.write(item.title.data(), title_length);

        m_file.write(reinterpret_cast<char *>(&item.complete), sizeof(item.complete));
    }
}

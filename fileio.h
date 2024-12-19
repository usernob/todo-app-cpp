#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include "todo.h"


class BinaryFileHanlder {
private:
    std::fstream m_file;
    std::string m_filename;

public:
    BinaryFileHanlder(const std::string &filename);

    ~BinaryFileHanlder();

    void read(TodoList &output);
    void write(const TodoList &input);
};

#endif  // !FILEIO_H

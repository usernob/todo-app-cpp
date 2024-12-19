#ifndef TERMINAL_FANCY_H
#define TERMINAL_FANCY_H

#include <unistd.h>

#include <string>

class EscapeSequence {
private:
    bool is_support_esc_seq;

public:
    EscapeSequence();

    class Modifier {
    public:
        static const char* RED;
        static const char* GREEN;
        static const char* YELLOW;
        static const char* BLUE;
        static const char* PURPLE;
        static const char* CYAN;
        static const char* WHITE;
        static const char* BOLD;
        static const char* UNDERLINE;
        static const char* RESET;
    };
    std::string escape(const char* input, const char* mod);
};

#endif  // !TERMINAL_FANCY_H

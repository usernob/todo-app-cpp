#include "terminal_fancy.h"

EscapeSequence::EscapeSequence() {
    // check apakah terminal support ansi escape sequence
    // karena masalah di cmd windows yang tidak support ansi escape sequence
    const char* term = getenv("TERM");
    if (term != nullptr) {
        std::string term_str = term;
        if (term_str == "dumb" || term_str.empty()) {
            is_support_esc_seq = false;
            return;
        }
    } else {
        is_support_esc_seq = false;
        return;
    }
    is_support_esc_seq = (isatty(STDOUT_FILENO) != 0);
}

std::string EscapeSequence::escape(const char* input, const char* mod) const {
    // jika tidak support maka kembalikan lagi input
    if (!is_support_esc_seq) {
        return input;
    }

    return std::string(mod) + input + "\x1b[0m";
}

const char* EscapeSequence::Modifier::RED = "\x1b[1;31;40m";
const char* EscapeSequence::Modifier::GREEN = "\x1b[1;32;40m";
const char* EscapeSequence::Modifier::YELLOW = "\x1b[1;33;40m";
const char* EscapeSequence::Modifier::BLUE = "\x1b[1;34;40m";
const char* EscapeSequence::Modifier::PURPLE = "\x1b[1;35;40m";
const char* EscapeSequence::Modifier::CYAN = "\x1b[1;36;40m";
const char* EscapeSequence::Modifier::WHITE = "\x1b[1;37;40m";
const char* EscapeSequence::Modifier::BOLD = "\x1b[1m";
const char* EscapeSequence::Modifier::UNDERLINE = "\x1b[4m";
const char* EscapeSequence::Modifier::RESET = "\x1b[0m";

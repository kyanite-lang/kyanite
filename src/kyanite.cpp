#include <iostream>
#include <cstring>
#include "Lexer.cpp"

const char* _KY_VERSION      = "0.0.1-alpha";
const char* _KY_OUT_TITLE    = "🐖 kyanite";
const char* _KY_OUT_GOOD     = "🎉 no errors";
const char* _KY_OUT_BAD      = "💀 uh oh!";
const char* _KY_OUT_BADPARAM = "👀 oops...";

const char* _SAMPLE          = "function potato(str)\n    print(str)\nend\n\nx = if p is __list then 1 else (x / (4 * 3))\n";

void print_badparams() noexcept {
    std::cout << _KY_OUT_BADPARAM << "\n\nusage: kyanite [-v]" << std::endl;
}

void print_version() noexcept {
    std::cout << _KY_OUT_TITLE << " " << _KY_VERSION << std::endl;
}

void print_good_output() noexcept {
    std::cout << _KY_OUT_GOOD << std::endl;
}

void print_bad_output() noexcept {
    std::cout << _KY_OUT_BAD << std::endl;
}

int do_kyanite() noexcept {
    Lexer lexer(_SAMPLE);
    for(Token t = lexer.next(); !t.is(Token::TokenType::EndOfStream); t = lexer.next()) {
        if (t.is(Token::TokenType::Unknown)) {
            print_bad_output();
            std::cout << "unexpected token: " << static_cast<std::string>(t.string()) << std::endl;
            return -1;
        }
        if (t.is(Token::TokenType::Newline)) {
            std::cout << "[newline] ";
        } else {
            std::cout << "`" << t.string() << "` ";
        }
    }
    std::cout << std::endl;
    print_good_output();
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return do_kyanite();
    } else if (argc == 2) {
        if (strcmp(argv[1], "-v") == 0) {
            print_version();
            return 0;
        }
    }
    print_badparams();
    return -1;    
}
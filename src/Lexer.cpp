#include <string>
#include <string_view>

class Token {
  public:
    enum class TokenType {
        Function,           Object,          Event,              Invoke,
        On,                 Do,              End,                For,
        In,                 To,              By,                 If,
        Unless,             Then,            Else,               Break,
        Return,             Switch,          Typeswitch,         Default,
        LeftParen,          RightParen,      LeftList,           RightList,
        LeftVars,           RightVars,       Comma,              Dot,
        Colon,              Typename,        Is,                 Equal,
        EqualEqual,         Not,             NotEqual,           InverseOp,
        GreaterThan,        LessThan,        GreaterThanOrEqual, LessThanOrEqual, 
        Plus,               Minus,           Divide,             Multiply,
        PlusEqual,          MinusEqual,      DivideEqual,        MultiplyEqual,
        Modulo,             Power,           LeftShift,          RightShift,
        ModuloEqual,        PowerEqual,      LeftShiftEqual,     RightShiftEqual,
        PlusPlus,           MinusMinus,      Bang,               StringLiteral,
        IntLiteral,         FloatLiteral,    BooleanLiteral,     Identifier,
        Nil,                Newline,            EndOfStream,     Unknown
    };

    Token(TokenType type) noexcept : type_{type} {}
    Token(TokenType type, const char* start, std::size_t length) noexcept
        : type_{type}, string_(start, length) {}
    Token(TokenType type, const char* start, const char* end) noexcept
        : type_{type}, string_(start, std::distance(start, end)) {}

    TokenType kind() const noexcept { return type_; }
    void kind(TokenType type) noexcept { type_ = type; }
    bool is(TokenType type) const noexcept { return type == type_; }
    bool is_one_of(TokenType t1, TokenType t2) const noexcept { return is(t1) || is(t2); }

    std::string_view string() const noexcept { return string_; }

  private:
    TokenType type_;
    std::string_view string_;
};

class Lexer {
  public:
    Lexer(const char* stream) noexcept : stream_{stream} {}

    Token next() noexcept;

  private:
    char peek() const noexcept { return *stream_; }
    char get()        noexcept { return *stream_++; }

    Token atom(Token::TokenType) noexcept;
    Token from(Token::TokenType, const char* start) noexcept;

    const char* stream_ = nullptr;
};

bool is_space(char c) noexcept { return (c == ' ' || c == '\t'); }
bool is_alpha(char c) noexcept { return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)); }
bool is_digit(char c) noexcept { return (c >= 48 && c <= 57); }
bool is_alpha_digit(char c) noexcept { return (is_alpha(c) || is_digit(c)); }
bool is_natural_digit(char c) noexcept { return (c >= 49 && c <= 57); }
bool is_special_alpha_digit(char c) noexcept { return (is_alpha_digit(c) || c == '_'); }
bool is_newline(char c) noexcept { return (c == '\n' || c == '\r'); }

Token Lexer::atom(Token::TokenType type) noexcept { return Token(type, stream_++, 1); }
Token Lexer::from(Token::TokenType type, const char* start) noexcept { return Token(type, start, stream_++); }

Token Lexer::next() noexcept {
    while (is_space(peek())) get();

    if (peek() == '\0') return atom(Token::TokenType::EndOfStream);

    const char *start = stream_;

    if (is_newline(peek())) {
        while (is_newline(peek())) get();
        return Token(Token::TokenType::Newline, start, stream_);
    }

    switch (peek()) {
        case '(': return atom(Token::TokenType::LeftParen);
        case ')': return atom(Token::TokenType::RightParen);
        case '[': return atom(Token::TokenType::LeftList);
        case ']': return atom(Token::TokenType::RightList);
        case '{': return atom(Token::TokenType::LeftVars);
        case '}': return atom(Token::TokenType::RightVars);
        case ',': return atom(Token::TokenType::Comma);
        case '.': return atom(Token::TokenType::Dot);
        case ':': return atom(Token::TokenType::Colon);
        case '~': return atom(Token::TokenType::InverseOp);
        case '+':
            get();
            switch (peek()) {
                case '+': return from(Token::TokenType::PlusPlus, start);
                case '=': return from(Token::TokenType::PlusEqual, start);
            }
            return Token(Token::TokenType::Plus, start, 1);
        case '-':
            get();
            switch (peek()) {
                case '-': return from(Token::TokenType::MinusMinus, start);
                case '=': return from(Token::TokenType::MinusEqual, start);
            }
            return Token(Token::TokenType::Minus, start, 1);
        case '/':
            get();
            if (peek() == '=') return from(Token::TokenType::DivideEqual, start);
            return Token(Token::TokenType::Divide, start, 1);
        case '*':
            get();
            if (peek() == '=') return from(Token::TokenType::MultiplyEqual, start);
            return Token(Token::TokenType::Multiply, start, 1);
        case '%':
            get();
            if (peek() == '=') return from(Token::TokenType::ModuloEqual, start);
            return Token(Token::TokenType::Modulo, start, 1);
        case '^':
            get();
            if (peek() == '=') return from(Token::TokenType::PowerEqual, start);
            return Token(Token::TokenType::Power, start, 1);
        case '<':
            get();
            switch (peek()) {
                case '<':
                    get();
                    if (peek() == '=') return from(Token::TokenType::LeftShiftEqual, start);
                    return Token(Token::TokenType::LeftShift, start, 1);
                case '=': return from(Token::TokenType::LessThanOrEqual, start);
            }
            return Token(Token::TokenType::LessThan, start, 1);
        case '>':
            get();
            switch (peek()) {
                case '>':
                    get();
                    if (peek() == '=') return from(Token::TokenType::RightShiftEqual, start);
                    return Token(Token::TokenType::RightShift, start, 1);
                case '=': return from(Token::TokenType::GreaterThanOrEqual, start);
            }
            return Token(Token::TokenType::GreaterThan, start, 1);
        case '=':
            get();
            if (peek() == '=') return from(Token::TokenType::EqualEqual, start);
            return Token(Token::TokenType::Equal, start, 1);
        case '!':
            get();
            if (peek() == '=') return from(Token::TokenType::NotEqual, start);
            return Token(Token::TokenType::Bang, start, 1);
        case '_':
            get();
            if (peek() == '_') {
                get();
                while (is_alpha(peek())) get();
                std::string typestring { std::string_view(start, std::distance(start, stream_)) };
                if (typestring.compare("__list") == 0 || typestring.compare("__int") == 0 ||
                    typestring.compare("__bool") == 0 || typestring.compare("__float") == 0 ||
                    typestring.compare("__string") == 0) {
                    return Token(Token::TokenType::Typename, start, stream_);
                }
            }
            return from(Token::TokenType::Unknown, start);
    }

    if (peek() == '"') {
        get();
        while(peek() != '"' && peek() != '\0') {
            get();
        }
        if (peek() == '"') return from(Token::TokenType::StringLiteral, start);
        return Token(Token::TokenType::Unknown, start, stream_);
    }

    if (peek() == '0') {
        get();
        if (peek() != '.') return Token(Token::TokenType::IntLiteral, start, 1);
        get();
        if (!is_digit(peek())) return Token(Token::TokenType::Unknown, start, 2);
        while(is_digit(peek())) get();
        return Token(Token::TokenType::FloatLiteral, start, stream_);
    }

    if (is_digit(peek())) {
        get();
        if (peek() == '.') {
            get();
            if (!is_digit(peek())) return Token(Token::TokenType::Unknown, start, 2);
            while(is_digit(peek())) get();
            return Token(Token::TokenType::FloatLiteral, start, stream_);
        }
        if (is_digit(peek())) {
            while (is_digit(peek())) get();
        }
        return Token(Token::TokenType::IntLiteral, start, stream_);
    }

    if (is_alpha(peek())) {
        get();
        while(is_special_alpha_digit(peek())) get();
        if (peek() == '?') get();
        std::string ident { std::string_view(start, std::distance(start, stream_)) };
        if (ident.compare("function") == 0)   return Token(Token::TokenType::Function, start, stream_);
        if (ident.compare("object") == 0)     return Token(Token::TokenType::Object, start, stream_);
        if (ident.compare("event") == 0)      return Token(Token::TokenType::Event, start, stream_);
        if (ident.compare("invoke") == 0)     return Token(Token::TokenType::Invoke, start, stream_);
        if (ident.compare("on") == 0)         return Token(Token::TokenType::On, start, stream_);
        if (ident.compare("do") == 0)         return Token(Token::TokenType::Do, start, stream_);
        if (ident.compare("end") == 0)        return Token(Token::TokenType::End, start, stream_);
        if (ident.compare("for") == 0)        return Token(Token::TokenType::For, start, stream_);
        if (ident.compare("in") == 0)         return Token(Token::TokenType::In, start, stream_);
        if (ident.compare("to") == 0)         return Token(Token::TokenType::To, start, stream_);
        if (ident.compare("by") == 0)         return Token(Token::TokenType::By, start, stream_);
        if (ident.compare("if") == 0)         return Token(Token::TokenType::If, start, stream_);
        if (ident.compare("unless") == 0)     return Token(Token::TokenType::Unless, start, stream_);
        if (ident.compare("then") == 0)       return Token(Token::TokenType::Then, start, stream_);
        if (ident.compare("else") == 0)       return Token(Token::TokenType::Else, start, stream_);
        if (ident.compare("break") == 0)      return Token(Token::TokenType::Break, start, stream_);
        if (ident.compare("return") == 0)     return Token(Token::TokenType::Return, start, stream_);
        if (ident.compare("switch") == 0)     return Token(Token::TokenType::Switch, start, stream_);
        if (ident.compare("typeswitch") == 0) return Token(Token::TokenType::Typeswitch, start, stream_);
        if (ident.compare("default") == 0)    return Token(Token::TokenType::Default, start, stream_);
        if (ident.compare("nil") == 0)        return Token(Token::TokenType::Nil, start, stream_);
        if (ident.compare("true") == 0 || ident.compare("false") == 0) return Token(Token::TokenType::BooleanLiteral, start, stream_);
        return Token(Token::TokenType::Identifier, start, stream_);
    }

    return atom(Token::TokenType::Unknown);
}
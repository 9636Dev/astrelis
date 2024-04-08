#include "Lexer.hpp"

namespace Nebula::Shader
{
    Lexer::Lexer(std::string_view source) noexcept : m_Source(source) {}
    Lexer::Lexer(const Lexer&) = default;
    Lexer& Lexer::operator=(const Lexer&) = default;



} // namespace Nebula::Shader

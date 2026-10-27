#ifndef NILIB_FORMAT_HPP
#define NILIB_FORMAT_HPP

#include <format>
#include <ranges>
#include <string>
#include <string_view>
#include <iterator>
#include <format>
#include <iterator>
#include <string>
#include <utility>

#include <format>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <fstream>
#include <iomanip>
#include <sstream>

template <class... Args>
class CheckedFormatString
{
public:
    template <std::size_t N>
    consteval CheckedFormatString(char const (&str)[N])
        : fmt_(str)
    {
        if (count_fields(std::string_view{str, N - 1}) != sizeof...(Args))
            throw "Number of format fields does not match number of arguments";
    }

    constexpr std::format_string<Args...> get() const
    {
        return fmt_;
    }

private:
    std::format_string<Args...> fmt_;

    static consteval std::size_t count_fields(std::string_view str)
    {
        std::size_t count = 0;

        for (std::size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == '{')
            {
                if (i + 1 < str.size() && str[i + 1] == '{')
                {
                    ++i; // Escaped opening brace: {{
                }
                else
                {
                    ++count;
                }
            }
        }

        return count;
    }
};

template <typename T>
concept Iterable = requires(T const &value) {
    std::begin(value);
    std::end(value);
};

template <Iterable R>
struct RangeView
{
    R const &range;
};

template <Iterable R>
RangeView<R> fmt_range(R const &range)
{
    return {range};
}

template <typename R>
struct std::formatter<RangeView<R>, char>
{
    constexpr auto parse(std::format_parse_context &ctx)
    {
        return ctx.begin();
    }

    auto format(RangeView<R> const &view, std::format_context &ctx) const
    {
        auto out = ctx.out();
        *out++ = '[';

        bool first = true;

        for (auto const &value : view.range)
        {
            if (!first)
                out = std::format_to(out, ", ");

            first = false;
            out = std::format_to(out, "{}", value);
        }

        *out++ = ']';
        return out;
    }
};

template <typename T>
struct PtrView
{
    T const *ptr;
};

template <typename T>
PtrView<T> fmt_ptr(T const *ptr)
{
    return {ptr};
}

template <typename T>
struct std::formatter<PtrView<T>, char> : std::formatter<std::string>
{
    auto format(PtrView<T> const &view, std::format_context &ctx) const
    {
        if (view.ptr == nullptr)
            return std::formatter<std::string>::format("nullptr", ctx);

        return std::format_to(ctx.out(), "{}", *view.ptr);
    }
};

std::string fmt_bytes(size_t bytes, bool useBinary = true);

#endif
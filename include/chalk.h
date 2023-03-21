#pragma once

#include <string>
#include <string_view>
#include <iostream>

#define DEFINE_UDL(name, Type) \
    inline constexpr auto operator"" name(const char* str, size_t size) { \
        return Type(str); \
    }

namespace chalk
{
/********** Utils *************************************************************/

    // Compile-time string
    template <size_t N>
    struct FixedString
    {
        static constexpr size_t Size = N;
        char value[N];

        constexpr FixedString() {}
        
        constexpr FixedString(const char str[N+1]) {
            for (size_t i = 0; i < N; i++)
                value[i] = str[i];
        }

        constexpr operator std::string_view() const {
            return std::string_view(&value[0], N);
        }

        template <size_t M>
        constexpr auto operator+(const FixedString<M>& other) const
        {
            FixedString<N+M> result;
            for (size_t i = 0; i < N; i++)
                result.value[i] = value[i];
            for (size_t i = 0; i < M; i++)
                result.value[N + i] = other.value[i];
            return result;
        }

        constexpr friend std::ostream& operator <<(std::ostream& os, const FixedString& fmt) {
            return os << std::string_view(fmt);
        }

    };

    FixedString() -> FixedString<0>;

    template <size_t N>
    FixedString(const char (&)[N]) -> FixedString<N-1>;

/********** Generic Formatting *************************************************************/

    template <FixedString Prefix, FixedString Suffix>
    struct Delimiter
    {
        using string = std::string;

        static constexpr size_t Padding = sizeof(Prefix) + sizeof(Suffix);
        
        constexpr string operator ()(std::string_view str) const
        {
            string result;
            result.reserve(str.size() + Padding);
            result += Prefix;
            result += str;
            result += Suffix;
            return result;
        }

        // For use with += and +
        constexpr operator string() const
        {
            string result;
            result.reserve(sizeof(Prefix));
            result += Prefix;
            return result;
        }

        // Support prepending: Bold + "text"
        constexpr string operator +(std::string_view str) const
        {
            string result;
            result.reserve(str.size() + sizeof(Prefix));
            result += Prefix;
            result += str;
            return result;
        }

        constexpr friend std::ostream& operator <<(std::ostream& os, const Delimiter& fmt) {
            return os << Prefix;
        }

        constexpr Delimiter<Suffix, ""> operator !(void) const {
            return {};
        }
    };

    // Support appending: "text" + Reset
    template <auto... Args>
    constexpr std::string operator+(const std::string& left, const Delimiter<Args...>& right) {
        return left + std::string(right);
    }

    // ANSI Escape Codes
    constexpr FixedString CSI = "\e[";
    constexpr FixedString SGR = "m";

    // Generic ANSI Formatter
    template <FixedString Begin, FixedString End>
    struct Format : Delimiter<CSI+Begin+SGR, CSI+End+SGR> {};
    

/********** Colors *************************************************************/

    inline namespace colors
    {
        template <
            FixedString Code,
            FixedString FG = "3",       // 30-39   (foreground - default)
            FixedString BG = "4",       // 40-49   (background)
            FixedString Bright = "9",   // 90-97   (bright foreground)
            FixedString BrightBG = "10" // 100-107 (bright background)
        >
        struct Color : Format<FG+Code, "39">
        {
            // Terminating codes are hardcoded
            Format<BG+Code, "49">       bg;
            Format<Bright+Code, "39">   bright;
            Format<BrightBG+Code, "49"> bgBright;
        };

        constexpr Color Default = Color<"9">();
        constexpr Color Black   = Color<"0">();
        constexpr Color Red     = Color<"1">();
        constexpr Color Green   = Color<"2">();
        constexpr Color Yellow  = Color<"3">();
        constexpr Color Blue    = Color<"4">();
        constexpr Color Magenta = Color<"5">(), Purple = Magenta;
        constexpr Color Cyan    = Color<"6">();
        constexpr Color White   = Color<"7">();

        // Gray is bright black
        constexpr Color Gray    = Color<"0", "9", "10">(), Grey = Gray;

        DEFINE_UDL(_black, Black);
        DEFINE_UDL(_red, Red);
        DEFINE_UDL(_green, Green);
        DEFINE_UDL(_yellow, Yellow);
        DEFINE_UDL(_blue, Blue);
        DEFINE_UDL(_magenta, Magenta);
            DEFINE_UDL(_purple, Purple);
        DEFINE_UDL(_cyan, Cyan);
        DEFINE_UDL(_white, White);
        DEFINE_UDL(_gray, Gray);
            DEFINE_UDL(_grey, Grey);
    }

/********** Text Formatting *************************************************************/

    inline namespace fmt
    {
        constexpr Format Reset      = Format<"0", "0">();
        constexpr Format End        = Reset;
        constexpr Format Normal     = Reset;
        constexpr Format Bold       = Format<"1", "22">();
        constexpr Format Dim        = Format<"2", "22">(), Faint = Dim;
        constexpr Format Italic     = Format<"3", "23">();
        constexpr Format Underline  = Format<"4", "24">();
        constexpr Format Blink      = Format<"5", "25">(), SlowBlink = Blink;
        constexpr Format RapidBlink = Format<"6", "25">();
        constexpr Format Inverse    = Format<"7", "27">(), Reverse = Inverse, Invert = Inverse;
        constexpr Format Conceal    = Format<"8", "28">(), Hide = Conceal, Hidden = Conceal;
        constexpr Format Strike     = Format<"9", "29">(), CrossedOut = Strike, Strikethrough = Strike;
        
        constexpr Format DefaultFont = Format<"10", "10">();

        constexpr Format DoubleUnderline = Format<"21", "24">();
        constexpr Format Overlined   = Format<"53", "55">();

        DEFINE_UDL(_b, Bold);
        DEFINE_UDL(_u, Underline);
        DEFINE_UDL(_i, Italic);

        DEFINE_UDL(_reset, Reset);
        DEFINE_UDL(_bold, Bold);
        DEFINE_UDL(_dim, Dim);
        DEFINE_UDL(_faint, Faint);
        DEFINE_UDL(_italic, Italic);
        DEFINE_UDL(_underline, Underline);
        DEFINE_UDL(_blink, Blink);
        DEFINE_UDL(_slowBlink, SlowBlink);
        DEFINE_UDL(_rapidBlink, RapidBlink);
        DEFINE_UDL(_inverse, Inverse);
        DEFINE_UDL(_reverse, Reverse);
        DEFINE_UDL(_conceal, Conceal);
        DEFINE_UDL(_strike, Strike);
        DEFINE_UDL(_crossedOut, CrossedOut);
        DEFINE_UDL(_strikethrough, Strikethrough);

        DEFINE_UDL(_uu, DoubleUnderline);
    }
}

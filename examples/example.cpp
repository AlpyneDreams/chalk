
#include <stdio.h>
#include <string>

#include "chalk.h"

using namespace chalk;

int main(int argc, char* argv[])
{
    std::string str = "\n ";

    // User defined literals for formatting
    str += "Formatting"_u + "\n  ";
    str += "regular " + "bold "_bold + "dim "_dim + "italic "_i;
    str += "underline"_u + " " + "double"_uu + " " + Overlined("overline") + "\n  ";
    str += "inverse"_inverse + " blink "_blink + "strikethrough"_strike + " " + " conceal"_conceal + "\n\n";

    // Colors
    str += " " + "Colors\n"_u;
    auto colors = "  black "_black  + "red "_red    + "green "_green
                + "yellow "_yellow  + "blue "_blue  + "magenta "_magenta
                + "cyan "_cyan      + "white"_white + " gray"_gray "\n";
    str += colors;

    // Background colors
    auto bgColors = Conceal("  " + Black.bg("black") + " " + Red.bg("red") + " " + Green.bg("green")
                  + " " + Yellow.bg("yellow") + " " + Blue.bg("blue") + " " + Magenta.bg("magenta")
                  + " " + Cyan.bg("cyan") + " " + White.bg("white") + " " + Gray.bg("gray") + "\n");
    str += bgColors;

    // Bright colors
    str += "\n " + "Bright Colors\n"_u;
    str += Black.bright("  black ") + Red.bright("red ") + Green.bright("green ")
        + Yellow.bright("yellow ") + Blue.bright("blue ") + Magenta.bright("magenta ")
        + Cyan.bright("cyan ") + White.bright("white ") + Gray.bright("gray") + "\n";

    // Bright background colors
    str += Hide("  " + Black.bgBright("black") + " " + Red.bgBright("red") + " " + Green.bgBright("green")
        + " " + Yellow.bgBright("yellow") + " " + Blue.bgBright("blue") + " " + Magenta.bgBright("magenta")
        + " " + Cyan.bgBright("cyan") + " " + White.bgBright("white") + " " + Gray.bgBright("gray")) + "\n";

    // Dim colors
    str += "\n " + "Dim Colors\n"_u;
    str += Dim(colors);
    str += Dim(bgColors);
    
    // Reset
    str += Reset;

    puts(str.c_str());
    return 0;
}

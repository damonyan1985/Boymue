//
//  Style.h
//  core
//
//  Created by yanbo on 2022/8/19.
//

#ifndef Style_h
#define Style_h

#include "StringUtil.h"

namespace boymue {
namespace css {
class Color {
public:
    Color(ColorValue value = 0);
    Color(BMByte a, BMByte r, BMByte g, BMByte b);
    Color(const String& colorText);
    BMByte alpha() const;
    BMByte red() const;
    BMByte green() const;
    BMByte blue() const;
    ColorValue value() const;

private:
    ColorValue m_value;
};

class Style
{
public:
    Style();
    ~Style();

    // css color
    Color color;
    // css background-color
    Color bgColor;

    LayoutUnit left;
    LayoutUnit top;
    LayoutUnit width;
    LayoutUnit height;
};
}    
}

#endif
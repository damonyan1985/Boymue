// Layout style property
#include "Style.h"

namespace boymue {
namespace css {
Color::Color(ColorValue value) : m_value(value) {}
Color::Color(BMByte a, BMByte r, BMByte g, BMByte b) {
    m_value = ((((ColorValue)a) << 24) | (((ColorValue)r) << 16) | (((ColorValue)b) << 8) | b);
}
Color::Color(const String& colorText) {
    if (StringUtil::startWith(colorText, "#")) {
        if (colorText.size() == 9) {
            m_value = StringUtil::stringToInt(colorText.substr(1), 16);
        } else if (colorText.size() <= 7) {
            m_value = StringUtil::stringToInt(colorText.substr(1), 16) | 0xFF;
        } else {
            m_value = 0;
        }
    } else {
        m_value = 0;
    }
}
BMByte Color::alpha() const { return (m_value >> 24) & 0xFF; }
BMByte Color::red() const { return (m_value >> 16) & 0xFF; }
BMByte Color::green() const { return (m_value >> 8) & 0xFF; }
BMByte Color::blue() const { return m_value & 0xFF; }
ColorValue Color::value() const { return m_value;  }

Style::Style() {}
Style::~Style() {}
}
}
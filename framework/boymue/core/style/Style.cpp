// Layout style property
#include "Style.h"

namespace boymue {
namespace css {

namespace {

BMByte clampColorByte(float x) {
    if (x <= 0.f) {
        return 0;
    }
    if (x >= 255.f) {
        return 255;
    }
    return static_cast<BMByte>(x + 0.5f);
}

/// 解析 rgb(r,g,b) / rgba(r,g,b,a)，大小写不敏感；a 在 [0,1] 时按透明度比例，否则按 0–255。
bool parseRgbFunction(const String& colorText, ColorValue& out) {
    String s = colorText;
    StringUtil::trim(s);
    if (s.size() < 5) {
        return false;
    }

    bool isRgba = false;
    if (s.size() >= 5 && (s[0] == 'r' || s[0] == 'R') && (s[1] == 'g' || s[1] == 'G') &&
        (s[2] == 'b' || s[2] == 'B') && (s[3] == 'a' || s[3] == 'A') && s[4] == '(') {
        isRgba = true;
    } else if (s.size() >= 4 && (s[0] == 'r' || s[0] == 'R') && (s[1] == 'g' || s[1] == 'G') &&
               (s[2] == 'b' || s[2] == 'B') && s[3] == '(') {
        isRgba = false;
    } else {
        return false;
    }

    const size_t open = s.find('(');
    const size_t close = s.rfind(')');
    if (open == String::npos || close == String::npos || close <= open + 1) {
        return false;
    }

    String inner = s.substr(open + 1, close - open - 1);
    Vector<String> parts = StringUtil::split(inner, ",");
    if (parts.empty()) {
        return false;
    }
    for (String& p : parts) {
        StringUtil::trim(p);
    }

    const size_t need = isRgba ? 4 : 3;
    if (parts.size() < need) {
        return false;
    }

    const float rf = StringUtil::stringToFloat(parts[0]);
    const float gf = StringUtil::stringToFloat(parts[1]);
    const float bf = StringUtil::stringToFloat(parts[2]);
    const BMByte r = clampColorByte(rf);
    const BMByte g = clampColorByte(gf);
    const BMByte b = clampColorByte(bf);
    BMByte a = 255;
    if (isRgba) {
        const float af = StringUtil::stringToFloat(parts[3]);
        if (af >= 0.f && af <= 1.f) {
            a = clampColorByte(af * 255.f);
        } else {
            a = clampColorByte(af);
        }
    }

    out = (static_cast<ColorValue>(a) << 24) | (static_cast<ColorValue>(r) << 16) |
          (static_cast<ColorValue>(g) << 8) | static_cast<ColorValue>(b);
    return true;
}

}  // namespace

Color::Color(ColorValue value) : m_value(value) {}
Color::Color(BMByte r, BMByte g, BMByte b, BMByte a) {
    m_value = (static_cast<ColorValue>(a) << 24) | (static_cast<ColorValue>(r) << 16) |
              (static_cast<ColorValue>(g) << 8) | static_cast<ColorValue>(b);
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
    } else if (!parseRgbFunction(colorText, m_value)) {
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

void Style::inheritFrom(const Style& parent) {
    color = parent.color;
    fontSizePx = parent.fontSizePx;
    fontFamily = parent.fontFamily;
    fontWeight = parent.fontWeight;
    fontStyle = parent.fontStyle;
    lineHeightPx = parent.lineHeightPx;
    textAlign = parent.textAlign;
    visibility = parent.visibility;
    letterSpacing = parent.letterSpacing;
    wordSpacing = parent.wordSpacing;
    textUnderline = parent.textUnderline;
    textLineThrough = parent.textLineThrough;
    opacity = parent.opacity;
    borderTopLeftRadius = parent.borderTopLeftRadius;
    borderTopRightRadius = parent.borderTopRightRadius;
    borderBottomRightRadius = parent.borderBottomRightRadius;
    borderBottomLeftRadius = parent.borderBottomLeftRadius;
}
}
}
#include "StyleParser.h"

#include <cctype>
#include <cstdio>

#include "StyleEngine.h"
#include "Style.h"
#include "StringUtil.h"

namespace boymue {
namespace css {

namespace {

String stripCssComments(const String& css) {
    String out;
    size_t begin = 0;
    while (true) {
        size_t open = css.find("/*", begin);
        if (open == String::npos) {
            out.append(css.substr(begin));
            break;
        }
        out.append(css.substr(begin, open - begin));
        size_t close = css.find("*/", open + 2);
        if (close == String::npos) {
            begin = open + 2;
            break;
        }
        begin = close + 2;
    }
    return out;
}

static float parseLengthPx(const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    if (v.empty()) {
        return 0.f;
    }
    if (StringUtil::endWith(v, "px")) {
        return StringUtil::stringToFloat(v.substr(0, v.size() - 2));
    }
    return StringUtil::stringToFloat(v);
}

static Vector<float> parseSpaceSeparatedLengths(const String& s) {
    Vector<float> out;
    Vector<String> parts = StringUtil::split(s, " ");
    for (String p : parts) {
        StringUtil::trim(p);
        if (!p.empty()) {
            out.push_back(parseLengthPx(p));
        }
    }
    return out;
}

static void expandToFourCorners(const Vector<float>& in, float o[4]) {
    if (in.empty()) {
        o[0] = o[1] = o[2] = o[3] = 0.f;
        return;
    }
    if (in.size() >= 4) {
        o[0] = in[0];
        o[1] = in[1];
        o[2] = in[2];
        o[3] = in[3];
    } else if (in.size() == 3) {
        o[0] = in[0];
        o[1] = in[1];
        o[2] = in[2];
        o[3] = in[1];
    } else if (in.size() == 2) {
        o[0] = o[2] = in[0];
        o[1] = o[3] = in[1];
    } else {
        o[0] = o[1] = o[2] = o[3] = in[0];
    }
}

/// 对标 WebKit CSSParser::parseBorderWidth + parse4Values：简写展开为四条 longhand，不存 bw: 内部串。
static bool expandBorderWidthDeclaration(CSSRule* rule, const String& val) {
    String v = val;
    StringUtil::trim(v);
    Vector<String> parts = StringUtil::split(v, " ");
    Vector<String> tok;
    for (auto& p : parts) {
        StringUtil::trim(p);
        if (!p.empty()) {
            tok.push_back(p);
        }
    }
    if (tok.empty()) {
        return false;
    }
    auto& map = rule->declarations.propertyMap;
    if (tok.size() == 1) {
        CSSDeclarations::CSSProperty p;
        p.numVal = parseLengthPx(tok[0]);
        p.strVal.clear();
        map[StyleEngine::kBorderWidth] = p;
        return true;
    }
    map.erase(StyleEngine::kBorderWidth);
    float top = 0.f;
    float right = 0.f;
    float bottom = 0.f;
    float left = 0.f;
    if (tok.size() == 2) {
        top = bottom = parseLengthPx(tok[0]);
        left = right = parseLengthPx(tok[1]);
    } else if (tok.size() == 3) {
        top = parseLengthPx(tok[0]);
        left = right = parseLengthPx(tok[1]);
        bottom = parseLengthPx(tok[2]);
    } else {
        top = parseLengthPx(tok[0]);
        right = parseLengthPx(tok[1]);
        bottom = parseLengthPx(tok[2]);
        left = parseLengthPx(tok[3]);
    }
    auto putSide = [&](int id, float x) {
        CSSDeclarations::CSSProperty p;
        p.numVal = x;
        p.strVal.clear();
        map[id] = p;
    };
    putSide(StyleEngine::kBorderTopWidth, top);
    putSide(StyleEngine::kBorderRightWidth, right);
    putSide(StyleEngine::kBorderBottomWidth, bottom);
    putSide(StyleEngine::kBorderLeftWidth, left);
    return true;
}

static void assignCornerRadiusProp(CSSDeclarations::CSSProperty& prop, float h, float v, bool elliptical) {
    prop.numVal = h;
    prop.strVal.clear();
    if (!elliptical) {
        prop.radiusSecond = h;
        return;
    }
    prop.radiusSecond = v;
}

/// 对标 WebKit CSSParser::parseBorderRadius：简写展开为四角 longhand，不存 br: 内部串。
static bool expandBorderRadiusDeclaration(CSSRule* rule, const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    if (v.empty()) {
        return false;
    }
    const size_t slash = v.find('/');
    const String hPart = slash == String::npos ? v : v.substr(0, slash);
    String vPart = slash == String::npos ? String() : v.substr(slash + 1);
    StringUtil::trim(vPart);
    Vector<float> hTok = parseSpaceSeparatedLengths(hPart);
    float h4[4];
    expandToFourCorners(hTok, h4);
    const bool elliptical = !vPart.empty();
    float v4[4] = {0.f, 0.f, 0.f, 0.f};
    if (elliptical) {
        Vector<float> vTok = parseSpaceSeparatedLengths(vPart);
        expandToFourCorners(vTok, v4);
    }
    static const int kCornerIds[4] = {StyleEngine::kBorderTopLeftRadius, StyleEngine::kBorderTopRightRadius,
                                        StyleEngine::kBorderBottomRightRadius,
                                        StyleEngine::kBorderBottomLeftRadius};
    auto& map = rule->declarations.propertyMap;
    map.erase(StyleEngine::kBorderRadius);
    for (int i = 0; i < 4; ++i) {
        CSSDeclarations::CSSProperty corner;
        assignCornerRadiusProp(corner, h4[i], v4[i], elliptical);
        map[kCornerIds[i]] = corner;
    }
    return true;
}

static void parseCornerRadiusValue(const String& raw, CSSDeclarations::CSSProperty& prop) {
    String v = raw;
    StringUtil::trim(v);
    if (v.empty()) {
        return;
    }
    Vector<float> tok = parseSpaceSeparatedLengths(v);
    if (tok.empty()) {
        return;
    }
    prop.numVal = tok[0];
    prop.strVal.clear();
    if (tok.size() >= 2) {
        prop.radiusSecond = tok[1];
    } else {
        prop.radiusSecond = tok[0];
    }
}

static String toLowerAscii(String s) {
    for (char& c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

static void parseFontShorthand(const String& value, CSSDeclarations::CSSProperty& prop) {
    Vector<String> tok = StringUtil::split(value, " ");
    for (auto& t : tok) {
        StringUtil::trim(t);
        if (t.empty()) {
            continue;
        }
        if (StringUtil::endWith(t, "px")) {
            prop.numVal = StringUtil::stringToFloat(t.substr(0, t.size() - 2));
        } else {
            if (!prop.strVal.empty()) {
                prop.strVal.push_back(' ');
            }
            prop.strVal += t;
        }
    }
}

static float parseOpacityValue(const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    float x = StringUtil::stringToFloat(v);
    if (x < 0.f) {
        x = 0.f;
    }
    if (x > 1.f) {
        x = 1.f;
    }
    return x;
}

static void parseZIndex(const String& raw, CSSDeclarations::CSSProperty& prop) {
    String v = raw;
    StringUtil::trim(v);
    v = toLowerAscii(v);
    if (v == "auto") {
        prop.strVal = "auto";
        prop.numVal = 0.f;
    } else {
        prop.strVal.clear();
        prop.numVal = static_cast<float>(StringUtil::stringToInt(v));
    }
}

static float parseFontWeightToken(const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    v = toLowerAscii(v);
    if (v == "normal") {
        return 400.f;
    }
    if (v == "bold") {
        return 700.f;
    }
    if (v == "lighter" || v == "bolder") {
        return 400.f;
    }
    int w = StringUtil::stringToInt(v);
    if (w < 1) {
        return 400.f;
    }
    if (w > 1000) {
        return 1000.f;
    }
    return static_cast<float>(w);
}

static String extractUrl(const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    String vl = toLowerAscii(v);
    if (!StringUtil::startWith(vl, "url")) {
        return v;
    }
    size_t l = v.find('(');
    size_t r = v.rfind(')');
    if (l == String::npos || r == String::npos || r <= l) {
        return String();
    }
    String inner = v.substr(l + 1, r - l - 1);
    StringUtil::trim(inner);
    if (inner.size() >= 2 &&
        ((inner.front() == '"' && inner.back() == '"') ||
         (inner.front() == '\'' && inner.back() == '\''))) {
        return inner.substr(1, inner.size() - 2);
    }
    return inner;
}

static float parseLengthOrNormalPx(const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    if (toLowerAscii(v) == "normal" || v.empty()) {
        return 0.f;
    }
    return parseLengthPx(raw);
}

static void parseLineHeightDecl(const String& raw, CSSDeclarations::CSSProperty& prop) {
    String v = raw;
    StringUtil::trim(v);
    if (toLowerAscii(v) == "normal" || v.empty()) {
        prop.numVal = 0.f;
        prop.strVal.clear();
        return;
    }
    prop.numVal = parseLengthPx(raw);
    prop.strVal = "px";
}

static void parseMaxDimension(const String& val, CSSDeclarations::CSSProperty& prop) {
    String v = val;
    StringUtil::trim(v);
    if (toLowerAscii(v) == "none") {
        prop.strVal = "none";
        prop.numVal = 0.f;
    } else {
        prop.strVal.clear();
        prop.numVal = parseLengthPx(val);
    }
}

}  // namespace

StyleParser::StyleParser(StyleEngine* engine)
    : m_engine(engine) {}

StyleParser::~StyleParser() = default;

SharedPtr<StyleSheet> StyleParser::parse(const String& css) {
    if (css.empty() || !m_engine) {
        return nullptr;
    }

    String cssText = stripCssComments(css);

    SharedPtr<StyleSheet> sheet(new StyleSheet());
    size_t begin = 0;
    size_t ruleIndex = 0;

    while (true) {
        size_t open = cssText.find('{', begin);
        if (open == String::npos) {
            break;
        }
        size_t close = cssText.find('}', open);
        if (close == String::npos) {
            break;
        }

        SharedPtr<CSSRule> rule(new CSSRule());
        rule->sourceOrder = ruleIndex++;

        String selectorText = cssText.substr(begin, open - begin);
        String declarationsText = cssText.substr(open + 1, close - open - 1);

        Vector<String> selectors = StringUtil::split(selectorText, ",");
        for (auto& selText : selectors) {
            StringUtil::trim(selText);
            if (selText.empty()) {
                continue;
            }
            SharedPtr<CSSSelector> selector(new CSSSelector());
            Vector<String> filters = StringUtil::split(selText, " ");
            for (auto& f : filters) {
                StringUtil::trim(f);
                if (f.empty()) {
                    continue;
                }
                selector->filters.push_back(
                    SharedPtr<SimpleSelector>(new SimpleSelector(f)));
            }
            if (!selector->filters.empty()) {
                rule->selectors.push_back(selector);
            }
        }

        if (rule->selectors.empty()) {
            begin = close + 1;
            continue;
        }

        Vector<String> declarationTexts = StringUtil::split(declarationsText, ";");
        for (auto& decl : declarationTexts) {
            StringUtil::trim(decl);
            if (decl.empty()) {
                continue;
            }
            Vector<String> kv = StringUtil::split(decl, ":");
            if (kv.size() < 2) {
                continue;
            }
            String propName = kv[0];
            StringUtil::trim(propName);
            propName = toLowerAscii(propName);

            String propValue;
            for (size_t i = 1; i < kv.size(); ++i) {
                if (i > 1) {
                    propValue.push_back(':');
                }
                propValue += kv[i];
            }
            StringUtil::trim(propValue);

            Vector<String> pair;
            pair.push_back(propName);
            pair.push_back(propValue);
            addDeclaration(rule.get(), pair);
        }

        sheet->addRule(rule);
        begin = close + 1;
    }

    return sheet->empty() ? nullptr : sheet;
}

void StyleParser::addDeclaration(CSSRule* rule, Vector<String>& kv) {
    if (kv.size() < 2 || !m_engine) {
        return;
    }
    int type = m_engine->getType(kv[0]);
    if (type < 0) {
        return;
    }

    CSSDeclarations::CSSProperty prop;
    const String& val = kv[1];
    String valTrim = val;
    StringUtil::trim(valTrim);
    if (toLowerAscii(valTrim) == "inherit") {
        prop.strVal = "inherit";
        rule->declarations.propertyMap[type] = prop;
        return;
    }

    switch (type) {
    case StyleEngine::kWidth:
    case StyleEngine::kHeight:
    case StyleEngine::kMargin:
    case StyleEngine::kMarginTop:
    case StyleEngine::kMarginRight:
    case StyleEngine::kMarginBottom:
    case StyleEngine::kMarginLeft:
    case StyleEngine::kPadding:
    case StyleEngine::kPaddingTop:
    case StyleEngine::kPaddingRight:
    case StyleEngine::kPaddingBottom:
    case StyleEngine::kPaddingLeft:
    case StyleEngine::kFontSize:
    case StyleEngine::kLeft:
    case StyleEngine::kTop:
    case StyleEngine::kRight:
    case StyleEngine::kBottom:
    case StyleEngine::kMinWidth:
    case StyleEngine::kMinHeight:
    case StyleEngine::kBorderTopWidth:
    case StyleEngine::kBorderRightWidth:
    case StyleEngine::kBorderBottomWidth:
    case StyleEngine::kBorderLeftWidth:
        prop.numVal = parseLengthPx(val);
        break;
    case StyleEngine::kBorderRadius:
        if (expandBorderRadiusDeclaration(rule, val)) {
            return;
        }
        break;
    case StyleEngine::kBorderTopLeftRadius:
    case StyleEngine::kBorderTopRightRadius:
    case StyleEngine::kBorderBottomRightRadius:
    case StyleEngine::kBorderBottomLeftRadius:
        parseCornerRadiusValue(val, prop);
        break;
    case StyleEngine::kMaxWidth:
    case StyleEngine::kMaxHeight:
        parseMaxDimension(val, prop);
        break;
    case StyleEngine::kLetterSpacing:
    case StyleEngine::kWordSpacing:
        prop.numVal = parseLengthOrNormalPx(val);
        break;
    case StyleEngine::kLineHeight:
        parseLineHeightDecl(val, prop);
        break;
    case StyleEngine::kOpacity:
        prop.numVal = parseOpacityValue(val);
        break;
    case StyleEngine::kZIndex:
        parseZIndex(val, prop);
        break;
    case StyleEngine::kFontWeight:
        prop.numVal = parseFontWeightToken(val);
        break;
    case StyleEngine::kBorderWidth:
        if (expandBorderWidthDeclaration(rule, val)) {
            return;
        }
        break;
    case StyleEngine::kColor:
    case StyleEngine::kBackgroundColor:
    case StyleEngine::kBorderColor: {
        String v = val;
        StringUtil::trim(v);
        if (v.empty()) {
            return;
        }
        Color parsed(v);
        prop.intVal = parsed.value();
        break;
    }
    case StyleEngine::kFontFamily:
        prop.strVal = val;
        break;
    case StyleEngine::kPosition:
    case StyleEngine::kVisibility:
    case StyleEngine::kDisplay:
    case StyleEngine::kFontStyle:
    case StyleEngine::kTextAlign:
    case StyleEngine::kTextDecoration:
    case StyleEngine::kOverflow:
    case StyleEngine::kOverflowX:
    case StyleEngine::kOverflowY: {
        String v = val;
        StringUtil::trim(v);
        prop.strVal = toLowerAscii(v);
        break;
    }
    case StyleEngine::kBackgroundImage: {
        String v = val;
        StringUtil::trim(v);
        if (toLowerAscii(v) == "none") {
            prop.strVal.clear();
        } else {
            prop.strVal = extractUrl(val);
        }
        break;
    }
    case StyleEngine::kFont:
        parseFontShorthand(val, prop);
        break;
    default:
        break;
    }

    rule->declarations.propertyMap[type] = prop;
}

}  // namespace css
}  // namespace boymue

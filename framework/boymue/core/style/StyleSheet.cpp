#include "StyleSheet.h"

#include <algorithm>
#include <cctype>

#include "DocumentElement.h"
#include "Layout.h"
#include "Style.h"
#include "StyleEngine.h"
#include "StringUtil.h"

namespace boymue {
namespace css {

namespace {

bool tokenHasClass(const String& classes, const String& cls) {
    if (classes.empty() || cls.empty()) {
        return false;
    }
    Vector<String> parts = StringUtil::split(classes, " ");
    for (const auto& p : parts) {
        if (p == cls) {
            return true;
        }
    }
    return false;
}

float lengthPxFromToken(const String& tok) {
    String t = tok;
    StringUtil::trim(t);
    if (StringUtil::endWith(t, "px")) {
        return StringUtil::stringToFloat(t.substr(0, t.size() - 2));
    }
    return StringUtil::stringToFloat(t);
}

void applyBorderWidthShorthand(Style& st, const CSSDeclarations::CSSProperty& prop) {
    if (!prop.strVal.empty() && StringUtil::startWith(prop.strVal, "bw:")) {
        String inner = prop.strVal.substr(3);
        Vector<String> parts = StringUtil::split(inner, " ");
        Vector<String> tok;
        for (auto& p : parts) {
            StringUtil::trim(p);
            if (!p.empty()) {
                tok.push_back(p);
            }
        }
        if (tok.empty()) {
            return;
        }
        if (tok.size() == 1) {
            LayoutUnit x = lengthPxFromToken(tok[0]);
            st.borderTopWidth = st.borderRightWidth = st.borderBottomWidth = st.borderLeftWidth = x;
        } else if (tok.size() == 2) {
            LayoutUnit a = lengthPxFromToken(tok[0]);
            LayoutUnit b = lengthPxFromToken(tok[1]);
            st.borderTopWidth = st.borderBottomWidth = a;
            st.borderLeftWidth = st.borderRightWidth = b;
        } else if (tok.size() == 3) {
            st.borderTopWidth = lengthPxFromToken(tok[0]);
            st.borderLeftWidth = st.borderRightWidth = lengthPxFromToken(tok[1]);
            st.borderBottomWidth = lengthPxFromToken(tok[2]);
        } else {
            st.borderTopWidth = lengthPxFromToken(tok[0]);
            st.borderRightWidth = lengthPxFromToken(tok[1]);
            st.borderBottomWidth = lengthPxFromToken(tok[2]);
            st.borderLeftWidth = lengthPxFromToken(tok[3]);
        }
        return;
    }
    LayoutUnit x = prop.numVal;
    st.borderTopWidth = st.borderRightWidth = st.borderBottomWidth = st.borderLeftWidth = x;
}

void applyTextDecoration(Style& st, const String& raw) {
    String v = raw;
    StringUtil::trim(v);
    for (char& c : v) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    st.textUnderline = false;
    st.textLineThrough = false;
    if (v == "none") {
        return;
    }
    Vector<String> tok = StringUtil::split(v, " ");
    for (String t : tok) {
        StringUtil::trim(t);
        if (t == "underline") {
            st.textUnderline = true;
        }
        if (t == "line-through") {
            st.textLineThrough = true;
        }
    }
}

PositionValue parsePositionKeyword(const String& s) {
    if (s == "relative") {
        return PositionValue::Relative;
    }
    if (s == "absolute") {
        return PositionValue::Absolute;
    }
    if (s == "fixed") {
        return PositionValue::Fixed;
    }
    if (s == "sticky") {
        return PositionValue::Relative;
    }
    return PositionValue::Static;
}

DisplayValue parseDisplayKeyword(const String& s) {
    if (s == "none") {
        return DisplayValue::None;
    }
    if (s == "block") {
        return DisplayValue::Block;
    }
    if (s == "inline-block") {
        return DisplayValue::InlineBlock;
    }
    if (s == "flex") {
        return DisplayValue::Flex;
    }
    if (s == "inline") {
        return DisplayValue::Inline;
    }
    return DisplayValue::Block;
}

VisibilityValue parseVisibilityKeyword(const String& s) {
    if (s == "hidden" || s == "collapse") {
        return VisibilityValue::Hidden;
    }
    return VisibilityValue::Visible;
}

TextAlignValue parseTextAlignKeyword(const String& s) {
    if (s == "center") {
        return TextAlignValue::Center;
    }
    if (s == "right" || s == "end") {
        return TextAlignValue::Right;
    }
    if (s == "left") {
        return TextAlignValue::Left;
    }
    if (s == "start") {
        return TextAlignValue::Start;
    }
    if (s == "justify") {
        return TextAlignValue::Justify;
    }
    return TextAlignValue::Start;
}

OverflowValue parseOverflowKeyword(const String& s) {
    if (s == "hidden") {
        return OverflowValue::Hidden;
    }
    if (s == "scroll") {
        return OverflowValue::Scroll;
    }
    if (s == "auto") {
        return OverflowValue::Auto;
    }
    return OverflowValue::Visible;
}

FontStyleValue parseFontStyleKeyword(const String& s) {
    if (s == "italic") {
        return FontStyleValue::Italic;
    }
    if (s == "oblique") {
        return FontStyleValue::Oblique;
    }
    return FontStyleValue::Normal;
}

void applyPropertyToStyle(int propId, const CSSDeclarations::CSSProperty& prop, Style& st) {
    switch (propId) {
    case StyleEngine::kWidth:
        if (prop.numVal > 0.f) {
            st.width = prop.numVal;
        }
        break;
    case StyleEngine::kHeight:
        if (prop.numVal > 0.f) {
            st.height = prop.numVal;
        }
        break;
    case StyleEngine::kColor:
        if (!prop.strVal.empty()) {
            st.color = Color(prop.strVal);
        }
        break;
    case StyleEngine::kBackgroundColor:
        if (!prop.strVal.empty()) {
            st.bgColor = Color(prop.strVal);
        }
        break;
    case StyleEngine::kFontSize:
        if (prop.numVal > 0.f) {
            st.fontSizePx = static_cast<int>(prop.numVal);
        }
        break;
    case StyleEngine::kFontFamily:
        if (!prop.strVal.empty()) {
            st.fontFamily = prop.strVal;
        }
        break;
    case StyleEngine::kMargin:
    case StyleEngine::kMarginTop:
    case StyleEngine::kMarginRight:
    case StyleEngine::kMarginBottom:
    case StyleEngine::kMarginLeft: {
        LayoutUnit v = prop.numVal;
        if (propId == StyleEngine::kMargin) {
            st.marginTop = st.marginRight = st.marginBottom = st.marginLeft = v;
        } else if (propId == StyleEngine::kMarginTop) {
            st.marginTop = v;
        } else if (propId == StyleEngine::kMarginRight) {
            st.marginRight = v;
        } else if (propId == StyleEngine::kMarginBottom) {
            st.marginBottom = v;
        } else if (propId == StyleEngine::kMarginLeft) {
            st.marginLeft = v;
        }
        break;
    }
    case StyleEngine::kPadding:
    case StyleEngine::kPaddingTop:
    case StyleEngine::kPaddingRight:
    case StyleEngine::kPaddingBottom:
    case StyleEngine::kPaddingLeft: {
        LayoutUnit v = prop.numVal;
        if (propId == StyleEngine::kPadding) {
            st.paddingTop = st.paddingRight = st.paddingBottom = st.paddingLeft = v;
        } else if (propId == StyleEngine::kPaddingTop) {
            st.paddingTop = v;
        } else if (propId == StyleEngine::kPaddingRight) {
            st.paddingRight = v;
        } else if (propId == StyleEngine::kPaddingBottom) {
            st.paddingBottom = v;
        } else if (propId == StyleEngine::kPaddingLeft) {
            st.paddingLeft = v;
        }
        break;
    }
    case StyleEngine::kFont:
        if (prop.numVal > 0.f) {
            st.fontSizePx = static_cast<int>(prop.numVal);
        }
        if (!prop.strVal.empty()) {
            st.fontFamily = prop.strVal;
        }
        break;
    case StyleEngine::kLeft:
        st.left = prop.numVal;
        break;
    case StyleEngine::kTop:
        st.top = prop.numVal;
        break;
    case StyleEngine::kRight:
        st.right = prop.numVal;
        break;
    case StyleEngine::kBottom:
        st.bottom = prop.numVal;
        break;
    case StyleEngine::kPosition:
        st.position = parsePositionKeyword(prop.strVal);
        break;
    case StyleEngine::kZIndex:
        if (prop.strVal == "auto") {
            st.zIndexAuto = true;
        } else {
            st.zIndexAuto = false;
            st.zIndex = static_cast<int>(prop.numVal);
        }
        break;
    case StyleEngine::kMinWidth:
        st.minWidth = prop.numVal;
        break;
    case StyleEngine::kMinHeight:
        st.minHeight = prop.numVal;
        break;
    case StyleEngine::kMaxWidth:
        if (prop.strVal == "none") {
            st.maxWidth = 0;
        } else {
            st.maxWidth = prop.numVal;
        }
        break;
    case StyleEngine::kMaxHeight:
        if (prop.strVal == "none") {
            st.maxHeight = 0;
        } else {
            st.maxHeight = prop.numVal;
        }
        break;
    case StyleEngine::kOpacity:
        st.opacity = prop.numVal;
        break;
    case StyleEngine::kVisibility:
        st.visibility = parseVisibilityKeyword(prop.strVal);
        break;
    case StyleEngine::kDisplay:
        st.display = parseDisplayKeyword(prop.strVal);
        break;
    case StyleEngine::kFontWeight:
        st.fontWeight = static_cast<int>(prop.numVal);
        break;
    case StyleEngine::kFontStyle:
        st.fontStyle = parseFontStyleKeyword(prop.strVal);
        break;
    case StyleEngine::kLineHeight:
        if (prop.strVal == "px") {
            st.lineHeightPx = prop.numVal;
        } else {
            st.lineHeightPx = 0;
        }
        break;
    case StyleEngine::kTextAlign:
        st.textAlign = parseTextAlignKeyword(prop.strVal);
        break;
    case StyleEngine::kTextDecoration:
        applyTextDecoration(st, prop.strVal);
        break;
    case StyleEngine::kLetterSpacing:
        st.letterSpacing = prop.numVal;
        break;
    case StyleEngine::kWordSpacing:
        st.wordSpacing = prop.numVal;
        break;
    case StyleEngine::kBorderWidth:
        applyBorderWidthShorthand(st, prop);
        break;
    case StyleEngine::kBorderTopWidth:
        st.borderTopWidth = prop.numVal;
        break;
    case StyleEngine::kBorderRightWidth:
        st.borderRightWidth = prop.numVal;
        break;
    case StyleEngine::kBorderBottomWidth:
        st.borderBottomWidth = prop.numVal;
        break;
    case StyleEngine::kBorderLeftWidth:
        st.borderLeftWidth = prop.numVal;
        break;
    case StyleEngine::kBorderColor: {
        String c = prop.strVal;
        StringUtil::trim(c);
        if (!c.empty()) {
            st.borderColor = Color(c);
        }
        break;
    }
    case StyleEngine::kBorderRadius:
        st.borderRadius = prop.numVal;
        break;
    case StyleEngine::kOverflow: {
        OverflowValue o = parseOverflowKeyword(prop.strVal);
        st.overflowX = o;
        st.overflowY = o;
        break;
    }
    case StyleEngine::kOverflowX:
        st.overflowX = parseOverflowKeyword(prop.strVal);
        break;
    case StyleEngine::kOverflowY:
        st.overflowY = parseOverflowKeyword(prop.strVal);
        break;
    case StyleEngine::kBackgroundImage:
        st.backgroundImage = prop.strVal;
        break;
    default:
        break;
    }
}

}  // namespace

SimpleSelector::SimpleSelector(const String& rawText) {
    String t = rawText;
    StringUtil::trim(t);
    if (t.empty()) {
        m_kind = SelectorKind::Type;
        return;
    }
    if (t == "*") {
        m_kind = SelectorKind::Universal;
        return;
    }
    if (t[0] == '.') {
        m_kind = SelectorKind::Class;
        m_name = t.substr(1);
        StringUtil::trim(m_name);
        return;
    }
    if (t[0] == '#') {
        m_kind = SelectorKind::Id;
        m_name = t.substr(1);
        StringUtil::trim(m_name);
        return;
    }
    m_kind = SelectorKind::Type;
    m_name = t;
}

int SimpleSelector::specificityWeight() const {
    switch (m_kind) {
    case SelectorKind::Universal:
        return 0;
    case SelectorKind::Type:
        return 1;
    case SelectorKind::Class:
        return 10;
    case SelectorKind::Id:
        return 100;
    }
    return 0;
}

bool SimpleSelector::matchesElement(const dom::DocumentElement* el) const {
    if (!el) {
        return false;
    }
    switch (m_kind) {
    case SelectorKind::Universal:
        return true;
    case SelectorKind::Type:
        return el->tagName() == m_name;
    case SelectorKind::Class:
        return tokenHasClass(el->getProperty("class"), m_name);
    case SelectorKind::Id:
        return el->styleId() == m_name || el->getProperty("id") == m_name;
    }
    return false;
}

bool CSSSelector::matches(dom::DocumentElement* el) const {
    if (filters.empty() || !el) {
        return false;
    }
    dom::DocumentElement* cur = el;
    for (int i = static_cast<int>(filters.size()) - 1; i >= 0; --i) {
        const SimpleSelector* sel = filters[static_cast<size_t>(i)].get();
        if (!sel) {
            return false;
        }
        bool found = false;
        while (cur) {
            if (sel->matchesElement(cur)) {
                found = true;
                break;
            }
            cur = cur->parent();
        }
        if (!found) {
            return false;
        }
        if (i > 0) {
            cur = cur->parent();
        }
    }
    return true;
}

int CSSSelector::specificity() const {
    int s = 0;
    for (const auto& f : filters) {
        if (f) {
            s += f->specificityWeight();
        }
    }
    return s;
}

int CSSRule::matchSpecificity(dom::DocumentElement* el, const CSSSelector** matchedOut) const {
    int best = -1;
    const CSSSelector* bestPtr = nullptr;
    for (const auto& sel : selectors) {
        if (sel && sel->matches(el)) {
            int sp = sel->specificity();
            if (sp > best) {
                best = sp;
                bestPtr = sel.get();
            }
        }
    }
    if (matchedOut) {
        *matchedOut = bestPtr;
    }
    return best;
}

StyleSheet::StyleSheet() = default;

StyleSheet::~StyleSheet() = default;

void StyleSheet::addRule(SharedPtr<CSSRule> rule) {
    if (rule) {
        m_ruleList.push_back(rule);
    }
}

bool StyleSheet::empty() const {
    return m_ruleList.empty();
}

void StyleSheet::apply() {}

struct RankedRule {
    CSSRule* rule{nullptr};
    int specificity{0};
    size_t order{0};
};

void StyleResolver::applyToElement(dom::DocumentElement* el,
                                     const Vector<SharedPtr<StyleSheet>>& sheets) {
    if (!el) {
        return;
    }
    Vector<RankedRule> ranked;
    for (const auto& sheet : sheets) {
        if (!sheet) {
            continue;
        }
        for (const auto& rulePtr : sheet->rules()) {
            if (!rulePtr) {
                continue;
            }
            CSSRule* rule = rulePtr.get();
            int sp = rule->matchSpecificity(el, nullptr);
            if (sp >= 0) {
                ranked.push_back(RankedRule{rule, sp, rule->sourceOrder});
            }
        }
    }
    std::sort(ranked.begin(), ranked.end(), [](const RankedRule& a, const RankedRule& b) {
        if (a.specificity != b.specificity) {
            return a.specificity < b.specificity;
        }
        return a.order < b.order;
    });

    layout::Layout* lay = el->createLayout();
    if (!lay) {
        return;
    }
    Style& st = lay->mutableStyle();
    for (const auto& rr : ranked) {
        if (!rr.rule) {
            continue;
        }
        for (const auto& kv : rr.rule->declarations.propertyMap) {
            applyPropertyToStyle(kv.first, kv.second, st);
        }
    }
    lay->syncMetricsFromStyle();
}

void StyleResolver::walk(dom::DocumentElement* el,
                         const Vector<SharedPtr<StyleSheet>>& sheets) {
    if (!el) {
        return;
    }
    applyToElement(el, sheets);
    el->visitChildren([&sheets](dom::DocumentElement* child) {
        walk(child, sheets);
    });
}

void StyleResolver::applyToSubtree(dom::DocumentElement* root,
                                   const Vector<SharedPtr<StyleSheet>>& sheets) {
    if (!root) {
        return;
    }
    walk(root, sheets);
}

}  // namespace css
}  // namespace boymue

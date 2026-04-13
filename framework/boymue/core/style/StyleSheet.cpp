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

static bool inheritKeywordMatch(const String& s) {
    String t = s;
    StringUtil::trim(t);
    for (char& c : t) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return t == "inherit";
}

/// 向上查找最近已挂 layout 的祖先，取其 `Layout::style()` 作为父计算样式（中间无 layout 的节点跳过）。
static const Style* nearestAncestorComputedStyle(dom::DocumentElement* el) {
    if (!el) {
        return nullptr;
    }
    for (dom::DocumentElement* p = el->parent(); p; p = p->parent()) {
        layout::Layout* pl = p->layout();
        if (pl) {
            return &pl->style();
        }
    }
    return nullptr;
}

/// `inherit` 关键字：显式使用父元素该属性的计算值（CSS 任意属性均可 `inherit`）。
static void copyPropertyFromParentForInherit(int propId, Style& st, const Style& p) {
    switch (propId) {
    case StyleEngine::kWidth:
        st.width = p.width;
        break;
    case StyleEngine::kHeight:
        st.height = p.height;
        break;
    case StyleEngine::kColor:
    case StyleEngine::kBackgroundColor:
    case StyleEngine::kBorderColor:
        if (propId == StyleEngine::kColor) {
            st.color = p.color;
        } else if (propId == StyleEngine::kBackgroundColor) {
            st.bgColor = p.bgColor;
        } else {
            st.borderColor = p.borderColor;
        }
        break;
    case StyleEngine::kFontSize:
        st.fontSizePx = p.fontSizePx;
        break;
    case StyleEngine::kFontFamily:
        st.fontFamily = p.fontFamily;
        break;
    case StyleEngine::kMargin:
        st.marginTop = p.marginTop;
        st.marginRight = p.marginRight;
        st.marginBottom = p.marginBottom;
        st.marginLeft = p.marginLeft;
        break;
    case StyleEngine::kMarginTop:
        st.marginTop = p.marginTop;
        break;
    case StyleEngine::kMarginRight:
        st.marginRight = p.marginRight;
        break;
    case StyleEngine::kMarginBottom:
        st.marginBottom = p.marginBottom;
        break;
    case StyleEngine::kMarginLeft:
        st.marginLeft = p.marginLeft;
        break;
    case StyleEngine::kPadding:
        st.paddingTop = p.paddingTop;
        st.paddingRight = p.paddingRight;
        st.paddingBottom = p.paddingBottom;
        st.paddingLeft = p.paddingLeft;
        break;
    case StyleEngine::kPaddingTop:
        st.paddingTop = p.paddingTop;
        break;
    case StyleEngine::kPaddingRight:
        st.paddingRight = p.paddingRight;
        break;
    case StyleEngine::kPaddingBottom:
        st.paddingBottom = p.paddingBottom;
        break;
    case StyleEngine::kPaddingLeft:
        st.paddingLeft = p.paddingLeft;
        break;
    case StyleEngine::kFont:
        st.fontSizePx = p.fontSizePx;
        st.fontFamily = p.fontFamily;
        st.fontWeight = p.fontWeight;
        st.fontStyle = p.fontStyle;
        break;
    case StyleEngine::kLeft:
        st.left = p.left;
        break;
    case StyleEngine::kTop:
        st.top = p.top;
        break;
    case StyleEngine::kRight:
        st.right = p.right;
        break;
    case StyleEngine::kBottom:
        st.bottom = p.bottom;
        break;
    case StyleEngine::kPosition:
        st.position = p.position;
        break;
    case StyleEngine::kZIndex:
        st.zIndexAuto = p.zIndexAuto;
        st.zIndex = p.zIndex;
        break;
    case StyleEngine::kMinWidth:
        st.minWidth = p.minWidth;
        break;
    case StyleEngine::kMinHeight:
        st.minHeight = p.minHeight;
        break;
    case StyleEngine::kMaxWidth:
        st.maxWidth = p.maxWidth;
        break;
    case StyleEngine::kMaxHeight:
        st.maxHeight = p.maxHeight;
        break;
    case StyleEngine::kOpacity:
        st.opacity = p.opacity;
        break;
    case StyleEngine::kVisibility:
        st.visibility = p.visibility;
        break;
    case StyleEngine::kDisplay:
        st.display = p.display;
        break;
    case StyleEngine::kFontWeight:
        st.fontWeight = p.fontWeight;
        break;
    case StyleEngine::kFontStyle:
        st.fontStyle = p.fontStyle;
        break;
    case StyleEngine::kLineHeight:
        st.lineHeightPx = p.lineHeightPx;
        break;
    case StyleEngine::kTextAlign:
        st.textAlign = p.textAlign;
        break;
    case StyleEngine::kTextDecoration:
        st.textUnderline = p.textUnderline;
        st.textLineThrough = p.textLineThrough;
        break;
    case StyleEngine::kLetterSpacing:
        st.letterSpacing = p.letterSpacing;
        break;
    case StyleEngine::kWordSpacing:
        st.wordSpacing = p.wordSpacing;
        break;
    case StyleEngine::kBorderWidth:
        st.borderTopWidth = p.borderTopWidth;
        st.borderRightWidth = p.borderRightWidth;
        st.borderBottomWidth = p.borderBottomWidth;
        st.borderLeftWidth = p.borderLeftWidth;
        break;
    case StyleEngine::kBorderTopWidth:
        st.borderTopWidth = p.borderTopWidth;
        break;
    case StyleEngine::kBorderRightWidth:
        st.borderRightWidth = p.borderRightWidth;
        break;
    case StyleEngine::kBorderBottomWidth:
        st.borderBottomWidth = p.borderBottomWidth;
        break;
    case StyleEngine::kBorderLeftWidth:
        st.borderLeftWidth = p.borderLeftWidth;
        break;
    case StyleEngine::kBorderRadius:
        st.borderRadius = p.borderRadius;
        break;
    case StyleEngine::kOverflow:
        st.overflowX = p.overflowX;
        st.overflowY = p.overflowY;
        break;
    case StyleEngine::kOverflowX:
        st.overflowX = p.overflowX;
        break;
    case StyleEngine::kOverflowY:
        st.overflowY = p.overflowY;
        break;
    case StyleEngine::kBackgroundImage:
        st.backgroundImage = p.backgroundImage;
        break;
    default:
        break;
    }
}

void applyPropertyToStyle(int propId, const CSSDeclarations::CSSProperty& prop, Style& st,
                          const Style* parentStyle) {
    if (parentStyle && inheritKeywordMatch(prop.strVal)) {
        copyPropertyFromParentForInherit(propId, st, *parentStyle);
        return;
    }
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
    case StyleEngine::kBackgroundColor:
    case StyleEngine::kBorderColor: {
        Color c(prop.intVal);
        if (propId == StyleEngine::kColor) {
            st.color = c;
        } else if (propId == StyleEngine::kBackgroundColor) {
            st.bgColor = c;
        } else {
            st.borderColor = c;
        }
        break;
    }
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
    const Style* parentStyle = nearestAncestorComputedStyle(el);
    Style& st = lay->mutableStyle();
    if (parentStyle) {
        st.inheritFrom(*parentStyle);
    }
    for (const auto& rr : ranked) {
        if (!rr.rule) {
            continue;
        }
        for (const auto& kv : rr.rule->declarations.propertyMap) {
            applyPropertyToStyle(kv.first, kv.second, st, parentStyle);
        }
    }
    lay->syncMetricsFromStyle();
    lay->invalidatePainter();
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

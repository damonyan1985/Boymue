//
//  StyleSheet.h
//  core
//
//  Created by yanbo on 2022/8/19.
//

#ifndef StyleSheet_h
#define StyleSheet_h

#include "StringUtil.h"

namespace boymue {
namespace dom {
class DocumentElement;
}

namespace css {
class StyleEngine;

/// 简单选择器种类（对标 WebKit/CSS 基本选择器，实现为最小子集）
enum class SelectorKind {
    Universal,
    Type,
    Class,
    Id,
};

/// 单个简单选择器（如 view、.cls、#id、*）
class SimpleSelector {
public:
    explicit SimpleSelector(const String& rawText);
    SelectorKind kind() const { return m_kind; }
    const String& name() const { return m_name; }
    int specificityWeight() const;

    bool matchesElement(const dom::DocumentElement* el) const;

private:
    SelectorKind m_kind{SelectorKind::Type};
    String m_name;
};

/// 由空格分隔的「后代」链，如 "view .item" 表示 .item 且祖先为 view
class CSSSelector {
public:
    Vector<SharedPtr<SimpleSelector>> filters;

    bool matches(dom::DocumentElement* el) const;
    int specificity() const;
};

class CSSDeclarations {
public:
    CSSDeclarations() = default;
    ~CSSDeclarations() = default;

    struct CSSProperty {
        /// 数值型声明（长度、字号等），与 LayoutUnit 一致为 float
        float numVal{0};
        String strVal;
    };

    HashMap<int, CSSProperty> propertyMap;
};

using CSSSelectorGroup = Vector<SharedPtr<CSSSelector>>;

class CSSRule {
public:
    CSSRule() = default;
    ~CSSRule() = default;

    CSSSelectorGroup selectors;
    CSSDeclarations declarations;
    /// 在样式表中出现的先后顺序，用于层叠
    size_t sourceOrder{0};

    /// 若匹配返回该选择器特异性，否则 -1；可选返回匹配到的选择器指针
    int matchSpecificity(dom::DocumentElement* el, const CSSSelector** matchedOut) const;
};

class StyleSheet {
public:
    StyleSheet();
    ~StyleSheet();

    bool empty() const;
    void addRule(SharedPtr<CSSRule> rule);
    const List<SharedPtr<CSSRule>>& rules() const { return m_ruleList; }

    void apply();

private:
    List<SharedPtr<CSSRule>> m_ruleList;
};

/// 将已解析的样式表按选择器与层叠规则应用到 DOM 子树（对标 WebCore 中 Style 应用阶段的最小实现）
class StyleResolver {
public:
    static void applyToSubtree(dom::DocumentElement* root,
                               const Vector<SharedPtr<StyleSheet>>& sheets);

private:
    static void applyToElement(dom::DocumentElement* el,
                               const Vector<SharedPtr<StyleSheet>>& sheets);
    static void walk(dom::DocumentElement* el,
                     const Vector<SharedPtr<StyleSheet>>& sheets);
};

}  // namespace css
}  // namespace boymue

#endif

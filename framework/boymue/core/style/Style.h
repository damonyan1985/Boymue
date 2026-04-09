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

/// 对标 WebCore CSSPropertyPosition 等的最小子集（供布局/绘制消费）
enum class PositionValue : uint8_t {
    Static,
    Relative,
    Absolute,
    Fixed,
};

enum class DisplayValue : uint8_t {
    Inline,
    Block,
    InlineBlock,
    None,
    Flex,
};

enum class VisibilityValue : uint8_t {
    Visible,
    Hidden,
};

enum class TextAlignValue : uint8_t {
    Start,
    Left,
    Center,
    Right,
    End,
    Justify,
};

enum class OverflowValue : uint8_t {
    Visible,
    Hidden,
    Scroll,
    Auto,
};

enum class FontStyleValue : uint8_t {
    Normal,
    Italic,
    Oblique,
};

class Color {
public:
    Color(ColorValue value = 0);
    Color(BMByte r, BMByte g, BMByte b, BMByte a = 0xFF);
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

    /// 从父节点的计算样式复制 CSS 中「默认继承」的属性（对标 WebCore RenderStyle::inheritFrom 子集）。
    void inheritFrom(const Style& parent);

    // css color
    Color color;
    // css background-color
    Color bgColor;

    LayoutUnit left{0};
    LayoutUnit top{0};
    LayoutUnit right{0};
    LayoutUnit bottom{0};
    LayoutUnit width{0};
    LayoutUnit height{0};

    LayoutUnit marginTop{0};
    LayoutUnit marginRight{0};
    LayoutUnit marginBottom{0};
    LayoutUnit marginLeft{0};
    LayoutUnit paddingTop{0};
    LayoutUnit paddingRight{0};
    LayoutUnit paddingBottom{0};
    LayoutUnit paddingLeft{0};

    PositionValue position{PositionValue::Static};
    bool zIndexAuto{true};
    int zIndex{0};

    LayoutUnit minWidth{0};
    LayoutUnit minHeight{0};
    /// 0 表示 max-*: none（无上限）
    LayoutUnit maxWidth{0};
    LayoutUnit maxHeight{0};

    float opacity{1.f};
    VisibilityValue visibility{VisibilityValue::Visible};
    DisplayValue display{DisplayValue::Block};

    /// 0 表示未设置，由布局使用默认值
    int fontSizePx{0};
    String fontFamily;
    int fontWeight{400};
    FontStyleValue fontStyle{FontStyleValue::Normal};
    /// 0 表示 line-height: normal
    LayoutUnit lineHeightPx{0};

    TextAlignValue textAlign{TextAlignValue::Start};
    bool textUnderline{false};
    bool textLineThrough{false};
    LayoutUnit letterSpacing{0};
    LayoutUnit wordSpacing{0};

    LayoutUnit borderTopWidth{0};
    LayoutUnit borderRightWidth{0};
    LayoutUnit borderBottomWidth{0};
    LayoutUnit borderLeftWidth{0};
    Color borderColor;
    LayoutUnit borderRadius{0};

    OverflowValue overflowX{OverflowValue::Visible};
    OverflowValue overflowY{OverflowValue::Visible};

    /// url(...) 内路径或裸 url，无则空
    String backgroundImage;
};
}
}

#endif
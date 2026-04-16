// Copyright Boymue Authors. All rights reserved.
// Author boymue on 2021.01.26

#ifndef DocumentElement_h
#define DocumentElement_h

#include <functional>

#include "StringUtil.h"

namespace boymue {
namespace layout {
class Layout;    
}    
namespace dom {
class Document;

class DocumentElement {
public:
    DocumentElement(Document* dom);
    virtual ~DocumentElement();

    virtual void parseAttribute(const char** atts) {}
    void addChild(DocumentElement* element);
    virtual void removeChild(DocumentElement* element);
    // layout与element一一对应
    layout::Layout* layout();
    // 创建对应的Layout对象
    layout::Layout* createLayout();
    
    int uid() const;
    const String& styleId() const;
    
    void setUid(int uid);
    void setStyleId(const String& styleId);
    virtual void setProperty(const String& key, const String& value);
    virtual String getProperty(const String& key) const;

    /// 用于 CSS 类型选择器，与 DomTags / 标签名一致（小写）
    virtual String tagName() const;
    DocumentElement* parent() const { return m_parent; }
    void setParent(DocumentElement* p) { m_parent = p; }
    Document* document() const { return m_dom; }

    virtual void visitChildren(const std::function<void(DocumentElement*)>& visitor) const;

    virtual bool isText() const;
    virtual bool isImage() const;
    virtual bool isTextField() const;
    virtual bool isView() const;
    virtual bool isButton() const;
    virtual bool isForm() const;
    virtual bool isIframe() const;
    virtual bool isStyle() const;

protected:
    Document* domDocument() const { return m_dom; }
    DocumentElement* m_parent{nullptr};

private:
    Document* m_dom;
    // 0是无效id, uid为js端分配的唯一id
    int m_uid;
    // 样式id, 通过styleId可以匹配样式
    String m_styleId;
    // 属性
    HashMap<String, String> m_properties;
    // Layout属性
    layout::Layout* m_layout{nullptr};
    /// 与 DOM id 属性同步，供 Document::elementById
    String m_domElementId;
    List<SharedPtr<DocumentElement>> m_children;
};

/// 内嵌子文档入口（对标 HTMLIFrameElement），与 DocumentElement 同文件声明
class IframeElement : public DocumentElement {
public:
    explicit IframeElement(Document* dom);
    void parseAttribute(const char** atts) override;
    void setProperty(const String& key, const String& value) override;
    String getProperty(const String& key) const override;

    bool isIframe() const override;
    String tagName() const override;

    const String& src() const { return m_src; }
    const String& name() const { return m_nameAttr; }
    const String& sandbox() const { return m_sandbox; }

private:
    static String normalizeKey(const String& key);
    void applyIframeAttribute(const String& keyLower, const String& value);

    String m_src;
    String m_nameAttr;
    String m_sandbox;
};

/// 对标 HTMLStyleElement，&lt;style&gt; 子节点文本为 CSS
class StyleElement : public DocumentElement {
public:
    explicit StyleElement(Document* dom);
    String tagName() const override;
    bool isStyle() const override;
};
}
}  // namespace boymue
#endif  // !DocumentElement_h

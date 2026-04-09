// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#ifndef Document_h
#define Document_h

#include <stack>
#include <string>

#include "DocumentElement.h"
#include "StyleEngine.h"
#include "TextElement.h"
#include "StringUtil.h"

namespace boymue {
namespace dom {

class Document;

/// 对标 WebCore LocalFrame：承载视口与关联的 Document（不嵌套在 Document 内）
class Frame {
public:
    explicit Frame(Document* owner);
    Document* document() const { return m_owner; }
    LayoutUnit viewportWidth() const { return m_viewportWidth; }
    LayoutUnit viewportHeight() const { return m_viewportHeight; }
    void setViewport(LayoutUnit w, LayoutUnit h);

private:
    Document* m_owner;
    LayoutUnit m_viewportWidth{0};
    LayoutUnit m_viewportHeight{0};
};

// Create and run in ui thread
class Document {
public:
    enum DOMCmdType {
        kAddNode = 1,
        kSetProp,
        kRemoveNode,
        kReplaceNode
    };
    
    Document();
    Frame& frame() { return m_frame; }
    const Frame& frame() const { return m_frame; }
    // 利用XML内容来初始化document
    void parseFromXML(const String& content);
    css::StyleEngine& styleEngine() { return m_styleEngine; }
    const css::StyleEngine& styleEngine() const { return m_styleEngine; }
    Stack<DocumentElement*>* getParseStack();
    DocumentElement* createElement(int tag, const char** atts,
                                 DocumentElement* parent);
    
    // 创建元素，tag标签名，uid js创建的唯一id，pid是parent_id
    void createElement(int tag, int uid, int pid);

    // 删除元素，pid是parent_id，uid js创建的唯一id
    void removeElement(int pid, int uid);
    
    void addUniqueElement(DocumentElement* elem);
    void addStyleElement(DocumentElement* elem);

    DocumentElement* root() const { return m_root; }

    /// 通过 id 查找（与 HTML getElementById 一致，供 form 关联等）
    DocumentElement* elementById(const String& id) const;
    void registerElementById(const String& id, DocumentElement* el);
    /// 仅当当前映射仍指向 el 时移除，避免误删已复用 id 的节点
    void unregisterElementId(const String& id, DocumentElement* el);
    /// 解析结束后将带 form 属性的控件关联到 FormElement
    void relinkFormControls();
    
    void setElementProperty(int uid, const String& key, const String& value);

private:
    Frame m_frame;
    css::StyleEngine m_styleEngine;
    Stack<DocumentElement*> m_parseStack;
    DocumentElement* m_root;
    // 使用elem id查找DocumentElement
    HashMap<int, DocumentElement*> m_uniqueElems;
    // 使用style id查找DocumentElement
    HashMap<String, DocumentElement*> m_styleElems;
    HashMap<String, DocumentElement*> m_idElems;
};
}
}  // namespace boymue
#endif  // !Document_h

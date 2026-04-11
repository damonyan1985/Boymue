//
//  InputElement.h
//  core
//
//  Created by yanbo on 2022/9/16.
//

#ifndef InputElement_h
#define InputElement_h

#include "ViewElement.h"

namespace boymue {
namespace dom {

/// 对标 HTMLFormElement，与表单控件同属表单相关，声明于本头文件
class FormElement : public ViewElement {
public:
    explicit FormElement(Document* dom);
    void parseAttribute(const char** atts) override;
    void setProperty(const String& key, const String& value) override;
    String getProperty(const String& key) const override;

    bool isForm() const override;
    String tagName() const override;

    const String& action() const { return m_action; }
    const String& method() const { return m_method; }
    const String& enctype() const { return m_enctype; }
    const String& target() const { return m_target; }
    const String& formName() const { return m_formName; }
    bool noValidate() const { return m_noValidate; }

private:
    static String normalizeKey(const String& key);
    void applyFormElemAttribute(const String& keyLower, const String& value);

    String m_action;
    String m_method{"get"};
    String m_enctype{"application/x-www-form-urlencoded"};
    String m_target;
    String m_formName;
    bool m_noValidate{false};
};

/// 表单控件字段对标 HTMLInputElement / 文本类输入（name、value、type、form 关联等）
class InputElement : public DocumentElement {
public:
    InputElement(Document* dom);
    void parseAttribute(const char** atts) override;
    void setProperty(const String& key, const String& value) override;
    String getProperty(const String& key) const override;

    bool isTextField() const override;
    String tagName() const override;

    const String& name() const { return m_name; }
    const String& value() const { return m_value; }
    void setValue(const String& v);
    const String& inputType() const { return m_inputType; }
    const String& placeholder() const { return m_placeholder; }
    const String& form() const { return m_form; }
    bool disabled() const { return m_disabled; }
    bool readOnly() const { return m_readOnly; }
    int maxLength() const { return m_maxLength; }
    const String& autoComplete() const { return m_autoComplete; }
    bool checked() const { return m_checked; }
    void setChecked(bool c);

    /// 解析 form 属性后关联到的 FormElement（依赖 Document::elementById）
    FormElement* formElement() const { return m_formElement; }
    void relinkFormOwner();

private:
    static String normalizeKey(const String& key);
    void applyFormAttribute(const String& keyLower, const String& value);
    void refreshFormElement();

    String m_name;
    String m_value;
    String m_inputType{"text"};
    String m_placeholder;
    /// HTML form 属性：关联的 form 元素 id
    String m_form;
    String m_autoComplete;
    int m_maxLength{-1};
    bool m_disabled{false};
    bool m_readOnly{false};
    bool m_checked{false};
    FormElement* m_formElement{nullptr};
};
}
}

#endif /* InputElement_h */

//
//  InputElement.cpp
//  core
//
//  Created by yanbo on 2022/9/16.
//

#include "InputElement.h"

#include <cctype>

#include "Document.h"

namespace boymue {
namespace dom {

namespace {

bool parseHtmlBoolean(const String& val) {
    if (val.empty()) {
        return true;
    }
    String v = val;
    for (char& c : v) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    StringUtil::trim(v);
    if (v == "false" || v == "0" || v == "no") {
        return false;
    }
    return true;
}

void toLowerAsciiInPlace(String& s) {
    for (char& c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

}  // namespace

// --- FormElement ---

String FormElement::normalizeKey(const String& key) {
    String k = key;
    toLowerAsciiInPlace(k);
    return k;
}

void FormElement::applyFormElemAttribute(const String& keyLower, const String& value) {
    if (keyLower == "action") {
        m_action = value;
    } else if (keyLower == "method") {
        m_method = value;
        toLowerAsciiInPlace(m_method);
        if (m_method.empty()) {
            m_method = "get";
        }
    } else if (keyLower == "enctype") {
        m_enctype = value;
    } else if (keyLower == "target") {
        m_target = value;
    } else if (keyLower == "name") {
        m_formName = value;
    } else if (keyLower == "novalidate") {
        m_noValidate = parseHtmlBoolean(value);
    }
}

FormElement::FormElement(Document* dom)
    : ViewElement(dom) {}

void FormElement::parseAttribute(const char** atts) {
    if (!atts) {
        return;
    }
    for (int i = 0; atts[i]; i += 2) {
        String k = atts[i];
        const char* av = atts[i + 1];
        String v = av ? String(av) : String();
        setProperty(k, v);
    }
}

void FormElement::setProperty(const String& key, const String& value) {
    applyFormElemAttribute(normalizeKey(key), value);
    DocumentElement::setProperty(key, value);
}

String FormElement::getProperty(const String& key) const {
    String kl = normalizeKey(key);
    if (kl == "action") {
        return m_action;
    }
    if (kl == "method") {
        return m_method;
    }
    if (kl == "enctype") {
        return m_enctype;
    }
    if (kl == "target") {
        return m_target;
    }
    if (kl == "name") {
        return m_formName;
    }
    if (kl == "novalidate") {
        return m_noValidate ? String("true") : String("false");
    }
    return DocumentElement::getProperty(key);
}

bool FormElement::isForm() const {
    return true;
}

String FormElement::tagName() const {
    return "form";
}

// --- InputElement ---

String InputElement::normalizeKey(const String& key) {
    String k = key;
    toLowerAsciiInPlace(k);
    return k;
}

void InputElement::applyFormAttribute(const String& keyLower, const String& value) {
    if (keyLower == "name") {
        m_name = value;
    } else if (keyLower == "value") {
        m_value = value;
    } else if (keyLower == "type") {
        m_inputType = value;
        toLowerAsciiInPlace(m_inputType);
        if (m_inputType.empty()) {
            m_inputType = "text";
        }
    } else if (keyLower == "placeholder") {
        m_placeholder = value;
    } else if (keyLower == "form") {
        m_form = value;
    } else if (keyLower == "autocomplete") {
        m_autoComplete = value;
    } else if (keyLower == "maxlength") {
        if (value.empty()) {
            m_maxLength = -1;
        } else {
            m_maxLength = StringUtil::stringToInt(value);
            if (m_maxLength < 0) {
                m_maxLength = -1;
            }
        }
    } else if (keyLower == "disabled") {
        m_disabled = parseHtmlBoolean(value);
    } else if (keyLower == "readonly") {
        m_readOnly = parseHtmlBoolean(value);
    } else if (keyLower == "checked") {
        m_checked = parseHtmlBoolean(value);
    }
}

void InputElement::refreshFormElement() {
    m_formElement = nullptr;
    if (m_form.empty()) {
        return;
    }
    Document* doc = domDocument();
    if (!doc) {
        return;
    }
    DocumentElement* el = doc->elementById(m_form);
    if (el && el->isForm()) {
        m_formElement = static_cast<FormElement*>(el);
    }
}

InputElement::InputElement(Document* dom)
    : DocumentElement(dom) {}

void InputElement::parseAttribute(const char** atts) {
    if (!atts) {
        return;
    }
    for (int i = 0; atts[i]; i += 2) {
        String k = atts[i];
        const char* av = atts[i + 1];
        String v = av ? String(av) : String();
        setProperty(k, v);
    }
    refreshFormElement();
}

void InputElement::setProperty(const String& key, const String& value) {
    String kl = normalizeKey(key);
    applyFormAttribute(kl, value);
    DocumentElement::setProperty(key, value);
    if (kl == "form") {
        refreshFormElement();
    }
}

void InputElement::relinkFormOwner() {
    refreshFormElement();
}

String InputElement::getProperty(const String& key) const {
    String kl = normalizeKey(key);
    if (kl == "name") {
        return m_name;
    }
    if (kl == "value") {
        return m_value;
    }
    if (kl == "type") {
        return m_inputType;
    }
    if (kl == "placeholder") {
        return m_placeholder;
    }
    if (kl == "form") {
        return m_form;
    }
    if (kl == "autocomplete") {
        return m_autoComplete;
    }
    if (kl == "maxlength") {
        if (m_maxLength < 0) {
            return String();
        }
        return std::to_string(m_maxLength);
    }
    if (kl == "disabled") {
        return m_disabled ? String("true") : String("false");
    }
    if (kl == "readonly") {
        return m_readOnly ? String("true") : String("false");
    }
    if (kl == "checked") {
        return m_checked ? String("true") : String("false");
    }
    return DocumentElement::getProperty(key);
}

void InputElement::setValue(const String& v) {
    m_value = v;
    DocumentElement::setProperty(String("value"), v);
}

void InputElement::setChecked(bool c) {
    m_checked = c;
    DocumentElement::setProperty(String("checked"), c ? String("true") : String("false"));
}

bool InputElement::isTextField() const {
    return true;
}

String InputElement::tagName() const {
    return "input";
}

}  // namespace dom
}  // namespace boymue

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#include "DomTags.h"

namespace boymue {
DomTags* DomTags::instance() {
    static DomTags sTags;
    return &sTags;
}

DomTags::DomTags() {
    initDomTags();
}

void DomTags::initDomTags() {
    m_map["view"] = DomTags::kView;
    m_map["button"] = DomTags::kButton;
    m_map["checkbox"] = DomTags::kCheckBox;
    m_map["radio"] = DomTags::kRadio;
    m_map["image"] = DomTags::kImage;
    m_map["text"] = DomTags::kText;
    m_map["input"] = DomTags::kInput;
    m_map["icon"] = DomTags::kIcon;
}

int DomTags::getTag(const std::string& key)
{
    return m_map[key];
}
}

// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#include "DomTags.h"

namespace boymue {
DomTags* DomTags::instance() {
    // 原子结构，支持多线程调用
    static DomTags sTags;
    return &sTags;
}

DomTags::DomTags() {
    initDomTags();
}

void DomTags::initDomTags() {
    m_map["view"] = DomTags::kView;
    m_map["button"] = DomTags::kButton;
    m_map["image"] = DomTags::kImage;
    m_map["text"] = DomTags::kText;
    m_map["textfield"] = DomTags::kTextField;
}

void DomTags::addExtensionTag(const String& key, int tag) {
    m_map[key] = tag;
}

int DomTags::getTag(const String& key) {
    return m_map[key];
}
}

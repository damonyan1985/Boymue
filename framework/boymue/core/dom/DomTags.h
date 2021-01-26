// Copyright Boymue Authors. All rights reserved.
// Author yanbo on 2021.01.23

#ifndef DomTags_h
#define DomTags_h

#include <string>
#include <unordered_map>

namespace boymue {
class DomTags {
public:
    enum TagType
    {
        kView,
        kButton,
        kImage,
        kText,
        kRadio,
        kCheckBox,
        kInput,
        kIcon,
    };

    static DomTags* instance();
    void initDomTags();
    int getTag(const std::string& key);

private:
    DomTags();
    std::unordered_map<std::string, int> m_map;
};
}
#endif

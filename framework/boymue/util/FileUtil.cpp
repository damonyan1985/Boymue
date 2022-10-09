//
//  FileUtil.cpp
//  core
//
//  Created by yanbo on 2022/9/5.
//

#include "FileUtil.h"
#include <fstream>

namespace boymue {
String FileUtil::readFile(const String& path) {
    std::ifstream file(path);
    if (file.fail()) {
        return "";
    }
    
    return String((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
}
}

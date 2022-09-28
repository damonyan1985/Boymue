//
//  FileUtil.h
//  core
//
//  Created by yanbo on 2022/9/5.
//

#ifndef FileUtil_h
#define FileUtil_h

#include "StringUtil.h"

namespace boymue {
class FileUtil {
public:
    static String readFile(const String& path);
};
}

#endif /* FileUtil_h */

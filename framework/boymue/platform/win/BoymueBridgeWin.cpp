#include "BoymueBridge.h"

namespace boymue {
String BoymueBridge::getAppRoot() {
	return "";
}

String BoymueBridge::getSourcePath(const String& path) {
	// TODO Debug时先使用工程目录，后期处理Release目录
	String envPath = getenv("BOYMUE_ROOT");
	return envPath + "\\boymuejs" + path;
}
}
#include "BoymueBridge.h"

namespace boymue {
String BoymueBridge::getAppRoot() {
	return "";
}

String BoymueBridge::getSourcePath(const String& path) {
	// TODO Debugʱ��ʹ�ù���Ŀ¼�����ڴ���ReleaseĿ¼
	String envPath = getenv("BOYMUE_ROOT");
	return envPath + "\\boymuejs" + path;
}
}
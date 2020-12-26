import os

android_dir = os.getcwd()
gradle_cmd = os.path.join(android_dir, 'gradlew')

build_core_library_cmd = (
    f'{gradle_cmd} :core:assembleDebug --rerun-tasks'
)

build_app_cmd = (
    f'{gradle_cmd} :app:assembleDebug'
)

install_apk_cmd = (
    'adb install -t app/build/outputs/apk/debug/app-debug.apk'
)

launch_app_cmd = (
    'adb shell am start -n com.boymue.app/com.boymue.app.MainActivity'
)


def main():
    os.system(build_core_library_cmd)
    os.system(build_app_cmd)
    os.system(install_apk_cmd)
    os.system(launch_app_cmd)


if __name__ == '__main__':
    main()

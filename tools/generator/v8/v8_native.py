# use python3 to generate v8 native file
# author yanbo 2021. All rights reserved.
import os

current_path = os.getcwd()
v8_project_path = os.path.join(
    current_path, '../../../framework/thirdparty/v8')

library_files = [
    f'{v8_project_path}/src/js/macros.py',
    f'{v8_project_path}/src/messages.h',
    f'{v8_project_path}/src/js/prologue.js',
    f'{v8_project_path}/src/js/runtime.js',
    f'{v8_project_path}/src/js/v8natives.js',
    f'{v8_project_path}/src/js/symbol.js',
    f'{v8_project_path}/src/js/array.js',
    f'{v8_project_path}/src/js/string.js',
    f'{v8_project_path}/src/js/uri.js',
    f'{v8_project_path}/src/js/math.js',
    f'{v8_project_path}/src/third_party/fdlibm/fdlibm.js',
    f'{v8_project_path}/src/js/regexp.js',
    f'{v8_project_path}/src/js/arraybuffer.js',
    f'{v8_project_path}/src/js/typedarray.js',
    f'{v8_project_path}/src/js/iterator-prototype.js',
    f'{v8_project_path}/src/js/generator.js',
    f'{v8_project_path}/src/js/object-observe.js',
    f'{v8_project_path}/src/js/weak-collection.js',
    f'{v8_project_path}/src/js/collection-iterator.js',
    f'{v8_project_path}/src/js/promise.js',
    f'{v8_project_path}/src/js/messages.js',
    f'{v8_project_path}/src/js/json.js',
    f'{v8_project_path}/src/js/array-iterator.js',
    f'{v8_project_path}/src/js/string-iterator.js',
    # f'{v8_project_path}/src/js/templates.js',
    f'{v8_project_path}/src/js/spread.js',
    f'{v8_project_path}/src/debug/mirrors.js',
    f'{v8_project_path}/src/debug/debug.js',
    f'{v8_project_path}/src/debug/liveedit.js',
]

experimental_library_files = [
    f'{v8_project_path}/src/js/macros.py',
    f'{v8_project_path}/src/messages.h',
    f'{v8_project_path}/src/js/proxy.js',
    f'{v8_project_path}/src/js/generator.js',
    f'{v8_project_path}/src/js/harmony-atomics.js',
    f'{v8_project_path}/src/js/harmony-regexp-exec.js',
    f'{v8_project_path}/src/js/harmony-object-observe.js',
    f'{v8_project_path}/src/js/harmony-sharedarraybuffer.js',
    f'{v8_project_path}/src/js/harmony-simd.js',
    f'{v8_project_path}/src/js/harmony-species.js',
    f'{v8_project_path}/src/js/harmony-unicode-regexps.js',
    f'{v8_project_path}/src/js/harmony-string-padding.js',
    f'{v8_project_path}/src/js/promise-extra.js',
]

target_out_path = os.path.join(v8_project_path, 'gen')

# 目前自动生成文件的脚本只能使用python2来生成
python_version_cmd = 'python2'

v8_extra_library_files = []

v8_experimental_extra_library_files = []

experimental_library_cc_cmd_head = f'{python_version_cmd} js2c.py {target_out_path}/experimental-libraries.cc EXPERIMENTAL '

library_cc_gen_cmd_head = f'{python_version_cmd} js2c.py {target_out_path}/libraries.cc CORE '

extras_libraries_cc_cmd_head = f'{python_version_cmd} js2c.py {target_out_path}/extras-libraries.cc EXTRAS '

experimental_extra_library_cc_cmd_head = f'{python_version_cmd} js2c.py {target_out_path}/experimental-extras-libraries.cc EXPERIMENTAL_EXTRAS '

concatenate_natives_blob_cmd = f'{python_version_cmd} concatenate-files.py {target_out_path}/libraries.bin {target_out_path}/libraries-experimental.bin {target_out_path}/libraries-extras.bin {target_out_path}/libraries-experimental-extras.bin {target_out_path}/natives_blob.bin'


def exec_libraries_bin_cmd():
    print('===begin build library===')
    library_cc_gen_cmd = library_cc_gen_cmd_head + ' '.join(library_files)
    global library_bin_gen_cmd
    library_bin_gen_cmd = library_cc_gen_cmd_head + \
        ' '.join(library_files) + \
        f' --startup_blob {target_out_path}/libraries.bin --nojs'
    print(library_cc_gen_cmd + '\n')
    os.system(library_cc_gen_cmd)

    print(library_bin_gen_cmd + '\n')
    os.system(library_bin_gen_cmd)
    print('===end build library===')


def exec_experimental_library_bin_cmd():
    print('===begin build experimental library===')
    experimental_library_cc_cmd = experimental_library_cc_cmd_head + \
        ' '.join(experimental_library_files)
    print(experimental_library_cc_cmd + '\n')
    os.system(experimental_library_cc_cmd)

    experimental_library_bin_cmd = experimental_library_cc_cmd_head + \
        ' '.join(experimental_library_files) + \
        f' --startup_blob {target_out_path}/libraries-experimental.bin --nojs'
    os.system(experimental_library_bin_cmd)
    print('===end build experimental library===')


def exec_extras_libraries_bin_cmd():
    print('===begin build extras library===')
    extras_libraries_cc_cmd = extras_libraries_cc_cmd_head + \
        ' '.join(v8_extra_library_files)
    print(extras_libraries_cc_cmd + '\n')
    os.system(extras_libraries_cc_cmd)

    extras_libraries_bin_cmd = extras_libraries_cc_cmd_head + \
        ' '.join(v8_extra_library_files) + \
        f' --startup_blob {target_out_path}/libraries-extras.bin --nojs'
    print(extras_libraries_bin_cmd + '\n')
    os.system(extras_libraries_bin_cmd)
    print('===end build extras library===')


def exec_experimental_extra_library_bin_cmd():
    print('===begin build experimental extras library===')
    experimental_extra_library_cc_cmd = experimental_extra_library_cc_cmd_head + \
        ' '.join(v8_experimental_extra_library_files)
    print(experimental_extra_library_cc_cmd + '\n')
    os.system(experimental_extra_library_cc_cmd)
    experimental_extra_library_bin_cmd = experimental_extra_library_cc_cmd_head + \
        ' '.join(v8_experimental_extra_library_files) + \
        f' --startup_blob {target_out_path}/libraries-experimental-extras.bin --nojs'
    print(experimental_extra_library_bin_cmd + '\n')
    os.system(experimental_extra_library_bin_cmd)
    print('===end build experimental extras library===')


def exec_natives_blob_bin_cmd():
    os.system(concatenate_natives_blob_cmd)


def snapshot_blob_bin_cmd():
    os.system(f'mksnapshot --startup_blob {target_out_path}/snapshot_blob.bin')


def main():
    '''
    for jsfile in library_files:
      library_cc_gen_cmd.join(' ')
      library_cc_gen_cmd.join(jsfile)
      library_bin_gen_cmd.join(' ')
      library_bin_gen_cmd.join(jsfile)
    '''
    if os.path.exists(target_out_path) == False:
        os.mkdir(target_out_path)

    exec_libraries_bin_cmd()
    exec_experimental_library_bin_cmd()
    exec_extras_libraries_bin_cmd()
    exec_experimental_extra_library_bin_cmd()
    exec_natives_blob_bin_cmd()


if __name__ == "__main__":
    main()

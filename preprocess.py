import os
from datetime import datetime

def generate_python_sources():
    sources = {}
    for file in os.listdir("project/python"):
        assert file.endswith(".py")
        key = file.split(".")[0]
        with open("project/python/" + file, encoding='utf-8') as f:
            value = f.read()
            value = value.encode('utf-8').hex(':')
            value = '\\x' + value.replace(':', '\\x')
        sources[key] = value

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    header = '''#pragma once
// generated on ''' + timestamp + '''
#include <map>
#include <string>

namespace pe{
    inline static std::map<std::string, const char*> kPythonLibs = {
'''
    for key, value in sources.items():
        header += ' '*8 + '{"' + key + '", "' + value + '"},'
        header += '\n'

    header += '''
    };
} // namespace pe
'''
    return header

with open("project/_generated.h", "w", encoding='utf-8') as f:
    f.write(generate_python_sources())

import shutil
shutil.rmtree("as_proj/app/src/main/assets", ignore_errors=True)
shutil.copytree("game", "as_proj/app/src/main/assets")

print("预处理完成")
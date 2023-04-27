import pype

INDENT = ' ' * 4

def parse_function(obj, depth):
    pass

def parse_class_or_module(obj, depth):
    ret = []
    for name, field in obj.__dict__.items():
        type_name = type(field).__name__
        if type_name.startswith('pype.'):
            type_name = type_name[5:]
        if isinstance(field, type):
            ret.append(f'class {name}:')
            ret.append(INDENT*depth + '\n'.join(parse_class_or_module(field, depth+1)))
            ret.append('')
        elif type_name in ('function', 'native_func'):
            ret.append(INDENT*depth + f'def {name}():\n' + INDENT*(depth+1) + 'pass')
        else:
            if type_name == 'NoneType':
                ret.append(INDENT*depth + f'{name} = ...')
            else:
                ret.append(INDENT*depth + f"{name}: {type_name} = ...")

    return ret

ret = parse_class_or_module(pype, 0)

with open('pype.pyi', 'w') as f:
    f.write('\n'.join(ret))


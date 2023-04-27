import pype

INDENT = ' ' * 4

def parse_function(obj, depth):
    pass

def tp_name(obj):
    obj = obj.__name__
    if obj.startswith('pype.'):
        obj = obj[5:]
    return obj

def parse_class_or_module(obj, depth):
    ret = []
    for name, field in obj.__dict__.items():
        type_name = tp_name(type(field))
        if isinstance(field, type):
            base_name = tp_name(field.__base__)
            ret.append(f'class {name}({base_name}):')
            ret.append(INDENT*depth + '\n'.join(parse_class_or_module(field, depth+1)))
            ret.append('')
        elif type_name in ('function', 'native_func'):
            ret.append(INDENT*depth + f'def {name}():')
            if '__doc__' in field.__dict__:
                doc = field.__doc__
                doc = doc.replace('\n', '\n' + INDENT*(depth+1))
                ret.append(INDENT*(depth+1) + f'"""{doc}"""')
            ret.append(INDENT*(depth+1) + 'pass')
        else:
            if type_name == 'NoneType':
                ret.append(INDENT*depth + f'{name} = ...')
            else:
                ret.append(INDENT*depth + f"{name}: {type_name} = ...")

    return ret

ret = parse_class_or_module(pype, 0)

with open('pype.pyi', 'w') as f:
    f.write('\n'.join(ret))


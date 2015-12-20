from os import listdir
from os.path import isfile, join, dirname, realpath, split
import json

def list_folder(folder):
    result = {
        'files': [],
        'folders': []
    }

    for f in listdir(folder):
        if isfile(join(folder, f)):
            result['files'].append(join(folder, f))
        else:
            result['folders'].append(join(folder, f))

    return result

def read_data(folderPath):
    TREE = []
    for folder in list_folder(folderPath)['folders']:
        head, tail = split(folder)
        el = {
            'name': tail,
            'cmd': False,
            'count': 0
        }

        for file in list_folder(folder)['files']:
            if file == join(folder , 'cmd'):
                with open(file) as f:
                    el['cmd'] = f.read().strip()
            else:
                el['count'] += 1
        if not el['cmd']:
            continue
        TREE.append(el)
    return TREE

dataPath = join(dirname(realpath(__file__)), '../data')
destPath = join(dirname(realpath(__file__)), '../gestures.json')

data = json.dumps(read_data(dataPath))
with open(destPath, 'w') as f:
    f.write(data)
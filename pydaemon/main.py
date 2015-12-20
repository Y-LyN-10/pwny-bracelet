from os import listdir
from os.path import isfile, join
from subprocess import call
import math
import time
import random

def exec_command(cmd):
    parts = cmd.split(' ')
    call(parts)

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
    TS = []
    CMDS = {}
    for folder in list_folder(folderPath)['folders']:
        for file in list_folder(folder)['files']:
            if file == join(folder , 'cmd'):
                with open(file) as f:
                    CMDS[folder] = f.read().strip()
            else:
                TS.append({
                    'name': folder,
                    'data': read_vector(file)
                })

    return (TS, CMDS)

def read_data_counts(folderPath):
    P = {'total': 0}
    for folder in list_folder(folderPath)['folders']:
        for file in list_folder(folder)['files']:
            if file == join(folder , 'cmd'):
                continue
            if not folder in P:
                P[folder] = 0
            P[folder] += 1
            P['total'] += 1

    for k in P:
        if k == 'total':
            continue
        P[k] = float(P[k]) / P['total']

    return P

def read_vector_weighted(filePath):
    data = []
    alpha = 0.8
    G=[0.0,0.0,0.0]
    with open(filePath) as file:
        for line in file.readlines():
            parts = line.strip().split(',')
            try:
                for k,p in enumerate(parts):
                    p = int(p)
                    G[k] = alpha * G[k] + (1 - alpha) * p
                    data.append(p - G[k])
            except:
                continue
    return data

def read_vector(filePath):
    data = []
    with open(filePath) as file:
        for line in file.readlines():
            parts = line.strip().split(',')
            try:
                for k,p in enumerate(parts):
                    p = int(p)
                    data.append(p)
            except:
                continue
    return data

def dist(V1, V2):
    pos = 0
    dist = 0

    while True:
        if pos >= len(V1) or pos >= len(V2):
            break
        d = V2[pos] - V1[pos]
        dist += math.sqrt(d * d)
        pos  += 1

    return dist

def dist_weighted(V1, V2, W):
    pos = 0
    dist = 0

    while True:
        if pos >= len(V1) or pos >= len(V2):
            break
        d = V2[pos] - V1[pos]
        dist += W[pos] * math.sqrt(d * d)
        pos  += 1

    return dist

def reliefF(TS, P, k, m):
    W = [0] * len(TS[0]['data'])

    C = {}
    for t in TS:
        if t['name'] not in C:
            C[t['name']] = []
        C[t['name']].append(t)

    for iteration in range(m):
        E = random.choice(TS)
        positive = kNN(E['data'], C[E['name']], k + 1)
        if len(positive) == 1:
            continue
        positive = positive[1:]

        N = {}
        for c in P:
            if c == 'total' or c == E['name']:
                continue

            N[c] = kNN(E['data'], C[c], k)

        for i in range(len(E['data'])):

            d = 0
            for p in positive:
                el = C[E['name']][p[0]]

                if i >= len(el['data']):
                    continue

                d += abs(E['data'][i] - el['data'][i])

            for c in N:
                sd = 0

                for n in N[c]:
                    el = C[c][n[0]]
                    if i >= len(el['data']):
                        continue
                    sd += abs(E['data'][i] - el['data'][i])

                d -= sd * (P[c] / (1-P[E['name']]))

            W[i] += d / (k * m)

    for k,w in enumerate(W):
        if w > 0.6:
            W[k] = 1
        else:
            W[k] = 0

    return W



def kNN(V,TS,k):
    D = []
    for n, E in enumerate(TS):
        D.append((n, E['name'], dist(V, E['data'])))

    D = sorted(D,key=lambda x: x[2])

    if(len( D ) < k):
        return D

    return D[:k]

def kNN_weighted(V,TS,k,W):
    D = []
    for n, E in enumerate(TS):
        D.append((n, E['name'], dist_weighted(V, E['data'], W)))

    D = sorted(D,key=lambda x: x[2])

    if(len( D ) < k):
        return D

    return D[:k]



dataPath   = '../data'
streamPath = '../stream'
refreshInterval = 10000

P = read_data_counts(dataPath)
TS, CMDS = read_data(dataPath)
W = reliefF(TS, P, 3, 100)

refresh = time.time()

#print(P)
#print('CMDS: ', CMDS)

# TODO: FEATURE SELECTION
# TODO: EXAMPLES CHANGE LISTENER

while True:
    V = read_vector(streamPath)
    print(kNN(V,TS,5))
    print(kNN_weighted(V,TS,5,W))
    break
    time.sleep(0.02)




'''
for file in list_folder('testset')['files']:
    print(file)
    V = read_vector(file)
    print(kNN(V,TS,5))
'''
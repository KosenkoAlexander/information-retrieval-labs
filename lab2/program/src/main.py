from collections import Counter
import math
import os

index = []#of tuples (filepath, tf dict)

def collect_tf(document):
    terms = Counter(document.split())
    return {k: math.log(1+v) for k,v in terms.items()}

def fill_index_from_file(path):
    with open(path, 'r') as f:
        doc = f.read()
        index.append((path, collect_tf(doc)))

def fill_index_from_dir(path):
    for f in os.scandir(path):
        if f.is_file():
            fill_index_from_file(f.path)

def cos_similarity(tf1, tf2, tf2_l2):
    v_prod = 0
    for k in tf1.keys():
        if k in tf2:
            v_prod += tf1[k]*tf2[k]
    return v_prod/(tf2_l2*math.sqrt(sum((v**2 for v in tf1.values()))))

cos_threshold = 0.5
max_output = 0#0 = unlimited

def print_retrieved(retrieved):
    for path, similarity in retrieved:
        print(f'{path} (cosine similarity = {similarity:.3f})\n')
        with open(path, 'r') as f:
            print(f.read())

def retrieve(query):
    query_tf = collect_tf(query)
    query_tf_l2 = math.sqrt(sum((v**2 for v in query_tf.values())))
    result = []
    for k,v in index:
        s = cos_similarity(v, query_tf, query_tf_l2)
        if(s>=cos_threshold):
            result.append((k,s))
    result.sort(key = lambda x: x[1], reverse=True)
    return result if max_output==0 else result[:max_output]

if __name__=='__main__':
    dir_path = input('Enter dir path:\n')
    fill_index_from_dir(dir_path)
    while(True):
        query = input('Enter query (:h to list special queries)\n')
        if query==':h':
            print(':h - help\n:q - quit\n:t <number between 0 and 1> - set cosine similarity threshold (default 0.5)\n:m <nonnegative integer> - set max number of outputs (0 (default) for unlimited)\n:f <path> - index file\n:d <path> - index files in directory\n')
        elif query==':q':
            break
        elif query.startswith(':t'):
            cos_threshold = float(query.split()[1])
        elif query.startswith(':m'):
            max_output = int(query.split()[1])
        elif query.startswith(':f'):
            fill_index_from_file(query.split()[1])
        elif query.startswith(':d'):
            fill_index_from_dir(query.split()[1])
        elif query.startswith(':'):
            print('Undefined special query\n')
        else:
            r = retrieve(query)
            print_retrieved(r)

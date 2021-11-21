import copy
verts = []
inds = []
cols = []
NUM_CUBES = 20 # length of side of cube
with open('./models/cube.obj','r') as f: #read in the obj file
    lines = f.readlines()
    nv,ni = int(lines[0].split(' ')[0]),int(lines[0].split(' ')[1])
    for i in range(1,len(lines)):
        if lines[i].startswith('v '):
            verts.append(lines[i].split(' ')[1:])
        elif lines[i].startswith('f '):
            inds.append(lines[i].split(' ')[1:])
        elif lines[i].startswith('c '):
            cols.append(lines[i].split(' ')[1:])

#strip newlines from verts, inds, and cols
verts = [[float(vt.strip()) for vt in v] for v in verts]
inds = [[int(it.strip()) for it in i] for i in inds]
cols = [[float(ct.strip()) for ct in c] for c in cols]

new_verts, new_inds = [], []
# generates 10x1 tower of cubes
for i in range(NUM_CUBES):
    curr_v = [[v[0],v[1]+float(i),v[2]] for v in verts]
    curr_i = [[vi + nv*i for vi in ind] for ind in inds]
    # new_verts.append(curr_v)
    # new_inds.append(curr_i)

for z in range(NUM_CUBES):
    for y in range(NUM_CUBES):
        for x in range(NUM_CUBES):
            curr_v = [[v[0]+float(x),v[1]+float(y),v[2]+float(z)] for v in verts]
            curr_i = [[vi + (nv*((z*10*10)+(y*10)+x)) for vi in ind] for ind in inds]
            new_verts.append(curr_v)
            new_inds.append(curr_i) 

verts = new_verts
inds = new_inds
with open(f'multicube{NUM_CUBES}.obj','w') as f:
    f.write(str(nv*NUM_CUBES*NUM_CUBES*NUM_CUBES)+' '+str(ni*NUM_CUBES*NUM_CUBES*NUM_CUBES)+' '+str(NUM_CUBES*NUM_CUBES*NUM_CUBES)+'\n')
    for v in verts:
        for vi,c in zip(v,cols):
            cs = ' '.join(str(vt) for vt in vi)+' ' + ' '.join(str(cl) for cl in c) + '\n'
            cs = cs.replace('[','').replace(']','').replace(',','')
            f.write(cs)
    for i in inds:
        for ind in i:
            f.write(' '.join(str(ini) for ini in ind)+'\n')
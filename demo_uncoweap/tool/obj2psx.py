import sys

# obj2psx v0.2
# 2025/06 Bjorn

if len(sys.argv) < 2:
    print('Usage: obj2psx.py file.obj')
    sys.exit()

file_in = open(sys.argv[1])
file_out = open(sys.argv[2], "w")

lines = file_in.readlines()
file_in.close()

print("vectrices:")
vectrices_list = list()
vectrices_replaced_list = list()
line_index = int(0)
for line in lines:
    line_list = line.split()
    if line_list[0]=='v':
        i = int(round(float(line_list[1]), 0))
        j = int(round(float(line_list[2]), 0))
        k = int(round(float(line_list[3]), 0))
        if [i,j,k] in vectrices_list:
            vectrices_replaced_list.append([line_index,vectrices_list.index([i,j,k])])
            print("vectrices "+str(line_index)+ " was removed.")
        else:
            file_out.write("{"+str(i)+", "+str(j)+", "+str(k)+", 0},\r")
            vectrices_replaced_list.append([line_index,len(vectrices_list)])
            vectrices_list.append([i,j,k])
        line_index+=1

for replace in vectrices_replaced_list:
    print("vectrices "+str(replace[0])+ " was replaced by "+str(replace[1])+ ".")

file_out.write("\r\n")
faces_index_norms = list()
faces_norms = list()
color = int(0)

print("faces:")
line_face = int(0)
for line in lines:
    line_list = line.split()
    if line_list[0]=='f':
        line_face +=1
        i = int(line_list[1].split('/')[0])-1
        j = int(line_list[2].split('/')[0])-1
        k = int(line_list[3].split('/')[0])-1
        for replace in vectrices_replaced_list:
            if replace[0] == i:
                i = replace[1]
            if replace[0] == j:
                j = replace[1]
            if replace[0] == k:
                k = replace[1]
        # print(i+" "+j+" "+k)
        if i == j or i == k or j == k:
            print("Face "+str(i)+", "+str(j)+", "+str(k)+ " was removed.")
        else:
            file_out.write("{"+str(j)+", "+str(i)+", "+str(k)+", "+str(color)+"},\r")
            faces_index_norms.append(int(line_list[1].split('/')[2]))
    
    if line_list[0]=='vn':
        faces_norms.append(line_list[1]+" "+line_list[2]+" "+line_list[3])
        
    if line_list[0]=='usemtl':
        color+=1

# for norm in faces_norms:
file_out.write("\r\n")

for index_norms in faces_index_norms:
    norm_coords = faces_norms[index_norms-1].split()
    i = str(int(float(norm_coords[0])*-4096.0))
    j = str(int(float(norm_coords[1])*-4096.0))
    k = str(int(float(norm_coords[2])*-4096.0))
    file_out.write("{"+i+", "+j+", "+k+", 0},\r")

file_out.close()

print("Number of vectrices: "+str(line_index))
print("Final number of vectrices: "+str(len(vectrices_list)))
print("Number of removed vectrices: "+str(line_index-len(vectrices_list)))
print("Number of faces: "+str(line_face))
print("Final number of faces: "+str(len(faces_index_norms)))
print("Number of removed faces: "+str(line_face-len(faces_index_norms)))
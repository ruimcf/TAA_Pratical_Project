import sys

inputName = str(sys.argv[1])
outputName = "Multiplied"+inputName
fd = open(inputName, 'r')
dest = open(outputName, 'w')
lines = fd.readlines()
dest.writelines(lines[0:2])
listVertices = lines[2].split(" ")
for vertice in listVertices:
    dest.write(str(int(vertice)*4)+" ")

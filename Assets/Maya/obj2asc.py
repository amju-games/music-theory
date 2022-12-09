import codecs
import os.path
import sys

# Convert .obj file to .asc format
# j.c. 2016

# Write .asc file
def outputAsc(outPath, verts, faces):
    outFile = codecs.open(outPath, "w", "utf-8")
    outFile.write('Named object: "%s"\n' % outPath)
    outFile.write('Tri-mesh, Vertices: %d     Faces: %d\n' % (len(verts), len(faces)))
    outFile.write('Vertex list:\n')
  
    for str in verts:
        outFile.write(str)

    outFile.write('Face list:\n')

    for str in faces:
        outFile.write(str)

    outFile.close()

# str is a string in format a/b/c. We just want the 'a' part. This is a vertex index,
#  and is one-based in .obj files. We subtract 1 so it's zero-based, for .asc file.
def getFace(str):
    strs = str.split('/')
    return int(strs[0]) - 1;

# Process a .obj file. Look for lines starting with "v " - these are vertices - 
#  and lines starting with "f " - these are faces. That's all we care about for
#  now.
def processObj(inFilename):
    # Lists of strings. We buildup the list before writing to the file, because we 
    #  have to write the number of items first.
    verts = []
    faces = []

    with open(inFilename) as f:
        for line in f:
            strs = line.split()

            if len(strs) > 0 and strs[0] == 'v':
                # Vertex
                s = 'Vertex %d: X:%s Y:%s Z:%s\n' % (len(verts), strs[1], strs[2], strs[3])
                verts.append(s);
                
            if len(strs) > 0 and strs[0] == 'f':
                # Face
                s = 'Face %d:    A:%d B:%d C:%d AB:1 BC:1 CA:1\n' % (len(faces), getFace(strs[1]), getFace(strs[2]), getFace(strs[3]))
                faces.append(s);

    outPath = '%s.asc' % inFilename
    outputAsc(outPath, verts, faces)

#
# Start
#
if len(sys.argv) >= 2:
    objFilename = sys.argv[1]
else:
    print("No obj filename specified.")
    exit(0)

print('\nProcessing .obj file: %s' % objFilename)
processObj(objFilename)
print('Done.')

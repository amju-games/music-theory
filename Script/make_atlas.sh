# Create texture atlas

IMG=../Assets/Image
DEST=$IMG/atlas.png
ATLAS=../Tools/atlas

# Create atlas, 1024*1024, no grid to avoid bleeding into adjacent cells
$ATLAS create $DEST 1024 1024 --overwrite

# Add heart, cell 0 in 4*4 grid
$ATLAS replace $IMG/heart.png $DEST 4 4 0 --overwrite

# Add circle, cell 1 in 4*4 grid
$ATLAS replace $IMG/circle.png $DEST 4 4 1 --overwrite

# Rough oval: under heart. Cell 8 in 4*8 grid
$ATLAS replace $IMG/rough-oval.png $DEST 4 8 8 --overwrite

# Rough circle: under circle. Cell 18 in 8 * 8 grid
$ATLAS replace $IMG/rough-circle.png $DEST 8 8 18 --overwrite


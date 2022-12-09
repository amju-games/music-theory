# Make binary versions of all .obj files

export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../..
export DEST_DIR=$TOP_DIR/Build/CompiledAssets/iOS
export OBJ2BIN=$THIS_DIR/obj2bin
export SRC_DIR=$TOP_DIR/Assets

mkdir -p $DEST_DIR
mkdir $DEST_DIR/obj

cd $SRC_DIR/obj

for f in *.obj
do
    echo "Found this file: " $f
    $OBJ2BIN $f $DEST_DIR/obj/$f
done

cd $THIS_DIR


# makeglue-ios.sh

export PLATFORM=iOS
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../..
export COMPILED_ASSETS=$TOP_DIR/Build/CompiledAssets
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM
export SRC_DIR=$TOP_DIR/Assets
export GLUE_EXE=$THIS_DIR/glue
export GLUE_FILE=$DEST_DIR/../data-$PLATFORM.glue

mkdir $COMPILED_ASSETS
mkdir $DEST_DIR
mkdir $DEST_DIR/Course
mkdir $DEST_DIR/font2d
mkdir $DEST_DIR/Gui
mkdir $DEST_DIR/Image
mkdir $DEST_DIR/Scene
mkdir -p $DEST_DIR/Shaders/gles
mkdir -p $DEST_DIR/Sound/wav

cp $SRC_DIR/*.txt $DEST_DIR
cp $SRC_DIR/Course/*.txt $DEST_DIR/Course/
cp -R $SRC_DIR/font2d/* $DEST_DIR/font2d
cp $SRC_DIR/Gui/*.txt $DEST_DIR/Gui

#cp $SRC_DIR/gui/$PLATFORM/*.txt $DEST_DIR
cp $SRC_DIR/Image/*.png $DEST_DIR/Image
#cp $SRC_DIR/tex/$PLATFORM/*.png $DEST_DIR
cp $SRC_DIR/Scene/*.txt $DEST_DIR/Scene


cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE

for f in *.txt Course/*.txt Image/*.png obj/*.obj 
do
    echo "Adding file: " $f
    $GLUE_EXE -a $GLUE_FILE $f
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE

cd $THIS_DIR


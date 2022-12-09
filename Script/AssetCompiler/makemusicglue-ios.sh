# makemusicglue-ios.sh

export PLATFORM=iOS
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../..
export COMPILED_ASSETS=$TOP_DIR/Build/CompiledAssets
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM
export SRC_DIR=$TOP_DIR/Assets
export GLUE_EXE=$THIS_DIR/glue
export GLUE_FILE=$DEST_DIR/../music-$PLATFORM.glue

mkdir $COMPILED_ASSETS
mkdir -p $DEST_DIR/Music
mkdir -p $DEST_DIR/Sound/wav

cp $SRC_DIR/Music/*.it $DEST_DIR/Music
cp $SRC_DIR/Sound/wav/*.wav $DEST_DIR/Sound/wav

cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE

for f in Sound/wav/*.wav Music/*.it
do
  echo "Adding file: " $f
  $GLUE_EXE -a $GLUE_FILE $f
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE
cd $THIS_DIR




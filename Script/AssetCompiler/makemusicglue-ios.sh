#!/bin/zsh

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

# Music - use for incidental music, TODO

mkdir -p $DEST_DIR/Sound/wav
rm $DEST_DIR/Sound/wav/*.wav
rm -f $DEST_DIR/Sound/*.sf2
cp $SRC_DIR/Sound/wav/*.wav $DEST_DIR/Sound/wav
cp $SRC_DIR/Sound/*.sf2 $DEST_DIR/Sound

rm $DEST_DIR/Songs/**/*.mid
cp -R $SRC_DIR/Songs $DEST_DIR

cd $DEST_DIR

# Create empty glue file
$GLUE_EXE -c $GLUE_FILE

for f in Sound/*.sf2 
do
  echo "Adding file: " $f
  $GLUE_EXE -a $GLUE_FILE $f
done

for f in Sound/wav/*.wav 
do
  echo "Adding file: " $f
  $GLUE_EXE -a $GLUE_FILE $f
done

for f in Songs/**/*.mid; do
  $GLUE_EXE -a $GLUE_FILE "$f"
done

# Verify glue file contents
$GLUE_EXE -d $GLUE_FILE
cd $THIS_DIR




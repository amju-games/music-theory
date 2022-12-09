# make-everything-ios.sh

export PLATFORM=iOS
export THIS_DIR=`pwd`
export TOP_DIR=$THIS_DIR/../..
export COMPILED_ASSETS=$TOP_DIR/Build/CompiledAssets/
export DEST_DIR=$COMPILED_ASSETS/$PLATFORM/

mkdir -p $COMPILED_ASSETS
mkdir -p $DEST_DIR

rm -rf $DEST_DIR/*

. makeobjsbin-ios.sh

. makeglue-ios.sh

. makemusicglue-ios.sh


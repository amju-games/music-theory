#!/bin/zsh

# 1. Setup paths (Added quotes to handle spaces safely)
export PLATFORM="iOS"
export THIS_DIR=$(pwd)
export TOP_DIR="$THIS_DIR/../.."
export COMPILED_ASSETS="$TOP_DIR/Build/CompiledAssets"
export DEST_DIR="$COMPILED_ASSETS/$PLATFORM"
export SRC_DIR="$TOP_DIR/Assets"
export GLUE_EXE="$THIS_DIR/glue"
export GLUE_FILE="$DEST_DIR/../data-$PLATFORM.glue"

# 2. Create all directories at once (-p creates parents and ignores existing)
mkdir -p "$DEST_DIR"/{Course,font2d,Gui/Palettes,Image,md2,obj,Scene,Shaders/gles,Shaders/opengl,Sound/wav,Songs}

# 3. Copy Assets (Simplified with wildcards)
cp "$SRC_DIR"/*.{txt,csv} "$DEST_DIR/" 2>/dev/null
cp -R "$SRC_DIR"/{Course,font2d,Gui,Songs,md2} "$DEST_DIR/"
cp "$SRC_DIR"/Image/*.png "$DEST_DIR/Image/"
cp "$SRC_DIR"/Shaders/gles/*.txt "$DEST_DIR/Shaders/gles/"
cp "$SRC_DIR"/Shaders/opengl/*.txt "$DEST_DIR/Shaders/opengl/"
cp "$SRC_DIR"/Scene/*.txt "$DEST_DIR/Scene/"
cp "$SRC_DIR"/obj/*.{png,mtl} "$DEST_DIR/obj/"
cp "$SRC_DIR"/obj/*.png "$DEST_DIR/" # Copying to top level as per your original script

# 4. Add EOL to all text/csv files (The "Universal" way)
# This finds every .txt and .csv file everywhere in DEST_DIR
find "$DEST_DIR" -type f \( -name "*.txt" -o -name "*.csv" \) -exec sh -c 'echo "" >> "$1"' _ {} \;

# 5. Create the Glue File
cd "$DEST_DIR"
"$GLUE_EXE" -c "$GLUE_FILE"

# 6. Add all files to Glue
# Instead of listing 50 paths, we find all files we care about:
find . -type f \( -name "*.txt" -o -name "*.csv" -o -name "*.png" -o -name "*.obj" -o -name "*.mtl" -o -path "./md2/*" \) | while read -r f; do
    # Remove the './' from the start of the filename for the glue tool
    clean_f="${f#./}"
    echo "Adding file: $clean_f"
    "$GLUE_EXE" -a "$GLUE_FILE" "$clean_f"
done

# 7. Verify and Return
"$GLUE_EXE" -d "$GLUE_FILE"
cd "$THIS_DIR"


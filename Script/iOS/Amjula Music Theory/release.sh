#!/bin/bash

# * PIANO FEST *
# * Release script for iOS *
# Juliet Colman 2026
# What this script does:
#  - Check for uncommitted changes: repo should be clean.
#  - TODO Build glue files
#  - Increment build number. 
#  - Generate Source/Version.h with version number.
#  - Builds xcode archive of game.
#  - Uploads symbols to BugSplat.
#  - TODO Use fastlane to send .ipa file to Apple App Store.

# Check for uncommitted changes
if [[ -n $(git status -s) ]]; then
  echo "⚠️  Error: You have uncommitted changes. Please commit or stash them before releasing."
  exit 1
fi

# Load secrets file - for BugSplat creds
if [ -f secrets ]; then
    export $(cat secrets | xargs)
else
    echo "❌ Error: secrets file not found!"
    exit 1
fi

# Increment build number
echo "--- 📈 Incrementing Build Number ---"
#xcrun agvtool next-version -nosave "Amjula Music Theory.xcodeproj"
# This is for XCode v. 13; the above might be better for subsequent versions. 
xcrun agvtool -noscm bump

# First we get the version string, in the format "1.2.3 (444)" 
# 1.2.3 is the "Marketing version" - what we see in the app store.
# 444 is the build number.

# Extract version string and build num from Xcode's build settings
APP_VERSION=$(xcodebuild -project "Amjula Music Theory.xcodeproj" -sdk iphoneos -scheme "Amjula Music Theory" -showBuildSettings 2>/dev/null | grep " MARKETING_VERSION = " | sed 's/.*= //')
APP_BUILD=$(xcodebuild -project "Amjula Music Theory.xcodeproj" -sdk iphoneos -scheme "Amjula Music Theory" -showBuildSettings 2>/dev/null | grep " CURRENT_PROJECT_VERSION = " | sed 's/.*= //')

# Create Version.h 
# Split "1.2.3" into variables
IFS='.' read -r MAJOR MINOR PATCH <<< "$APP_VERSION"

# Fallback for missing components (e.g., if version is just "1.0")
MAJOR=${MAJOR:-0}
MINOR=${MINOR:-0}
PATCH=${PATCH:-0}
BUILD=${APP_BUILD:-0}

# Generate the header using multiple sed replacements
sed -e "s/VERSION_TOKEN/$VER/g" \
    -e "s/MAJOR_TOKEN/$MAJOR/g" \
    -e "s/MINOR_TOKEN/$MINOR/g" \
    -e "s/PATCH_TOKEN/$PATCH/g" \
    -e "s/BUILD_TOKEN/$BUILD/g" \
    ../../../Source/Version.h.template > ../../../Source/Version.h

# Format version and build num like this: "1.2.3 (4)"
VER="$APP_VERSION ($APP_BUILD)"

echo "Extracted Version: $VER"
 
# Configuration
DB="amju-games"
APP="piano-fest"
SCHEME="Amjula Music Theory"
ARCHIVE_PATH="../../../Build/iOS/amju-piano-fest.xcarchive"

echo "--- 🛠️ Building Archive ---"
xcodebuild archive \
  -project Amjula\ Music\ Theory.xcodeproj \
  -scheme "$SCHEME" \
  -archivePath "$ARCHIVE_PATH" \
  -allowProvisioningUpdates

echo "--- 📤 Uploading Symbols to BugSplat ---"
# Path to the tool you downloaded
# If I ever get a decent mac, upgrade from intel to arm below I think.
~/Tools/BugSplat/symbol-upload-macos-intel \
  -b "$DB" \
  -a "$APP" \
  -v "$VER" \
  -d "$ARCHIVE_PATH" \
  -f "**/dSYMs/*.dSYM" \
  -i "$BUGSPLAT_ID" \
  -s "$BUGSPLAT_SECRET"

echo "--- ✅ Build and Symbol Upload Complete ---"

# --- 5. Automatic Git Commit ---
echo "--- 💾 Committing Version Bump ---"
git add .
git commit -m "Release: Bumping build number to $VER"


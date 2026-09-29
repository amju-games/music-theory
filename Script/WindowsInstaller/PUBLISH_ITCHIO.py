# PIANO FEST (c) Amju Games 2026
# Build and publish a new version of PIANO FEST to ITCH.IO
# Run this from a "Developer Command Prompt" to get msbuild, with:
#
#    python3 PUBLISH_ITCHIO.py
#
# You need these tools to be accessible too:
# * Python (of course!)
# * Git
# * Perl
# * Butler (itchio uploader)
#
# This script prompts for new version (in major.minor.patch format).
# It may well also prompt for authentication for butler and git.

import subprocess
import sys
import re
import os
import argparse
from pathlib import Path

VERSION_HEADER_PATH = "../../Source/Windows/WindowsVersion.h"
ITCH_TARGET = "amju-games/piano-fest:windows"
LOCALISE_PL = "../../../amjulib/Source/Localise/localise.pl"
EN_TXT = "../../Assets/en.txt"
ASSETS_DIR = "../../Assets"
SOURCE_DIR = "../../Source"
GAME_EXE = "amju_piano_fest.exe"

def is_valid_version(version_str):
    # Regex breakdown:
    # ^       = start of string
    # \d+     = one or more digits
    # \.      = a literal dot
    # $       = end of string
    pattern = r"^\d+\.\d+\.\d+$"    
    return bool(re.match(pattern, version_str))

def parse_args():
    parser = argparse.ArgumentParser(description="Automated release script.")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Run through the process without making permanent Git commits, tags, or pushing/uploading."
    )
    return parser.parse_args()

def run_command(cmd, check=True, capture_output=False, dry_run=False, is_side_effect=False, cwd="."):
    """
    Executes a shell command. 
    If dry_run is True and is_side_effect is True, logs the command instead of executing.
    """
    if dry_run and is_side_effect:
        print(f"--> [DRY RUN - SKIPPED]: {cmd}")
        return subprocess.CompletedProcess(args=cmd, returncode=0, stdout="", stderr="")

    print(f"--> Running: {cmd}")
    result = subprocess.run(cmd, shell=True, text=True, capture_output=capture_output, cwd=cwd)
    if check and result.returncode != 0:
        print(f"Error: Command failed with exit code {result.returncode}")
        if capture_output:
            print(f"Output: {result.stderr}")
        sys.exit(1)
    return result

def get_latest_git_tag():
    res = run_command("git describe --tags --abbrev=0", check=False, capture_output=True)
    if res.returncode == 0:
        return res.stdout.strip()
    return "v.0.0.0"

def update_version_header(version_str, dry_run=False):
    clean_ver = version_str.lstrip('v.')
    parts = clean_ver.split('.')
    major, minor, patch = parts[0], parts[1], parts[2] if len(parts) > 2 else "0"

    if dry_run:
        print(f"--> [DRY RUN - SKIPPED]: Update {VERSION_HEADER_PATH} to {clean_ver}")
        return

    if os.path.exists(VERSION_HEADER_PATH):
        with open(VERSION_HEADER_PATH, 'r') as f:
            content = f.read()

        content = re.sub(r'#define\s+VERSION_MAJOR\s+\d+', f'#define VERSION_MAJOR {major}', content)
        content = re.sub(r'#define\s+VERSION_MINOR\s+\d+', f'#define VERSION_MINOR {minor}', content)
        content = re.sub(r'#define\s+VERSION_REVISION\s+\d+', f'#define VERSION_REVISION {patch}', content)

        with open(VERSION_HEADER_PATH, 'w') as f:
            f.write(content)
        print(f"Updated {VERSION_HEADER_PATH} to {clean_ver}")
    else:
        print(f"Fatal error: Header file not found at {VERSION_HEADER_PATH}")
        sys.exit(1)

def main():
    args = parse_args()
    if args.dry_run:
        print("\n*** RUNNING IN DRY RUN MODE - No changes will be saved, tagged, or pushed ***\n")

    # Check we have got everything we need!
    run_command("git --version");
    # Localise script is in perl, until we redo it in python.
    run_command("perl --version");
    run_command("msbuild --version");
    run_command("butler --version");
    
    # 1. Clean workspace check
    status = run_command("git status --porcelain", capture_output=True).stdout.strip()
    if status and not args.dry_run:
        print("Error: Working directory is not clean. Commit or stash changes first.")
        sys.exit(1)

    # 2. Version selection
    latest_tag = get_latest_git_tag()
    print(f"Latest release tag: {latest_tag}")
    new_version = input("Enter new version (e.g., 0.3.0): ").strip()
    if not is_valid_version(new_version):
        print("Bad version! Must be <major>.<minor>.<patch>, e.g. 0.3.0.")
        sys.exit(1)
    clean_version = new_version
    new_version = f"v.{new_version}"

    # 3. Localisation check
    print("\n--- Running Localisation Checks ---")
    # Running a localisation pass should have no effect: all player-facing
    #  strings should be localised already!
    # capture_output hides the thousands of lines of spam
    run_command(f"perl {LOCALISE_PL} {EN_TXT} {ASSETS_DIR}", capture_output=True)
    run_command(f"perl {LOCALISE_PL} {EN_TXT} {SOURCE_DIR}", capture_output=True)
    loc_status = run_command("git status --porcelain", capture_output=True).stdout.strip()
    if loc_status and not args.dry_run:
        print("Error: Un-translated strings or untracked changes detected during localisation pass.")
        sys.exit(1)

    # 4. Update version header: after localisation check, so updated 
    #   version file doesn't look like a localise error.
    # Exits if header not found, that is serious!
    update_version_header(new_version, dry_run=args.dry_run)

    # 5. Build Release
    # This takes a while (~10 mins) - better to spew output so we know it's alive.
    print("\n--- Building Release ---")
    # Add capture_output to hide the thousands of lines of spam
    run_command("MakeItchioFolder.bat")

    # 6. Automated Smoke Test
    # Runs the game, takes a few minutes, you can see it working tho.
    print("\n--- Running Automated Tests ---")
    build_dir = (Path(__file__).parent / ".." / ".." / "Build" / "WindowsItchio").resolve()
    run_command(f"{GAME_EXE} --smoketest", cwd=build_dir)

    # 7. Upload to Itch.io via Butler
    # Could prompt for creds
    print("\n--- Uploading to Itch.io ---")
    run_command(
        f"butler push {build_dir.as_posix()} {ITCH_TARGET} --userversion {clean_version}",
        dry_run=args.dry_run,
        is_side_effect=True
    )

    # 8. Git Commit & Tag
    # Could prompt for creds
    print("\n--- Tagging & Pushing Release ---")
    run_command(f'git commit -am "Release {new_version}"', dry_run=args.dry_run, is_side_effect=True)
    run_command(f'git tag -a {new_version} -m "Release {new_version}"', dry_run=args.dry_run, is_side_effect=True)
    run_command("git push origin master --tags", dry_run=args.dry_run, is_side_effect=True)

    print(f"\nCompleted process for {new_version}!")

if __name__ == "__main__":
    main()


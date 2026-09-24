import subprocess
import sys
import re
import os
import argparse

VERSION_HEADER_PATH = "../../Source/Windows/WindowsVersion.h"
ITCH_TARGET = "amju-games/piano-fest:windows"
LOCALISE_PL = "../../../amjulib/Source/Localise.pl"
EN_TXT = "../../Assets/en.txt"
ASSETS_DIR = "../../Assets"
SOURCE_DIR = "../../Source"

def parse_args():
    parser = argparse.ArgumentParser(description="Automated release script.")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Run through the process without making permanent Git commits, tags, or pushing/uploading."
    )
    return parser.parse_args()

def run_command(cmd, check=True, capture_output=False, dry_run=False, is_side_effect=False):
    """
    Executes a shell command. 
    If dry_run is True and is_side_effect is True, logs the command instead of executing.
    """
    if dry_run and is_side_effect:
        print(f"--> [DRY RUN - SKIPPED]: {cmd}")
        return subprocess.CompletedProcess(args=cmd, returncode=0, stdout="", stderr="")

    print(f"--> Running: {cmd}")
    result = subprocess.run(cmd, shell=True, text=True, capture_output=capture_output)
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
    return "v0.0.0"

def update_version_header(version_str, dry_run=False):
    clean_ver = version_str.lstrip('v')
    parts = clean_ver.split('.')
    print(parts);
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

    # 1. Clean workspace check
    status = run_command("git status --porcelain", capture_output=True).stdout.strip()
    if status and not args.dry_run:
        print("Error: Working directory is not clean. Commit or stash changes first.")
        sys.exit(1)

    # 2. Version selection
    latest_tag = get_latest_git_tag()
    print(f"Latest release tag: {latest_tag}")
    new_version = input("Enter new version (e.g., 0.3.0 or v0.3.0): ").strip()
    if not new_version.startswith('v'):
        new_version = f"v{new_version}"

    # 3. Update version header
    # Exits if header not found, that is serious!
    update_version_header(new_version, dry_run=args.dry_run)

    # 4. Localisation check
    print("\n--- Running Localisation Checks ---")
    # Running a localisation pass should have no effect: all player-facing
    #  strings should be localised already!
    run_command(f"perl {LOCALISE_PL} {EN_TXT} {ASSETS_DIR}")
    run_command(f"perl {LOCALISE_PL} {EN_TXT} {SOURCE_DIR}")
    loc_status = run_command("git status --porcelain", capture_output=True).stdout.strip()
    if loc_status and not args.dry_run:
        print("Error: Un-translated strings or untracked changes detected during localization pass.")
        sys.exit(1)

    # 5. Build Release
    print("\n--- Building Release ---")
    run_command("MakeItchioFolder.bat")

    # 6. Automated Smoke Test
    print("\n--- Running Automated Tests ---")
    run_command("../../Build/WindowsItchio/amju_piano_fest.exe --smoketest")

    # 7. Upload to Itch.io via Butler
    print("\n--- Uploading to Itch.io ---")
    run_command(
        f"butler push ./build/Release {ITCH_TARGET} --userversion {new_version}",
        dry_run=args.dry_run,
        is_side_effect=True
    )

    # 8. Git Commit & Tag
    print("\n--- Tagging & Pushing Release ---")
    run_command(f'git commit -am "Release {new_version}"', dry_run=args.dry_run, is_side_effect=True)
    run_command(f'git tag -a {new_version} -m "Release {new_version}"', dry_run=args.dry_run, is_side_effect=True)
    run_command("git push origin master --tags", dry_run=args.dry_run, is_side_effect=True)

    print(f"\nCompleted process for {new_version}!")

if __name__ == "__main__":
    main()


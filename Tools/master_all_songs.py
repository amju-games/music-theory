# master_all_songs.py
import sys
from pathlib import Path
from master_midi import master_file

def batch_master_directory(songs_dir_path, settings_json_path):
    songs_dir = Path(songs_dir_path)
    settings_file = Path(settings_json_path)
    
    if not songs_dir.is_dir():
        sys.exit(f"❌ Songs directory not found at: {songs_dir}")
    if not settings_file.is_file():
        sys.exit(f"❌ Settings profile file not found at: {settings_file}")
        
    print(f"🚀 Batch Masterer initialized targeting: {songs_dir.resolve()}")
    print(f"🎛️  Applying volume profile: {settings_file.name}\n")
    
    mastered_count = 0
    
    # Recursively find all MIDI files in the directory
    for midi_path in songs_dir.rglob("*.mid"):
        # Crucial Skip: We do NOT master score files, only performance/audio targets
        if "-score" in midi_path.name or "_with_perc" in midi_path.name:
            continue
            
        print(f" ⏳ Processing -> {midi_path.relative_to(songs_dir)}")
        
        # Overwrite file inplace with optimized volume ranges
        success = master_file(str(midi_path), str(settings_file))
        if success:
            mastered_count += 1

    print(f"\n🎉 Finished! Successfully leveled and humanized {mastered_count} track variants.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 master_all_songs.py <Songs_Dir> <settings.json>")
        sys.exit(1)
        
    batch_master_directory(sys.argv[1], sys.argv[2])


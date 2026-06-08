# master_midi.py
import sys
import json
import mido
import random
from pathlib import Path

def humanize_asymmetric(center, min_delta, max_delta):
    """Calculates absolute bounds relative to the center, then humanizes."""
    # Ensure deltas move in the correct mathematical direction
    abs_min = center + min_delta  # e.g., 90 + (-10) = 80
    abs_max = center + max_delta  # e.g., 90 + (10)  = 100
    
    # Clamp absolute bounds to valid MIDI velocity limits (1-127)
    abs_min = max(1, min(127, abs_min))
    abs_max = max(1, min(127, abs_max))

    if random.random() < 0.5:
        return int(random.uniform(abs_min, center))
    else:
        return int(random.uniform(center, abs_max))

def master_file(input_midi, settings_path, output_midi=None):
    try:
        mid = mido.MidiFile(input_midi)
        with open(settings_path, 'r') as f:
            settings = json.load(f)
    except Exception as e:
        print(f"  ❌ Error loading files: {e}")
        return False

    # --- NEW: Map Channel Numbers directly to their Profile Configurations ---
    # Re-indexes the JSON data so we can look it up instantly via msg.channel
    channel_map = {}
    for instrument_name, cfg in settings.items():
        ch_num = cfg.get("channel")
        if ch_num is not None:
            channel_map[int(ch_num)] = cfg

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' and msg.velocity > 0:
                cfg = channel_map.get(msg.channel)
                
                if cfg:
                    # Apply relative delta calculations
                    new_vel = humanize_asymmetric(
                        center=cfg['center'],
                        min_delta=cfg.get('min_delta', 0),
                        max_delta=cfg.get('max_delta', 0)
                    )
                    msg.velocity = new_vel

    if not output_midi:
        output_midi = input_midi
        
    mid.save(output_midi)
    return True

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 master_midi.py <input.mid> <settings.json> [output.mid]")
        sys.exit(1)
        
    in_mid = sys.argv[1]
    cfg_json = sys.argv[2]
    out_mid = sys.argv[3] if len(sys.argv) == 4 else None
    
    if master_file(in_mid, cfg_json, out_mid):
        print(f"✅ Mastered: {in_mid}")


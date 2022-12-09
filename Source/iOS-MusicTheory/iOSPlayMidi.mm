// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#import <MIKMIDI/MIKMIDI.h>

// TEST
#define SOUND_FONT "Grand Piano"
//#define SOUND_FONT "ChoriumRevA"
//#define SOUND_FONT "velocity_grand_piano"

static MIKMIDISynthesizer* Synthesizer()
{
  static MIKMIDISynthesizer* _synthesizer = nullptr;
  
  if (!_synthesizer) {
    _synthesizer = [[MIKMIDISynthesizer alloc] init];
    
    NSURL *soundfont = [[NSBundle mainBundle] URLForResource:@"/" SOUND_FONT withExtension:@"sf2"];
    NSError *error = nil;
    if (![_synthesizer loadSoundfontFromFileAtURL:soundfont error:&error]) {
      NSLog(@"Error loading soundfont for synthesizer. Sound will be degraded. %@", error);
    }
  }
  return _synthesizer;
}

void PlayMidi(int note, int vol)
{
  MIKMIDINoteOnCommand *noteOn = [MIKMIDINoteOnCommand 
    noteOnCommandWithNote:note 
    velocity:vol 
    channel:0 
    timestamp:[NSDate date]];

  [Synthesizer() handleMIDIMessages:@[noteOn]];
}


#!/usr/bin/perl -w

#############################################
## AMT LOG PLAY SESSION
#############################################

# Expects these params:
# device_id: unique ID for a device
# session_id: unique ID on the device
# session_start: timestamp
# session_end: timestamp
# session_level: int, level we are on during this session (changing levels will create 2 sessions)
# session_depth: int, depth when we finished session
# session_score: int, score when we finished session
# session_flags: int: died, did complete
# session_user_nick: string, last name entered by user, if any. Can be empty.

require "common.pl";

# Sanitise params and connect to DB
my_connect();

sub log_session();
log_session();


sub log_session()
{
  my $device_id = param('device_id') or die "Expected device_id\n";
  my $session_start = param('session_start') or die "Expected session_start\n";
  my $session_end = param('session_end') or die "Expected session_end\n";
  my $session_level = param('session_level') or die "Expected session_level\n";
  my $session_depth = param('session_depth') or die "Expected session_depth\n";
  my $session_score = param('session_score') or die "Expected session_score\n";
  my $session_flags = param('session_flags') or die "Expected session_flags\n";
  my $session_lives = param('session_lives') or die "Expected session_lives\n";

  # Decode coded params
  my $dec_device_id = pack('H*', "$device_id");
  my $dec_session_start = pack('H*', "$session_start");
  my $dec_session_end = pack('H*', "$session_end");
  my $dec_session_level = pack('H*', "$session_level");
  my $dec_session_depth = pack('H*', "$session_depth");
  my $dec_session_score = pack('H*', "$session_score");
  my $dec_session_flags = pack('H*', "$session_flags");
  my $dec_session_lives = pack('H*', "$session_lives");

  print "device_id: \"$dec_device_id\"\n";

  my $sql = <<END;
  INSERT INTO session (
    device_id, 
    session_start,
    session_end,
    session_level,
    session_depth,
    session_score,
    session_lives,
    session_flags)
    VALUES (
    '$dec_device_id',
    FROM_UNIXTIME($dec_session_start), 
    FROM_UNIXTIME($dec_session_end), 
    '$dec_session_level', 
    '$dec_session_depth', 
    '$dec_session_score', 
    '$dec_session_lives', 
    '$dec_session_flags'
    );
END

  print "Query: $sql\n";
  insert($sql);
}



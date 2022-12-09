#!/usr/bin/perl -w

#############################################
## AMT LOG ATTEMPT 
#############################################

# Expects these params:
# device_id: unique ID for a device
# attempt_start: timestamp
# attempt_end: timestamp
# attempt_topic: string, level ID 
# attempt_score: int, score when we finished attempt
# attempt_flags: int: died, did complete
# attempt_lives: int: number of lives at attempt end

require "common.pl";

# Sanitise params and connect to DB
my_connect();

sub log_attempt();
log_attempt();


sub log_attempt()
{
  my $device_id = param('device_id') or die "Expected device_id\n";
  my $attempt_start = param('attempt_start') or die "Expected attempt_start\n";
  my $attempt_end = param('attempt_end') or die "Expected attempt_end\n";
  my $attempt_topic = param('attempt_topic') or die "Expected attempt_topic\n";
  my $attempt_score = param('attempt_score') or die "Expected attempt_score\n";
  my $attempt_flags = param('attempt_flags') or die "Expected attempt_flags\n";
  my $attempt_lives = param('attempt_lives') or die "Expected attempt_lives\n";

  # Decode coded params
  my $dec_device_id = pack('H*', "$device_id");
  my $dec_attempt_start = pack('H*', "$attempt_start");
  my $dec_attempt_end = pack('H*', "$attempt_end");
  my $dec_attempt_topic = pack('H*', "$attempt_topic");
  my $dec_attempt_score = pack('H*', "$attempt_score");
  my $dec_attempt_flags = pack('H*', "$attempt_flags");
  my $dec_attempt_lives = pack('H*', "$attempt_lives");

  my $sql = <<END;
  INSERT INTO attempt (
    device_id, 
    attempt_start,
    attempt_end,
    attempt_topic,
    attempt_score,
    attempt_lives,
    attempt_flags)
    VALUES (
    '$dec_device_id',
    FROM_UNIXTIME($dec_attempt_start), 
    FROM_UNIXTIME($dec_attempt_end), 
    '$dec_attempt_topic', 
    '$dec_attempt_score', 
    '$dec_attempt_lives', 
    '$dec_attempt_flags'
    );
END

  insert($sql);
}



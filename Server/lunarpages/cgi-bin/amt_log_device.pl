#!/usr/bin/perl -w

#############################################
## RD LOG DEVICE
#############################################

# Expects these params:
# device_id: unique ID for a device
# device_user_name: name given to device by user
# device_model: e.g. "iPad"
# device_os_version: e.g. "9.2.3"
# device_manufacturer, e.g. "Apple"
# device_client_version, e.g. "1.1"

require "common.pl";

# Sanitise params and connect to DB
my_connect();

sub log_device();
log_device();


sub log_device()
{
  my $device_id = param('device_id') or die "Expected device_id\n";
  my $device_user_name = param('device_user_name') or die "Expected device_user_name\n";
  my $device_model = param('device_model') or die "Expected device_model\n";
  my $device_os_version = param('device_os_version') or die "Expected device_os_version\n";
  my $device_manufacturer = param('device_manufacturer') or die "Expected device_manufacturer\n";
  my $device_client_version = param('device_client_version') or die "Expected device_client_version\n";
  my $device_ip = $ENV{REMOTE_ADDR};

  # Decode coded params
  my $dec_device_id = pack('H*', "$device_id");
  my $dec_device_user_name = pack('H*', "$device_user_name");
  my $dec_device_model = pack('H*', "$device_model");
  my $dec_device_os_version = pack('H*', "$device_os_version");
  my $dec_device_manufacturer = pack('H*', "$device_manufacturer");
  my $dec_device_client_version = pack('H*', "$device_client_version");

  print "device_id: \"$dec_device_id\"\n";
  print "device_user_name: \"$dec_device_user_name\"\n";
  print "device_model: \"$dec_device_model\"\n";
  print "device_os_version: \"$dec_device_os_version\"\n";
  print "device_manufacturer: \"$dec_device_manufacturer\"\n";
  print "device_client_version: \"$dec_device_client_version\"\n";
  print "device_ip: \"$device_ip\"\n";

  my $sql = <<END;
  INSERT INTO device (
    device_id, 
    device_user_name, 
    device_model, 
    device_os_version, 
    device_manufacturer, 
    device_client_version, 
    device_ip,
    device_when_logged,
    device_num_updates) 
    VALUES (
    '$dec_device_id',
    '$dec_device_user_name', 
    '$dec_device_model', 
    '$dec_device_os_version', 
    '$dec_device_manufacturer', 
    '$dec_device_client_version', 
    '$device_ip',
    FROM_UNIXTIME(NOW()),
    0) 
    ON DUPLICATE KEY UPDATE
    device_user_name = '$dec_device_user_name', 
    device_os_version = '$dec_device_os_version', 
    device_client_version = '$dec_device_client_version', 
    device_ip = '$device_ip',
    device_num_updates = device_num_updates + 1;
END

  print "Query: $sql\n";
  update_or_insert($sql);
}



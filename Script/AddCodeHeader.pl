#!/usr/bin/perl -w

#
# Amju Games copyright (c) Juliet Colman 2026
# Add comment block to top of C++ source and header files,
#   and txt files (e.g. Assets/Gui.)
# 
# Arguments:
#  - Directory in which to start looking for source files
#     - not recursive
#  - Optionally, "nowrite" - don't write modified file
#
# Usually run like this:
#   ./AddCommentBlock.pl ../../Code [nowrite]
#   perl AddCommentBlock.pl ../../Code [nowrite]
#

use File::Find;
use strict;

my $COMMENT_BLOCK = "// * Amju PIANO FEST *\n// (c) Copyright Juliet Colman 2000-2026\n\n";

sub AddHeaders($)
{
  # Read file to modify
  # -------------------

  my $fileToModify = shift; 
  if (!open(MODIFY_THIS, $fileToModify))
  {
    print "Can't open specified file to modify: $fileToModify\n";
    return;
  }

  my @lines = <MODIFY_THIS>;
  close (MODIFY_THIS);

  # Check for "/*" on top line.
  # If found, set flag. 
  # If flag is set, delete each line until
  # we find "*/". 
  # Then add new comment block to top of file.
 
  my $lineNum = 0;
  my $wasChanged = 0;
  my $foundCommentBlock = 0;

  foreach my $line (@lines)
  {
    # Remove newline characters
    chomp($line);

    # Break when we find the first line which is blank or not a comment

    if (($line =~ /\/\*/ and $line =~ /\*\//) or $line =~ /^\/\//)
    {
      print "Found one-line comment: line ", $lineNum+1, "\n";
      $foundCommentBlock = 0;
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
      ###last; # like break in C++
    }
    elsif ($line =~ /\/\*/)
    {
      print "Found comment block start, line ", $lineNum+1, "\n";
      $foundCommentBlock = 1;
    }
    elsif ($line =~ /\*\//)
    {
      print "Found end of comment block, line ", $lineNum+1, "\n";
      $foundCommentBlock = 0;
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
      #last; # like break in C++ -- i.e. we are done
    }
    elsif ($line eq "")
    {
      print "Found empty line, line ", $lineNum+1, "\n";
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
    }
    elsif ($foundCommentBlock == 0)
    {
      # not comment or blank
      print "Found code, line ", $lineNum+1, "\n";
      last; 
    }

    if ($foundCommentBlock == 1)
    {
      $lines[$lineNum] = "**DELETE_ME**";    
      $wasChanged = 1;
    }
    
    $lineNum++;
  }

  if (!$wasChanged)
  {
    print "No existing comment block found!?!\n";
  }

  if ($lines[0] eq "**DELETE_ME**")
  {
    $lines[0] = $COMMENT_BLOCK;
  }
  else
  {
    $lines[0] = $COMMENT_BLOCK . $lines[0];
  }



  # Print new verison of file 
  # -------------------------
  #
  print "\n\nNEW FILE:\n";
  foreach my $line (@lines)
  {
    # Remove newline characters
    chomp($line);

    if ($line ne "**DELETE_ME**")
    {
      print "$line\n";
    }
  }
  print "\n\n";

  if ($ARGV[1] eq "nowrite")
  {
    return;
  }

  print "WRITING OUTPUT TO FILE $fileToModify\n";

  # Print lines to file
  open(MODIFY_THIS, ">$fileToModify");
  # Can't use $line again
  foreach my $line2 (@lines)
  {
    # Remove newline characters
    chomp($line2);

    if ($line2 ne "**DELETE_ME**")
    {
      print MODIFY_THIS "$line2" . $/;
    }
  }
  close (MODIFY_THIS);
}

# START HERE

my $target = $ARGV[0] // '.';

if (-f $target) {
    # If the argument is a single file, process it directly
    print "FOUND FILE: $target\n";
    AddHeaders($target);
}
elsif (-d $target) {
    # If the argument is a directory, glob for .cpp, .h, and .txt files
    for my $file (glob("$target/*.{cpp,h,txt}")) {
        if (-f $file) {
            print "FOUND FILE: $file\n";
            AddHeaders($file);
        }
    }
}
else {
    die "Error: '$target' is neither a valid file nor a directory.\n";
}


// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

namespace Amju
{
// NetSend* functions: send info to the server (and mostly don't need to care
//  about a response, which keeps things nice and simple).
  
// Call when there is no config file, so this is a fresh install.
// Sends device info to server with a unique device ID.
bool NetSendDeviceInfoFirstRunEver();
  
// Call when we start up, and there is a config file, so not the first run.
// But something could have changed which we should update on the server.
// E.g. client (i.e. this exe) version, OS version, etc.
bool NetSendUpdateDeviceInfo();

// Call when a new play session starts, i.e. when Play state activates
void NetSendMarkAttemptStart(const std::string& topicId);
  
// Call when play session ends. Pass in flags depending on whether we got killed
//  or completed the level, etc.
bool NetSendAttempt(const std::string& topicId, int flags, int score, int lives);

// Flag values for above function.
const int NET_SEND_ATTEMPT_QUIT = 1;
const int NET_SEND_ATTEMPT_COMPLETE = 2;
const int NET_SEND_ATTEMPT_NO_LIVES_LEFT = 4;
const int NET_SEND_ATTEMPT_CHEAT = 8;
// (More flags as required)
  

}


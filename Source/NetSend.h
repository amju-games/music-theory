// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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
void NetSendMarkSessionStart();
  
// Call when play session ends. Pass in flags depending on whether we got killed
//  or completed the level, etc.
bool NetSendPlaySession(int flags = 0);

// Flag values for above function.
// Zero means the session finished but player did not die or complete level, so
//  session interrupted.
const int NET_SEND_PLAYER_DIED = 1;
const int NET_SEND_LEVEL_COMPLETE = 2;
// (More flags as required)
  

}


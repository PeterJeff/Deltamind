#pragma once


#include "MessageEvents.h"
void Death();
void SendMessge(void) {
	MessageEvents::Subscribe(EVENT_Player_Death, Death);
	MessageEvents::SendMessage(EVENT_Player_Death);
}
void Death(void) {
	WriteLine("Death");
}

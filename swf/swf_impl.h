#pragma once

class SWFEvent
{
public:
  // NOTE: DO NOT USE THESE AS VALUE TYPES IN AN
  // array<>!  They cannot be moved!  The private
  // operator=(const swf_event&) should help guard
  // against that.

  event_id	event;
  as_value	method;

  SWFEvent() {}

private:
  // DON'T USE THESE
  SWFEvent(const SWFEvent& s) { ASSERT(0); }
  void	operator=(const SWFEvent& s) { ASSERT(0); }
};

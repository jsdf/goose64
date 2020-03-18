#ifndef _TRACE_H_
#define _TRACE_H_

#include "constants.h"

#define TRACE_EVENT_BUFFER_SIZE 10000

typedef enum TraceEventType {
  FrameTraceEvent,
  CharactersUpdateTraceEvent,
  PhysUpdateTraceEvent,
  PathfindingTraceEvent,
  DrawTraceEvent,
  DrawSortTraceEvent,
  DrawIterTraceEvent,
  PhysWorldCollisionTraceEvent,
  PhysObjCollisionTraceEvent,
  RSPTasksTraceEvent,
  RDPTasksTraceEvent,
  SkippedGfxTaskTraceEvent,
  UpdateTraceEvent,
  CollisionGetTrianglesTraceEvent,
  MainUpdateTraceEvent,
  MainMakeDisplayListTraceEvent,
  DebugDrawTraceEvent,
  MAX_TRACE_EVENT_TYPE,
} TraceEventType;

typedef struct TraceEvent {
  short type;
  float start;
  float end;
} TraceEvent;

extern TraceEvent traceEvents[TRACE_EVENT_BUFFER_SIZE];

extern char* TraceEventTypeStrings[];

void Trace_addEvent(short type, float start, float end);
void Trace_clear();
int Trace_isFull();
void Trace_start();
void Trace_stop();
int Trace_getEventsCount();
int Trace_isTracing();

#endif /* !_TRACE_H_ */

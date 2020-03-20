#ifndef _TRACE_H_
#define _TRACE_H_

#include "constants.h"

#define TRACE_EVENT_BUFFER_SIZE 10000

typedef enum TraceEventType {
  FrameTraceEvent,
  SkippedGfxTaskTraceEvent,
  MainCPUTraceEvent,
  MainMakeDisplayListTraceEvent,
  MainUpdateTraceEvent,
  RSPTaskTraceEvent,
  RDPTaskTraceEvent,
  CharactersUpdateTraceEvent,
  PathfindingTraceEvent,
  DrawTraceEvent,
  DrawSortTraceEvent,
  DrawIterTraceEvent,
  PhysUpdateTraceEvent,
  PhysWorldCollisionTraceEvent,
  PhysObjCollisionTraceEvent,
  CollisionGetTrianglesTraceEvent,
  CollisionTestMeshSphereTraceEvent,
  DebugDrawTraceEvent,
  DrawAnimTraceEvent,
  AnimLerpTraceEvent,
  MAX_TRACE_EVENT_TYPE,
} TraceEventType;

typedef struct TraceEvent {
  short type;
  float start;
  float end;
} TraceEvent;

extern TraceEvent traceEvents[TRACE_EVENT_BUFFER_SIZE];

void Trace_addEvent(short type, float start, float end);
void Trace_clear();
int Trace_isFull();
void Trace_start();
void Trace_stop();
int Trace_getEventsCount();
int Trace_isTracing();

#endif /* !_TRACE_H_ */

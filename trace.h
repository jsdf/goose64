#ifndef _TRACE_H_
#define _TRACE_H_

#include "constants.h"

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
  MAX_TRACE_EVENT_TYPE,
} TraceEventType;

extern char* TraceEventTypeStrings[];
extern float traceEventDurations[MAX_TRACE_EVENT_TYPE];
extern float traceEventStarts[MAX_TRACE_EVENT_TYPE];

#endif /* !_TRACE_H_ */

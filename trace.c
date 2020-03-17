#include "trace.h"

char* TraceEventTypeStrings[] = {
    "FrameTraceEvent",
    "CharactersUpdateTraceEvent",
    "PhysUpdateTraceEvent",
    "PathfindingTraceEvent",
    "DrawTraceEvent",
    "DrawSortTraceEvent",
    "DrawIterTraceEvent",
    "PhysWorldCollisionTraceEvent",
    "PhysObjCollisionTraceEvent",
    "RSPTasksTraceEvent",
    "RDPTasksTraceEvent",
    "SkippedGfxTaskTraceEvent",
    "UpdateTraceEvent",
    "MAX_TRACE_EVENT_TYPE",
};

TraceEvent traceEvents[TRACE_EVENT_BUFFER_SIZE];
int traceEventsCount = 0;
int tracingEnabled = FALSE;

void Trace_addEvent(short type, float start, float end) {
  if (!tracingEnabled || traceEventsCount == TRACE_EVENT_BUFFER_SIZE) {
    return;
  }
  traceEvents[traceEventsCount].type = type;
  traceEvents[traceEventsCount].start = start;
  traceEvents[traceEventsCount].end = end;
  traceEventsCount++;
}
void Trace_clear() {
  traceEventsCount = 0;
}

int Trace_isFull() {
  if (traceEventsCount == TRACE_EVENT_BUFFER_SIZE) {
    return TRUE;
  }
  return FALSE;
}

void Trace_start() {
  tracingEnabled = TRUE;
}
void Trace_stop() {
  tracingEnabled = FALSE;
}
int Trace_getEventsCount() {
  return traceEventsCount;
}

int Trace_isTracing() {
  return tracingEnabled;
}
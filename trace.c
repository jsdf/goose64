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

float traceEventDurations[MAX_TRACE_EVENT_TYPE];
float traceEventStarts[MAX_TRACE_EVENT_TYPE];

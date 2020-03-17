import sys
import json


event_names = [
    # copy from trace.c
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
]


line_prefix = "TRACE="

output = []
with open(sys.argv[1]) as fp:
    line = fp.readline()
    while line:
        line = fp.readline()
        if line.startswith("TRACE="):
            try:
                line_slice = line[len(line_prefix) :]
                frame_packed = json.loads(line_slice)
            except ValueError:  # includes simplejson.decoder.JSONDecodeError
                print("failed to decode line", line_slice)
                continue

            for event in frame_packed:
                start, end, event_type_index = event
                if start > 0:
                    output.append(
                        {
                            "pid": event_type_index,
                            "tid": 0,
                            "name": event_names[event_type_index],
                            "cat": event_names[event_type_index],
                            "ph": "X",
                            "ts": start * 1000,
                            "dur": (end - start) * 1000,
                        }
                    )
for event_type_index, event_name in enumerate(event_names):
    output.append(
        {
            "pid": event_type_index,
            "tid": 0,
            "ph": "M",
            "cat": "__metadata",
            "name": "thread_name",
            "args": {"name": event_names[event_type_index]},
        }
    )


with open(sys.argv[2], "w") as outfile:
    json.dump(output, outfile)

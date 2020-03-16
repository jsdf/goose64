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
frame_num = 0
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

            frame = {}
            for packed_value in frame_packed:
                event_name_index = packed_value[2]
                frame[event_names[event_name_index]] = packed_value

            line = fp.readline()
            for event_type, event in frame.items():
                if event[0] > 0:
                    output.append(
                        {
                            "pid": event_names.index(event_type),
                            "name": event_type + (" %d" % frame_num),
                            "ph": "X",
                            "ts": event[0] * 1000,
                            "dur": event[1] * 1000,
                        }
                    )
            frame_num += 1


with open(sys.argv[2], "w") as outfile:
    json.dump(output, outfile)

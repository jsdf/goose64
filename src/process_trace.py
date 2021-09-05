import sys
import json
import re

event_names = None


with open("trace.h") as fp:
    file_text = fp.read()
    matches = re.search(r"TraceEventType\s+{([^}]+?)}", file_text)
    assert matches

    parts = matches.group(1).split(",")
    event_names = [part.strip().replace("TraceEvent", "") for part in parts]
    event_names = [name for name in event_names if name != ""]

assert event_names

line_prefix = "TRACE="

output = []
unique_events = set()

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

                # remove duplicate events
                event_key = "%d/%f/%f" % (event_type_index, start, end)
                if event_key in unique_events:
                    continue
                unique_events.add(event_key)

                # remove invalid events
                if start <= 0:
                    continue

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

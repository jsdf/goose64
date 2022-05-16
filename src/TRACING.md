how to use tracing

set defines in stage00.c:
```
CONSOLE_SHOW_TRACING 1
LOG_TRACES 1
```

build with ED64=1

deploy with 

```
PROFILE=1 CATAPULT=~/code/catapult/ ./deploy.sh
```

when ready to trace, press c-up to start tracing.

press c-up to stop tracing (will happen automatically if trace buffer is full). wait for logging to finish (see tracing info on screen).

kill ed64log with ctrl-c. trace will open automatically

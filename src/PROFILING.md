
clone catapult

```
git clone https://chromium.googlesource.com/catapult

```

catapult needs python 2, so on mac you might need to do
```
brew reinstall python@2
```

run game with this command, replacing `/path/to/catapult/dir` with the location you checked out the catapult repo:

```
PROFILE=1 CATAPULT=/path/to/catapult/dir deploy.sh

# eg.
PROFILE=1 CATAPULT=~/code/catapult ./deploy.sh
```

do stuff in the game


use ctrl+c to kill logger. the trace will then be processed and opened in the browser

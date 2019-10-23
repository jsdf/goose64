

### mechanics
- people have a default activity that they're trying to do (walking somewhere, meditating, selling items at a bake sale)
- people have items which belong to them, which they will try to recover when lost
- you can chase some (meek) people by honking. some will fight back
- you can steal people's items (but non-meek people will take them back)
- you have a todo list to complete
  - soak the term paper
  - steal the professor's briefcase
  - chase away the yoga class
  - ruin the bake sale

### stuff to build
- item pickup/drop
- people's default activities
- people return item to original location
- indicators of people's states

### nice to have
- dynamic music
 
### todo
-



### AI character states
in descending order or priority (lower priority states won't be entered from higher priority states)
- fleeing goose
  - entered when a meek character is honked at enough times from close range
  - run away from goose
  - exit after some time or out of range of goose
- seeking source of sound
  - entered when hearing a sound
  - go to location of sound
  - if goose is seen
    - enter 'seen goose' state
    - after a short time return to idle
  - upon getting to location of sound (without seeing goose)
    - enter confused state
    - after a short time, return to idle
- seeking stolen item
  - entered when seeing a stolen owned item (in visual area)
  - go to item location
  - when in range, pickup item
  - go to item home location
  - drop item
  - return to idle
- default activity
  - go to location of default activity
  - when in range, do default activity
- idle state
  - enter eligable state of highest priority

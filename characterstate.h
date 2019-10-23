
#ifndef CHARACTERSTATE_H
#define CHARACTERSTATE_H

typedef enum CharacterState {
  IdleState,
  DefaultActivityState,
  SeekingItemState,
  SeekingSoundSourceState,
  FleeingState,
  MAX_CHARACTER_STATE
} CharacterState;

#ifndef __N64__
extern char* CharacterStateStrings[];
#endif

#endif /* !CHARACTERSTATE_H */

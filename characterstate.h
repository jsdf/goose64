
#ifndef CHARACTERSTATE_H
#define CHARACTERSTATE_H

typedef enum CharacterState {
  IdleState,
  ConfusionState,
  DefaultActivityState,
  SeekingItemState,
  SeekingSoundSourceState,
  FleeingState,
  MAX_CHARACTER_STATE
} CharacterState;

extern char* CharacterStateStrings[];

#endif /* !CHARACTERSTATE_H */


#ifndef CHARACTERSTATE_H
#define CHARACTERSTATE_H

typedef enum CharacterState {
  IdleState,
  ConfusionState,
  DefaultActivityState,
  SeekingLastSeenState,
  SeekingItemState,
  SeekingSoundSourceState,
  FleeingState,
  MAX_CHARACTER_STATE
} CharacterState;

typedef enum CharacterTarget {
  NoneCharacterTarget,
  HonkCharacterTarget,
  ItemCharacterTarget,
  MAX_CHARACTER_TARGET
} CharacterTarget;

extern char* CharacterStateStrings[];
extern char* CharacterTargetStrings[];

#endif /* !CHARACTERSTATE_H */

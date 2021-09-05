#include "characterstate.h"

char* CharacterStateStrings[] = {
    "IdleState",                //
    "ConfusionState",           //
    "DefaultActivityState",     //
    "SeekingLastSeenState",     //
    "SeekingItemState",         //
    "SeekingSoundSourceState",  //
    "FleeingState",             //
    "MAX_CHARACTER_STATE",      //
};

char* CharacterTargetStrings[] = {
    "NoneCharacterTarget",
    "HonkCharacterTarget",
    "ItemCharacterTarget",
    "MAX_CHARACTER_TARGET",
};
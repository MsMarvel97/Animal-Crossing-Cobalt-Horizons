#include "MinigameQuestFlags.h"

void MinigameQuestFlags::SetChaos(bool flag)
{
    cafeChaos = flag;
}

bool MinigameQuestFlags::GetChaos()
{
    return cafeChaos;
}

void MinigameQuestFlags::SetFruit(bool flag)
{
    fruitShakedown = flag;
}

bool MinigameQuestFlags::GetFruit()
{
    return fruitShakedown;
}

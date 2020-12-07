#include "EndConditions.h"
#include <iostream>

void EndConditions::SetStevieFlag(bool flag)
{
    stevieFlag = flag;
    std::cout << "Stevie flag" << std::endl;
}

bool EndConditions::GetStevieFlag()
{
    return stevieFlag;
}

void EndConditions::SetKyraFlag(bool flag)
{
    kyraFlag = flag;
    std::cout << "Kyra flag" << std::endl;
}

bool EndConditions::GetKyraFlag()
{
    return kyraFlag;
}

void EndConditions::SetKainatFlag(bool flag)
{
    kainatFlag = flag;
    std::cout << "Kainat flag" << std::endl;
}

bool EndConditions::GetKainatFlag()
{
    return kainatFlag;
}

void EndConditions::SetMithunanFlag(bool flag)
{
    mithunanFlag = flag;
    std::cout << "Mithunan flag" << std::endl;
}

bool EndConditions::GetMithunanFlag()
{
    return mithunanFlag;
}

void EndConditions::SetWinstonFlag(bool flag)
{
    winstonFlag = flag;
    std::cout << "Winston flag" << std::endl;
}

bool EndConditions::GetWinstonFlag()
{
    return winstonFlag;
}

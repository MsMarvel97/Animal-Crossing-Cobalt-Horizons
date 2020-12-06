#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
//int MainEntities::m_stunIcon = 0;
//int MainEntities::m_whack = 0;

int MainEntities::MainCamera()
{
	return m_mainCamera;
}

int MainEntities::MainPlayer()
{
	return m_mainPlayer;
}

//int MainEntities::StunIcon()
//{
//	return m_stunIcon;
//}
//
//int MainEntities::Whack()
//{
//	return m_whack;
//}

void MainEntities::MainCamera(int main)
{
	m_mainCamera = main;
}

void MainEntities::MainPlayer(int main)
{
	m_mainPlayer = main;
}

//void MainEntities::SetStunIcon(int stun)
//{
//	m_stunIcon = stun;
//}
//
//void MainEntities::SetWhack(int whack)
//{
//	m_whack = whack;
//}

void MainEntities::ResetEntities()
{
	m_mainPlayer = 0;//resets to default state
	m_mainCamera = 0;
	//m_stunIcon = 0;
	//m_whack = 0;
}

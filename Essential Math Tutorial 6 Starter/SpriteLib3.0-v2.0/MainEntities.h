#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	//static int StunIcon();
	//static int Whack();

	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);

	//static void SetStunIcon(int stun);
	//static void SetWhack(int whack);

	static void ResetEntities();

private:
	static int m_mainCamera;
	static int m_mainPlayer;
	//static int m_stunIcon;
	//static int m_whack;
};

#endif // !__MAINENTITIES_H__

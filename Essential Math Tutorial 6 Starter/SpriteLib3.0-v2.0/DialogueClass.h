#pragma once
class DialogueClass
{
public:
	void SetKainat(bool check);//setter
	bool GetKainat();//getter

	void SetKyra(bool check);//setter
	bool GetKyra();//getter

	void SetMithunan(bool check);//setter
	bool GetMithunan();//getter

	void SetStevie(bool check);//setter
	bool GetStevie();//getter

	void SetWinston(bool check);//setter
	bool GetWinston();//getter

	void SetBookOne(bool check);//setter
	bool GetBookOne();//getter

	void SetBookTwo(bool check);//setter
	bool GetBookTwo();//getter

	void SetBookThree(bool check);//setter
	bool GetBookThree();//getter

protected:
	bool m_kainat = false;
	bool m_kyra = false;
	bool m_mithunan = false;
	bool m_stevie = false;
	bool m_winston = false;
	bool m_bookOne = false;
	bool m_bookTwo = false;
	bool m_bookThree = false;

	int m_bookCount = 0;

};


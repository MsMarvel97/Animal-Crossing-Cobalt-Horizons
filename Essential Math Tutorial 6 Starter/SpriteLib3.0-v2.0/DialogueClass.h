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

	//Stevie's lost items
	void SetBookOne(bool check);//setter
	bool GetBookOne();//getter

	void SetBookTwo(bool check);//setter
	bool GetBookTwo();//getter

	void SetBookThree(bool check);//setter
	bool GetBookThree();//getter

	//Mithunan's lost items
	void SetRollOne(bool check);//setter
	bool GetRollOne();//getter

	void SetRollTwo(bool check);//setter
	bool GetRollTwo();//getter

	void SetRollThree(bool check);//setter
	bool GetRollThree();//getter

	//Winston's lost items
	void SetInstrumentOne(bool check);//setter
	bool GetInstrumentOne();//getter

	void SetInstrumentTwo(bool check);//setter
	bool GetInstrumentTwo();//getter

	void SetInstrumentThree(bool check);//setter
	bool GetInstrumentThree();//getter

	//Cafe Chaos
	void SetCafe(bool check);//setter
	bool GetCafe();//getter

	//Orchard
	void SetOrchard(bool check);//setter
	bool GetOrchard();//getter

protected:
	bool m_kainat = false;
	bool m_kyra = false;
	bool m_mithunan = false;
	bool m_stevie = false;
	bool m_winston = false;
	bool m_bookOne = false;
	bool m_bookTwo = false;
	bool m_bookThree = false;
	bool m_rollOne = false;
	bool m_rollTwo = false;
	bool m_rollThree = false;
	bool m_instrumentOne = false;
	bool m_instrumentTwo = false;
	bool m_instrumentThree = false;
	bool m_cafe = false;
	bool m_orchard = false;
	//int m_bookCount = 0;

};


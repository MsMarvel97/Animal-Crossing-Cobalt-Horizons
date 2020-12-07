#pragma once
class EndConditions
{
public:
	void SetStevieFlag(bool flag);
	bool GetStevieFlag();

	void SetKyraFlag(bool flag);
	bool GetKyraFlag();

	void SetKainatFlag(bool flag);
	bool GetKainatFlag();

	void SetMithunanFlag(bool flag);
	bool GetMithunanFlag();

	void SetWinstonFlag(bool flag);
	bool GetWinstonFlag();
protected:
	bool stevieFlag = false;
	bool kyraFlag = false;
	bool kainatFlag = false;
	bool mithunanFlag = false;
	bool winstonFlag = false;
};


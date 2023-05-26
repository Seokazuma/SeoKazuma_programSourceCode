#pragma once

class System
{
public:


	static System& GetInstance()
	{
		static System system;
		return system;
	}

	//m_enterKeyFlgの内容を取得する
	bool GetEnterKeyFlg()const
	{
		return m_enterKeyFlg;
	}
	bool GetLbuttonFlg()const
	{
		return m_lbuttonFlg;
	}
	bool GetRbuttonFlg()const
	{
		return m_rbuttonFlg;
	}
	bool GetEscapeFlg()const
	{
		return m_escapeFlg;
	}
	//m_enterKeyFlgにtrue/falseをセットする関数
	void SetEnterKeyFlg(bool _flg)
	{
		m_enterKeyFlg = _flg;
	}
	void SetLbuttonFlg(bool _flg)
	{
		m_lbuttonFlg = _flg;
	}
	void SetRbuttonFlg(bool _flg)
	{
		m_rbuttonFlg = _flg;
	}
	void SetEscapeFlg(bool _flg)
	{
		m_escapeFlg = _flg;
	}


private:
	System()
	{
	}

	bool m_enterKeyFlg = false;
	bool m_lbuttonFlg = false;
	bool m_rbuttonFlg = false;
	bool m_escapeFlg = false;
};
#define SYSTEM System::GetInstance()
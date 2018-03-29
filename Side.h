#pragma once

namespace Chess
{
	class Side
	{
	private:
		//enum Id
		//{
		//	White,
		//	Black,
		//	End
		//};
		int m_id;
	public:
		static const Side White;
		static const Side Black;

		Side() {};
		Side(int id) : m_id { id }
		{
		}

		friend bool operator== (const Side &a, const Side &b);
		friend bool operator!= (const Side &a, const Side &b);

		operator int()
		{
			return m_id;
		}

		operator int() const
		{
			return m_id;
		}

		Side& operator++();
	};
}
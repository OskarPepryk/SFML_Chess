#include "stdafx.h"
#include "Side.h"

namespace Chess
{
	bool operator==(const Side & a, const Side & b)
	{
		return a.m_id == b.m_id;
	}

	bool operator!=(const Side & a, const Side & b)
	{
		return a.m_id != b.m_id;
	}

	Side& Side::operator++()
	{
		if (m_id >= 1)
			m_id = 0;
		else
			++m_id;
		
		return *this;
	}

	const Side Side::White( 0 );
	const Side Side::Black( 1 );
}
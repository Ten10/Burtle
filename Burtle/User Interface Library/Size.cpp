#include "stdafx.h"
#include "Size.h"

CSize::CSize()
{
	cx = cy = 0;
}

CSize::CSize(LONG Width, LONG Height)
	: CSize()
{
	this->cx = Width;
	this->cy = Height;
}
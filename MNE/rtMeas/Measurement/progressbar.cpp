//=============================================================================================================
/**
* @file		progressbar.cpp
* @author	Christoph Dinh <christoph.dinh@live.de>;
* @version	1.0
* @date		October, 2010
*
* @section	LICENSE
*
* Copyright (C) 2010 Christoph Dinh. All rights reserved.
*
* No part of this program may be photocopied, reproduced,
* or translated to another program language without the
* prior written consent of the author.
*
*
* @brief	Contains the implementation of the ProgressBar class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "progressbar.h"


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace CSART;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

ProgressBar::ProgressBar()
: Measurement()
, m_iMin(-1)
, m_iMax(-1)
, m_iValue(0)

{

}


//*************************************************************************************************************

ProgressBar::~ProgressBar()
{

}


//*************************************************************************************************************

void ProgressBar::setValue(double v)
{
    if(v < m_iMin) v = m_iMin;
    else if(v > m_iMax) v = m_iMax;
    m_iValue = (int)v;
    notify();
}

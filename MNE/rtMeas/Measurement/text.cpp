//=============================================================================================================
/**
* @file		text.cpp
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
* @brief	Contains the declaration of the Text class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "text.h"


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace CSART;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

Text::Text()
: Measurement()
, m_Text("")

{

}


//*************************************************************************************************************

Text::~Text()
{

}


//*************************************************************************************************************

void Text::setText(const QString& text)
{
    m_Text = text;
    notify();
}

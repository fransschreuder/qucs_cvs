/***************************************************************************
                          dc_sim.cpp  -  description
                             -------------------
    begin                : Sat Aug 23 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : margraf@mwt.ee.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "dc_sim.h"


DC_Sim::DC_Sim()
{
  Description = QObject::tr("dc simulation");

  Lines.append(new Line(-56,-12, 56,-12,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-56, 12, 56, 12,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-56,-12,-56, 12,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 56,-12, 56, 12,QPen(QPen::darkBlue,2)));

  Lines.append(new Line(-51, 17, 60, 17,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-56, 12,-51, 17,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 56, 12, 60, 17,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 60, -7, 60, 17,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 56,-12, 60, -7,QPen(QPen::darkBlue,2)));

  x1 = -60; y1 = -16;
  x2 =  64; y2 =  21;

  tx = x1+4;
  ty = y2+4;
  Sign  = ".DC";
  Model = ".DC";
  Name  = "DC";
}

DC_Sim::~DC_Sim()
{
}

DC_Sim* DC_Sim::newOne()
{
  return new DC_Sim();
}
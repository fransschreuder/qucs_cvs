/***************************************************************************
                          tline.cpp  -  description
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

#include "tline.h"


TLine::TLine()
{
  Description = QObject::tr("ideal transmission line");

  Arcs.append(new Arc(-20, -9, 9, 19,     0, 16*360,QPen(QPen::darkBlue,2)));
  Arcs.append(new Arc( 11, -9, 9, 19,16*270, 16*180,QPen(QPen::darkBlue,2)));

  Lines.append(new Line(-30,  0,-16,  0,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 18,  0, 30,  0,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-16, -9, 16, -9,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-16,  9, 16,  9,QPen(QPen::darkBlue,2)));

  Ports.append(new Port(-30, 0));
  Ports.append(new Port( 30, 0));

  x1 = -30; y1 =-10;
  x2 =  30; y2 = 10;

  tx = x1+4;
  ty = y2+4;
  Sign  = "TLIN";
  Model = "TLIN";
  Name  = "Line";

  Props.append(new Property("Z", "50 Ohm", true, QObject::tr("characteristic impedance")));
  Props.append(new Property("L", "1 mm", true, QObject::tr("electrical length of the line")));
}

TLine::~TLine()
{
}

TLine* TLine::newOne()
{
  return new TLine();
}
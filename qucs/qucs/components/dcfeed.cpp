/***************************************************************************
                          dcfeed.cpp  -  description
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

#include "dcfeed.h"


dcFeed::dcFeed()
{
  Description = QObject::tr("dc feed");

  Arcs.append(new Arc(-17, -6, 13, 13,  0, 16*180,QPen(QPen::darkBlue,2)));
  Arcs.append(new Arc( -6, -6, 13, 13,  0, 16*180,QPen(QPen::darkBlue,2)));
  Arcs.append(new Arc(  5, -6, 13, 13,  0, 16*180,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-30,  0,-17,  0,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 17,  0, 30,  0,QPen(QPen::darkBlue,2)));

  Lines.append(new Line(-23,-13, 23,-13,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-23, 13, 23, 13,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-23,-13,-23, 13,QPen(QPen::darkBlue,1)));
  Lines.append(new Line( 23,-13, 23, 13,QPen(QPen::darkBlue,1)));

  Ports.append(new Port(-30,  0));
  Ports.append(new Port( 30,  0));

  x1 = -30; y1 = -15;
  x2 =  30; y2 =  16;

  tx = x1+4;
  ty = y2+4;
  Sign  = "DCFeed";
  Model = "DCFeed";
  Name  = "L";
}

dcFeed::~dcFeed()
{
}

dcFeed* dcFeed::newOne()
{
  return new dcFeed();
}
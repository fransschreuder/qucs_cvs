/***************************************************************************
                          biast.cpp  -  description
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

#include "biast.h"


BiasT::BiasT()
{
  Description = QObject::tr("bias t");

  Arcs.append(new Arc( -3,  2, 7, 7, 16*270, 16*180,QPen(QPen::darkBlue,1)));
  Arcs.append(new Arc( -3,  8, 7, 7, 16*270, 16*180,QPen(QPen::darkBlue,1)));
  Arcs.append(new Arc( -3, 14, 7, 7, 16*270, 16*180,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-22,-10, 22,-10,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-22,-10,-22, 22,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-22, 22, 22, 22,QPen(QPen::darkBlue,1)));
  Lines.append(new Line( 22,-10, 22, 22,QPen(QPen::darkBlue,1)));

  Lines.append(new Line(-13, -6,-13,  7,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( -9, -6, -9,  7,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( -9,  0, 22,  0,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-22,  0,-13,  0,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(-30,  0,-22,  0,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 22,  0, 30,  0,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(  0,  0,  0,  2,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(  0, 20,  0, 22,QPen(QPen::darkBlue,1)));
  Lines.append(new Line(  0, 22,  0, 30,QPen(QPen::darkBlue,2)));

  Ports.append(new Port(-30,  0));
  Ports.append(new Port( 30,  0));
  Ports.append(new Port(  0, 30));

  x1 = -30; y1 = -13;
  x2 =  30; y2 =  30;

  tx = x1+4;
  ty = y2+4;
  Sign  = "BiasT";
  Model = "BiasT";
  Name  = "X";
}

BiasT::~BiasT()
{
}

BiasT* BiasT::newOne()
{
  return new BiasT();
}
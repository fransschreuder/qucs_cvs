/***************************************************************************
                            bjt.h  -  description
                             -------------------
    begin                : Fri Jun 4 2004
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

#ifndef BJT_H
#define BJT_H

#include "component.h"


class BJT : public Component  {
public:
  BJT();
  ~BJT();
  Component* newOne();
  static Component* info(QString&, char* &, bool getNewOne=false);
  static Component* info_pnp(QString&, char* &, bool getNewOne=false);
  void recreate();
};

#endif
/*
 * strafo.cpp - symmetrical trafo class implementation
 *
 * Copyright (C) 2003 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  
 *
 * $Id: strafo.cpp,v 1.1 2003/12/20 19:03:29 ela Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "complex.h"
#include "object.h"
#include "node.h"
#include "circuit.h"
#include "strafo.h"

strafo::strafo () : circuit (6) {
}

void strafo::calc (nr_double_t frequency) {

  nr_double_t t1 = getPropertyDouble ("T1");
  nr_double_t t2 = getPropertyDouble ("T2");

  complex  d = t1 * t1 + t2 * t2 + t1 * t1 * t2 * t2;
  complex z1 = t2 * t2 / d;
  complex z2 = t1 * t1 / d;
  complex z3 = t1 * t1 * t2 * t2 / d;
  complex z4 = t1 * t2 * t2 / d;
  complex z5 = t1 * t1 * t2 / d;
  complex z6 = t1 * t2 / d;

  setS (1, 1,  z1);    setS (1, 2, z4);     setS (1, 3, -z4); 
  setS (1, 4, -z6);    setS (1, 5, z6);     setS (1, 6, 1 - z1);
  setS (2, 1,  z4);    setS (2, 2, z3);     setS (2, 3, 1 - z3); 
  setS (2, 4, -z5);    setS (2, 5, z5);     setS (2, 6, -z4);
  setS (3, 1, -z4);    setS (3, 2, 1 - z3); setS (3, 3, z3); 
  setS (3, 4,  z5);    setS (3, 5, -z5);    setS (3, 6, z4);
  setS (4, 1, -z6);    setS (4, 2, -z5);    setS (4, 3, z5); 
  setS (4, 4,  z2);    setS (4, 5, 1 - z2); setS (4, 6, z6);
  setS (5, 1,  z6);    setS (5, 2, z5);     setS (5, 3, -z5); 
  setS (5, 4, 1 - z2); setS (5, 5, z2);     setS (5, 6, -z6);
  setS (6, 1, 1 - z1); setS (6, 2, -z4);    setS (6, 3, z4); 
  setS (6, 4, z6);     setS (6, 5, -z6);    setS (6, 6, z1);
}
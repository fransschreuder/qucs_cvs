/*
 * mstee.h - microstrip t-junction class definitions
 *
 * Copyright (C) 2004 Stefan Jahn <stefan@lkcc.org>
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
 * $Id: mstee.h,v 1.1 2004/09/26 13:31:37 ela Exp $
 *
 */

#ifndef __MSTEE_H__
#define __MSTEE_H__

class mstee : public circuit
{
 public:
  mstee ();
  void initDC (void);
  void initAC (void);
  void initSP (void);
};

#endif /* __MSTEE_H__ */
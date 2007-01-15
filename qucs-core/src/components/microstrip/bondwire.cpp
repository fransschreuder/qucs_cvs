/*
 * bondwire.cpp - bondwire class implementation
 *
 * Copyright (C) 2006 Bastien Roucaries <roucaries.bastien@gmail.com>
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
 * the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.  
 *
 * $Id: bondwire.cpp,v 1.1 2006/12/20 11:43:06 raimi Exp $
 *
 */

/*!\file bondwire.cpp
   \brief Implement a bondwire model
   
   Bibliography:

   [1] Microwave Solid State Circuit Design, 
       Inder Bahl and Prakash Barthia  -- 2d edition
       2003 - Wiley interscience
       ISBN 9-471-20755-1

   [2] Wikibook Electronics/Inductors
       http://en.wikibooks.org/w/index.php?title=Electronics/Inductors&oldid=497883
       online 2006/11/02

   [3] Physical and electrical modeling of bonding wires up to 110 GHz,
       Descharles, C.   Algani, C.   Mercier, B.   Alquie, G. 
       Microwave Conference, 2003. 33rd European 
       Volume: 2,  On page(s): 639- 642 vol.2
       ISBN: 1-58053-834-7
       INSPEC Accession Number: 1262971
*/
  

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "logging.h"
#include "complex.h"
#include "object.h"
#include "node.h"
#include "circuit.h"
#include "component_id.h"
#include "substrate.h"
#include "constants.h"
#include "matrix.h"
#include "bondwire.h"

bondwire::bondwire () : circuit (2) {
  type = CIR_BONDWIRE;
}


/*! Create a matching table between model number and string */
#define TABLE(x) { #x, x }
/*! Array size */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/*! bondwire model number */
enum bondwiremodel {
  FREESPACE,       /*!< fresspace model */
  MIRROR,          /*!< mirror plane model */
  DESCHARLES       /*!< Descharles Algani Mercier Alquie model */
};

/*! model number to string matching table definition */
struct modeltable_t { 
  char * name; 
  int model;
};

/*! model number to string matching table implementation */
static const modeltable_t modeltable[] = {
  TABLE(FREESPACE),
  TABLE(MIRROR),
};


/*!\brief Get properties from model 
 * Get properties and fill the class
 *  
 *  \todo check values
 */
void bondwire::getProperties (void) {
  unsigned int i;

  R = 0;
  l = getPropertyDouble ("L");
  d = getPropertyDouble ("D");
  h = getPropertyDouble ("H");
  rho = getPropertyDouble ("rho");
  mur =getPropertyDouble ("mur");
  
  /* model used */
  char * Model  = getPropertyString ("Model");
  if (Model == NULL) {
    model = FREESPACE;
    logprint (LOG_STATUS, "Model is not specified force FREESPACE");
  } else {
    model = (enum bondwiremodel) -1;
    for (i = 0 ; i < ARRAY_SIZE (modeltable); i++) {
      if (!strcasecmp (modeltable[i].name, Model))
	model = modeltable[i].model;
    }

    if (model == -1) 
      /* XXXX: how to abort ? */
      logprint (LOG_ERROR, "Model %s not defined", Model);
  }

  /* For noise */
  temp = getPropertyDouble ("Temp");

  /* how to get properties of the substrate, e.g. Er, H */
  substrate * subst = getSubstrate ();
  nr_double_t er    = subst->getPropertyDouble ("er");
  nr_double_t h     = subst->getPropertyDouble ("h");
  nr_double_t t     = subst->getPropertyDouble ("t");
  
  /* Not yet used */
  (void) er;
  (void) h;
  (void) t;
}

/*! Compute skin depth
 * \todo Factorize the compution of skin depth in a header file
 * 
 * \param f frequency
 * \param rho bond wire resistivity 
 * \param mur relative magnetic permeabillity
 */
static nr_double_t skindepth (const nr_double_t f,
			      const nr_double_t rho, const nr_double_t mur) {
  return sqrt (rho / (M_PI * f * MU0 * mur));
}

/*! Compute resitance of the wire
 * Resitance of the wire is computed using classical 
 * tube model enhanced for case where tube is greater
 * than conductor (eg low frequency case)
 *
 * \todo Offer other resistance model for 
 *        instance exponential decay and bessel function exact
 *         computation. But it do not know it is worth the effort
 *
 *  \todo factorise the resistance model
 */
nr_double_t bondwire::resistance (const nr_double_t f) const {
  nr_double_t delta, rout, rin;
  rout = d / 2;
  if (f > 0.0) {
    delta = skindepth (f, rho, mur);
    rin = rout - delta;
    if (rin < 0.0)
      rin = 0.0;
  }
  else
    rin = 0.0;

  return (rho * M_1_PI * l) / (rout * rout - rin * rin);
}


/*! Compute correction factor
 *  According to [1] pp63 (2.30a-b) correction factor 
 *  is such as:
 *  \f[
 *   C = 0.25 \tanh \frac{4\delta}{d}
 *  \f]
 *  Where \f$\delta\f$ is the well know skin depth
 * 
 *  \param f frequency
 *  \param d bond wire diameter
 *  \param rho bond wire resistivity 
 *  \param mur relative magnetic permeabillity
 *
 * However according to [2] it seems that the author of [1] do the assumption of
 * \f$\mu_r=1\f$ therefore rewrite the equation such as: 
 *
 *  \f[
 *   C = \frac{\mu_r}{4} \tanh \frac{4\delta}{d}
 *  \f]
 *
 *  \return mur/4 if rho == 0 else C
 *  \todo Check domain validity for round C factor
 */
static nr_double_t correctionfactor (const nr_double_t f,
				     const nr_double_t d, 
				     const nr_double_t rho,
				     const nr_double_t mur) {
  /* Skin depth */
  nr_double_t delta;

  if (f > 0.0 && rho > 0.0) {
    delta = skindepth (f, rho, mur);
    if (delta <= d /2)
      return (mur / 4) * tanh ((4 * delta) / d);
  }
  return mur / 4;
}

/*! Compute free space inductance
    According to [1] pp63 (2.29) free space inductance (in nH)
   is such as (\f$l\f$ in micrometers):

   \f[
    L = 2\times10^{-4} l \left[ 
               \ln\left\{ \frac{2l}{d} 
	                  + \sqrt{1 + \left(\frac{2l}{d}\right)^2} \right\}
	       + \frac{d}{2l} - \sqrt{1+\left(\frac{d}{2l}\right)^2}
	       + C \right]
			  
   \f]

   According to [2] self inductance is (in H with l im m):
   \f[
    L = \frac{\mu_0}{2\pi} l\left[ 
               \ln\left\{ \frac{2l}{d} 
	                  + \sqrt{1 + \left(\frac{2l}{d}\right)^2} \right\}
	       + \frac{d}{2l} - \sqrt{1+\left(\frac{d}{2l}\right)^2}
	       + \frac{\mu_r}{4} \right]
			  
   \f]

   Finally we will use (in H with l im m)::
   \f[
    L = \frac{\mu_0}{2\pi} l\left[ 
               \ln\left\{ \frac{2l}{d} 
	                  + \sqrt{1 + \left(\frac{2l}{d}\right)^2} \right\}
	       + \frac{d}{2l} - \sqrt{1+\left(\frac{d}{2l}\right)^2}
	       + C \right]
			  
   \f]

   \param f frequency
   \param d bond wire diameter
   \param l bond wire length (in meter)
   \param rho  bond wire resistivity
   \param mur relative magnetic permeabillity
*/
nr_double_t bondwire::Lfreespace (const nr_double_t f) const {
  nr_double_t _2ld = (2.0 * l) / d;
  nr_double_t d2l = d / (2.0 * l);
  nr_double_t tmp;

  tmp = log (_2ld + sqrt (1 + _2ld * _2ld));
  tmp += d2l - sqrt (1 + d2l * d2l);
  tmp += correctionfactor (f, d, rho, mur);

  return MU0 * (M_1_PI / 2) * l * tmp;
}


/*! Compute inductance modeling ground plane effect
   According to [1] pp64 (2.32) inductance (in nH)
   is such as (\f$l\f$ in micrometers):

   \f[
    L = 2\times10^{-4} l \left[ 
               \ln \frac{4h}{d} 
	       + \ln \frac{l+\sqrt{l^2+d^2/4}}{l+\sqrt{l^2+4h^2}}
	       + \sqrt{1+\frac{4h^2}{l^2}} - \sqrt{1-\frac{d^2}{4l^2}}
	       - 2 \frac{h}{l} + \frac{d}{2l}
	       \right]
			  
   \f]

   Finally we will use (in H with l im m):
   \f[
    L = \frac{\mu_0}{2\pi} l \left[ 
               \ln \frac{4h}{d} 
	       + \ln \frac{l+\sqrt{l^2+d^2/4}}{l+\sqrt{l^2+4h^2}}
	       + \sqrt{1+\frac{4h^2}{l^2}} - \sqrt{1-\frac{d^2}{4l^2}}
	       - 2 \frac{h}{l} + \frac{d}{2l}
	       \right]			  
   \f]

   \note Mirror is a strange model that is frequency independant. Whereas computation 
        are valid, hypothesis are arguable. Indeed, they did the assumption that mass 
	is perfect that is really a zero order model in high frequency.
*/
nr_double_t bondwire::Lmirror (void) const {
  nr_double_t tmp;

  /* compute \$\ln \frac{l+\sqrt{l^2+d^2/4}}{l+\sqrt{l^2+4h^2}}\$ */
  tmp  = log ((l + sqrt (l * l + d * d / 4)) / (l + sqrt (l * l + 4 * h * h)));
  tmp += log (4 * h / d);
  tmp += sqrt (1 + 4 * h * h / (l * l));
  tmp -= sqrt (1 + d * d / (4 * l * l));
  tmp -= 2 * h / l;
  tmp += d / (2 * l);

  return  MU0 * (M_1_PI / 2) * l * tmp;
}
  
  
/*! Compute Y matrix of bond wire
 */
matrix bondwire::calcMatrixY (const nr_double_t f) {
  nr_double_t L = 0, denom , Lw;

  switch (model) {
  case MIRROR:
    L = Lmirror ();
    R = resistance (f);
    break;
  case FREESPACE:
    L = Lfreespace (f);
    R = resistance (f);
    break;
  default:
    break;
  }

  Lw = L * 2 * M_PI;
  denom = R * R + Lw * Lw;

  /* build Y-parameter matrix */
  complex yL = rect (R / denom, -Lw / denom);

  matrix Y (2);
  Y.set (NODE_1, NODE_1, +yL);
  Y.set (NODE_1, NODE_2, -yL);
  Y.set (NODE_2, NODE_1, -yL);
  Y.set (NODE_2, NODE_2, +yL);
  return Y;
}

/*! Initialise S parameter simulation */
void bondwire::initSP (void) {
   getProperties ();
}

/*! Compute S parameters 
 *! Reuse computed Y matrix 
 */
void bondwire::calcSP (const nr_double_t frequency) {
  setMatrixS (ytos (calcMatrixY (frequency)));
}

/*! DC model initialisation 
 *! DC model of a bondwire is a resistance 
 */
void bondwire::initDC (void) {
  nr_double_t g;

  getProperties ();

  /* for non-zero resistances usual MNA entries */
  if (rho != 0.0) {
    g = 1.0 / resistance (0);
    setVoltageSources (0);
    allocMatrixMNA ();
    setY (NODE_1, NODE_1, +g); setY (NODE_2, NODE_2, +g);
    setY (NODE_1, NODE_2, -g); setY (NODE_2, NODE_1, -g);
  }
  /* for zero resistances create a zero voltage source */
  else {
    setVoltageSources (1);
    setInternalVoltageSource (1);
    allocMatrixMNA ();
    clearY ();
    voltageSource (VSRC_1, NODE_1, NODE_2);
  }
}

/*! Initialise AC simulation */
void bondwire::initAC (void) {
  getProperties (); 
  setVoltageSources (0);
  allocMatrixMNA ();
}

/*! Compute AC model 
 *! Use serial LR model (Y matrix)
 */
void bondwire::calcAC (const nr_double_t frequency) {
  setMatrixY (calcMatrixY (frequency));
}

void bondwire::calcNoiseSP (nr_double_t) {
  // calculate noise correlation matrix
  nr_double_t T = getPropertyDouble ("Temp");
  nr_double_t f = kelvin (T) * 4.0 * R * z0 / norm (4.0 * z0 + R) / T0;
  setN (NODE_1, NODE_1, +f); setN (NODE_2, NODE_2, +f);
  setN (NODE_1, NODE_2, -f); setN (NODE_2, NODE_1, -f);
}

void bondwire::calcNoiseAC (nr_double_t) {
  // calculate noise current correlation matrix
  nr_double_t y = 1 / R;
  nr_double_t T = getPropertyDouble ("Temp");
  nr_double_t f = kelvin (T) / T0 * 4.0 * y;
  setN (NODE_1, NODE_1, +f); setN (NODE_2, NODE_2, +f);
  setN (NODE_1, NODE_2, -f); setN (NODE_2, NODE_1, -f);
}
/*
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 *  Copyright (C) 2000 - 2005 Liam Girdwood  
 */

#ifndef LN_SIDEREAL_TIME_H
#define LN_SIDEREAL_TIME_H

<<<<<<< HEAD
#include <libnova/ln_types.h>
=======
#include "ln_types.h"
>>>>>>> 6d8b8ad7a55c616262f643554a2d8d4584365c22

#ifdef __cplusplus
extern "C" {
#endif


/*! \defgroup sidereal Sidereal Time 
*
* TODO
*/

/*! \fn ln_double ln_get_mean_sidereal_time (double JD)
* \brief Calculate mean sidereal time from date. 
* \ingroup sidereal
*/ 
double LIBNOVA_EXPORT ln_get_mean_sidereal_time (double JD);

/*! \fn ln_get_apparent_sidereal_time (double JD)
* \brief Calculate apparent sidereal time from date.
* \ingroup sidereal
*/
 
double LIBNOVA_EXPORT ln_get_apparent_sidereal_time (double JD);

#ifdef __cplusplus
};
#endif

#endif

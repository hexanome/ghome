/* ------------------------------
   $Id: internals.h 36 2007-03-20 21:44:48Z phm $
   ------------------------------------------------------------

   internals.h

   Declaration des "Constantes" matérielles extraites du fichier de
   configuration et exploitees par hardware.c
   Ces variables sont à l'usage exclusif de "config.l" ! 

*/

#ifndef _INTERNALS_H_
#define _INTERNALS_H_

int setIValue(char *name, int v);
int setSValue(char *name, char *v);

#endif

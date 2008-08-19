/** Fichier util.c
 **
 ** Diverses routines utilitaires pour calife.
 **
 ** Copyright (c) 1991-1995 by Ollivier ROBERT
 ** A distribuer selon les regles de la GNU GPL General Public Licence
 ** Voir le fichier COPYING fourni.
 **/

#ifndef lint
static const char * rcsid = "@(#) $Id: util.c,v 88c4f70961ac 2008/08/19 15:34:11 roberto $";
#endif

#include "config.h" /* configure */
#include "conf.h"

/** Renvoie la derniere composante (le nom du fichier) d'un nom complet
 **
 ** Parametre :     filename    char *  nom a analyser
 **
 ** Retourne :      p           char *  le nom de fichier
 **/

#ifndef HAVE_BASENAME       /* linux as already one it seems */
char * 
basename (char * filename)
{
    char * p;
    p = filename + (strlen (filename) * sizeof (char));
    while (*p != '/')
    {
        if (p == filename)
            return (filename);
        p--;
    }
    return (char *) (p + 1);
}
#endif /* HAVE_BASENAME */

/** Alloue de la m�moire par malloc, la met � z�ro et renvoie le pointeur.
 ** Sort si plus de m�moire.
 **
 ** Parametres:     num         size_t  nombre de blocs � allouer
 **                 size        size_t  taille d'un bloc
 **
 ** Retourne :      ptr         void *  un pointeur sur la zone
 **/

void *
xalloc (size_t size)
{
    void *  ptr;
    
    ptr = (void *) malloc (size);
    if (ptr == NULL)
        die (1, "No more memory.");
    memset ((char *) ptr, '\0', size);
    return ptr;
}

/** Sort du programme avec le message et le code de retour sp�cifi�s.
 **
 ** Contribu� par Nat Makar�vitch <nat@nataa.frmug.fr.net>
 ** Modifi�e pour se conformer � mes usages :-)
 **
 ** Param�tres:     err         code d'erreur de exit(3)
 **                 fmt         format du message
 **                 ...         les arguments
 **
 ** Retourne:       rien
 */

#ifdef HAVE_STDARG_H
#include <stdarg.h>

void
die (int err, const char * fmt, ...)
{
    va_list ap;

    fflush (stdout);

    closelog ();
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    va_end (ap);
    fprintf (stderr, "\n");
    exit (err);
}

#elif defined(HAVE_VARARGS_H)

void
die (int err, const char * fmt, ...)
{
    va_list ap;

    fflush (stdout);

    closelog ();
    va_start (ap);
    vfprintf (stderr, fmt, ap);
    va_end (ap);
    fprintf (stderr, "\n");
    exit (err);
}

#else
#error You should have at least one of stdarg/varargs
#endif /* neither */


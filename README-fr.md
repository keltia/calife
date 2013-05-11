# Introduction

Calife est un programme permettant à certains utilisateurs, dont les noms sont présents dans une liste, de passer sous un quelconque ID utilisateur (par exemple, mais pas uniquement, `root`) en rentrant leur propre mot de passe.

Ce système permet de ne pas divulguer le mot de passe de `root` (par exemple pour le mode *single-user* de certains Unix) tout en conférant les droits de `root` à certains utilisateurs bien identifiés.

L'introduction obligatoire du mot de passe lors du changement d'ID permet de ne pas compromettre l'accès `root` si l'utilisateur reste logé sur une machine puis s'en éloigne.

La barrière de sécurité finale réside dans le mot de passe de ces utilisateurs, qui, nous l'espérons, connaissent les règles élémentaires.

Des listes d'utilisateurs vers lesquels quelqu'un a le droit d'utiliser `calife` permettent d'affiner la gestion des personnes autorisées.

# Distribution

Calife est distribué sous la GNU General Public Licence qui vous permet de diffuser, utiliser et modifier ce programme tant que vous en diffusez les sources et me communiquez toutes les modifications effectuées. Cela me permet de l'améliorer et de le maintenir dans de bonnes conditions.

Vous trouverez la licence GNU dans le fichier `COPYING` fourni avec l'archive.

Pour ce qui est du *disclaimer* standard associé à ce type de programme et à ce type de diffusion, le paragraphe suivant, tiré de la licence BSD, s'applique :

    THIS SOFTWARE IS PROVIDED BY ME ``AS IS'' AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
    EVENT SHALL I BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
    OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
    OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Ceci dit, si vous l'utilisez et le trouvez utile, je ne refuserais pas une Guinness :-)

Le site primaire pour Calife est [ici](http://www.keltia.net/programs/calife/) et en [FTP](ftp://mutt.frmug.org/pub/calife/). Un miroir existe [ici](ftp://postfix.EU.org/pub/calife/).

Calife utilise maintenant [Bitbucket](http://bitbucket.org/) comme frontal de gestion des tickets, flux RSS, release, forums, etc. Le site est accessible [ici](https://bitbucket.org/keltia/calife/).

# Installation

## Autoconf

Calife utilise dorénavant le programme GNU `autoconf` pour la configuration. GNU cc n'est pas obligatoire pour compiler Calife mais ne sont testés régulièrement que les compilateurs suivants :

- GNU cc 3.x et 4.3.2
- Clang
- SUN cc sur plateforme Sparc

Tapez la commande :

    ./configure

Le script va analyser votre système et créer les fichiers suivants :

- `Makefile`
- `config.h`
- `calife.1`
- `calife.auth.5`

Le binaire et les pages de  manuels seront par défaut installés, lors du `make install`, sous `/usr/local`.

Utiliser l'argument de configure `--prefix=/le/prefixe` pour indiquer le nom du répertoire adéquat.

Quelques options spéciales sont disponibles  :

    --with-etcdir=PATH        Directory containing calife.auth (default /etc)
    --disable-shadow=yes|no   Disable getspnam(3) usage for some Linuxes
    --enable-debug=yes|no     Set the debugging mode (default is no)
    --enable-shellhack=yes|no (default is no)
    --enable-dynamic          Build the program dynamically (default static)
    --enable-global-rc        Add /etc/calife.out support.

`--with-etcdir=CHEMIN`

Permet de  placer les fichiers  calife.auth et  calife.out ailleurs que dans /etc. 

`--enable-debug`

Exige l'affichage des messages de "debugging". Inutile en usage normal. 

`--enable-shellhack`

Si vous avez le message "Permission denied" après avoir tapé votre mot de passe, essayez de relancer  `configure` avec cette option. 

`--enable-dynamic`

Si vous désirez avoir le binaire lié dynamiquement et non pas statiquement. L'utilisation du binaire statique permet non seulement d'améliorer le partage des pages entre de multiples instances de calife mais aussi de ne pas être dépendant de la version de la bibliothèque standard.

Parmi les systèmes devant utiliser `--enable-dynamic`, on compte Solaris 2.x, HP-UX, Linux et MacOS X/Darwin.

## Cas de Linux :

Ce système utilise plusieurs systèmes de *shadow passwords*. Il est possible que `configure` ne parvienne pas à déterminer le type d'outil de *shadow passwords* employé. Je n'ai pas de Linux, il m'est donc difficile de tester... Toute suggestion et modification de `configure.in` appréciées.

L'option suivante permet à calife de ne pas utiliser `getspnam(3)` dans le cas où la fonction existe mais les shadows passwords ne sont pas utilisés.

`--disable-shadow`

Le défaut est d'utiliser getspnam(3) si elle est définie.

Si vous devez créer une nouvelle section dans `conf.h` ou une nouvelle entrée dans `Makefile.in` ou `configure.in` , merci de me communiquer les modifications pour incorporation dans les versions suivantes.

Le programme s'installe sous forme d'un binaire nommé `calife` pour des raisons de facilité mais le nom historique est `...`.

Il installe aussi un fichier de configuration d'exemple, appelé `calife.auth-dist` et par défaut placé dans `/etc`.

Consulter les pages de manuel fournies (`calife` et `calife.auth`) pour en apprendre davantage.

Le programme essaie de lancer le fichier `calife.out` (par défaut dans `/usr/local/etc` sur FreeBSD et `/etc` ailleurs) à la sortie de `calife`. Ceci permet par exemple d'envoyer un mail à chaque utilisation de calife. Cette possibilité est optionnelle et dépend de l'option `--enable-global-rc`.

## Compatibilité

Calife a été testé et développé sur les systèmes UNIX suivants (les versions testées peuvent être anciennes et devraient être retestées mais je n'ai plus accès à pas mal de ces machines...) :

- FreeBSD 1 - 9.x   (mach. de développement de l'auteur)
- MacOS X 10.2 - 10.8.
- Linux 0.99+ et 1.* (1.2.* et 1.3.*), 2.*
- Solaris 2.x (et la plupart des SVR4)
- HP-UX 8.*, 9.*, 10.* et 11.*

Bien que Calife n'ait pas été testé par l'auteur sur NetBSD et OpenBSD, il est fort probable du fait de la base commune de code, il n'y ait que peu de souci à utiliser Calife sur ces systèmes, à part sans doute quelques différences dans le sous-système PAM.

NOTE: la version 3.0 sera une version orienté PAM de manière à simplifier le code, sa maintenance et la compatibilité avec les UNIX le supportant, le code non PAM restant a priori utilisable.

Systèmes pour lesquels la compatibilité n'est pas/plus testée directement par l'auteur, ceux-ci étant donc non supportés.

- SunOS 4.1.*
- SunOS 3.2
- NeXTStep 3
- SCO 3.2
- HP-UX 8.* et 9.*
- SVR4 NeWSOS 5.02
- Encore Gould NP1
- AIX 3.x & 4.x
- IRIX 5.x (probablement 6.x aussi)
- GNU Hurd (nouveau)

Pour calife 3.0/PAM, voir les exemple de fichier de configuration de PAM dans le répertoire `pam/` de l'archive. Placer ce fichier dans `/etc/pam.d` (ou l'endroit demandé par votre implémentation de PAM) sous le nom `calife`.

Il existe à l'heure actuelle deux fichiers :

- `macosx`    pour les systèmes MacOS 10.3/Panther à 10.5/Leopard ;
- `macosx10`  pour Mac OS X Snow Leopard 10.6 ;
- `freebsd5`  pour les systèmes FreeBSD 5.x et supérieurs
- `solaris`   pour Solaris 8, 9 et 10 (à insérer dans pam.conf)
- `linux`

Pour utiliser LDAP/NIS et autres systèmes d'authentification, il faudra modifier le fichier PAM correspondant.

La fonctionnalité syslog(3) est maintenant obligatoire pour compiler calife. Ca simplifie le code manière significative.

Des packages FreeBSD et Debian existent dorénavant grâce à Thomas Quinot. Alain Thivillon a généré des packages Red-hat (RPM) pour 2.8.4b.

## Pages de manuel

Les pages de manuel sont très succinctes et utilisent le système de macros mis au point à Berkeley appelé `mandoc` donc si vous n'avez pas ces macros, le résultat n'est pas très lisible. Je conseille vivement d'utiliser ce système qui est plus complet que le `man` habituel.

Si vous disposez de GNU groff, alors la commande pour les pages de manuel est `nroff -mandoc` ou `groff -mandoc`.

# Historique

Ce programme est né en 1991 d'une idée commune à François Berjon et moi, qui utilisions un système très simple, appelé ``...'', capable de lancer un shell root après vérification de l'uid du requérant, sans demander de mot de passe.

Il a évolué lors de mon travail à Antenne 2 et surtout lors de mon stage à l'Université de Marne-la-Vallée, où il a gagné la majorité de ses possibilités actuelles.

Il est utilisé quotidiennement par plusieurs sites et donne des résultats satisfaisants.

Tout commentaire, critique [constructive] et patches bienvenus à l'une des adresses suivantes :

[adresse privée maison](mailto:roberto@keltia.net)<br/>
[Main site](http://bitbucket.org/keltia/calife/)

# Remerciements

René Cougnenc<br/>
François Berjon         <fcb@gwynedd.frmug.org><br/>
Laurent Chemla          <laurent@brainstorm.fr><br/>
Pierre Beyssac          <pb@fasterix.frmug.org><br/>
Nathanaël Makarévitch   <nat@nataa.frmug.org><br/>
Thomas Quinot           <thomas@cuivre.fr.eu.org><br/>
Christian Perrier       <bubulle@kheops.frmug.org><br/>
Philippe Regnault       <regnauld@starbsd.org><br/>
Thierry Besancon        <Thierry.Besancon@csi.uvsq.fr><br/>
Laurent Wacrenier       <lwa@teaser.fr><br/>
Erwan David             <erwan@rail.eu.org><br/>
Bertrand Petit          <elrond@phoe.frmug.org><br/>
Denis Ducamp            <Denis.Ducamp@hsc.fr><br/>

et à tous ceux qui ont participé pour la mise au point et les tests.

# $Id$

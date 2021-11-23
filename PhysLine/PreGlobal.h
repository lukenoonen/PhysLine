#ifndef PREGLOBAL_H
#define PREGLOBAL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "UTIL.h"
#include "Math.h"
#include "DataStructures.h"

class CGlobalValues;
class CRenderManager;
class CEntityManager;
class CInputManager;
class CPlayerManager;
class CHUDManager;
class CFileManager;
class CAudioManager;

#define DECLARE_CLASS( x, y )			typedef x ThisClass; \
										typedef y BaseClass;

#define DECLARE_CLASS_NOBASE( x )		typedef x ThisClass;

#endif // PREGLOBAL_H
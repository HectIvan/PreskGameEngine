#pragma once

#if PK_PLATFORM == PK_PLATFORM_WIN32
# if PK_COMPILER == PK_COMPILER_MSVC
#   if defined( PK_STATIC_LIB )
#     define PK_PHYSICS_EXPORT
#   else
#     if defined ( PK_PHYSICS_EXPORTS )
#       define PK_PHYSICS_EXPORT __declspec( dllexport )
#     else
#       define PK_PHYSICS_EXPORT __declspec( dllimport )
#     endif
#   endif
# else // any other compiler
#   if defined( PK_STATIC_LIB )
#     define PK_PHYSICS_EXPORT
#   else
#     if defined ( PK_PHYSICS_EXPORTS)
#       define PK_PHYSICS_EXPORT __attribute__ ((dllexport))
#     else
#       define PK_PHYSICS_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define PK_PHYSICS_HIDDEN
#else
# define PK_PHYSICS_EXPORT __attribute__ ((visibility ("default")))
# define PK_PHYSICS_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

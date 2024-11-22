project(pkUtilities)
add_compile_definitions(PK_UTILITY_EXPORTS)

# Configurar la biblioteca compartida (DLL)
add_library(${PROJECT_NAME} SHARED
    source/pkCapsule.cpp
    source/pkCube.cpp
    source/pkMatrix4.cpp
    source/pkPlane.cpp
    source/pkPlatformMath.cpp
    source/pkQuaternion.cpp
    source/pkSphere.cpp
    source/pkVector2.cpp
    source/pkVector3.cpp
    source/pkVector4.cpp
)


# Incluir los archivos en la carpeta include
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Configurar el sufijo 'd' para Debug
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "d"
)

# Establecer las rutas de salida
set_output_directories(${PROJECT_NAME})

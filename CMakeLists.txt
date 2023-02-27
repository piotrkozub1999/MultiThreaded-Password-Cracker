 # Nazwa pliku po kompilacji
 project (hash_cracker)

 # Linki do wszystkich plików cpp
 set(SOURCES
         src/Cracker.cpp
         src/HashAlgorithm.cpp
         src/MD5Algorithm.cpp
         src/pch.cpp
         src/SHA1Algorithm.cpp
         src/SHA256Algorithm.cpp
         src/main.cpp
         )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

 # Dodanie pliku wykonawczego z powyższymi źródłami
 add_executable(hash_cracker ${SOURCES})

 # Ścieżka plików nagłówkowych
 target_include_directories(hash_cracker
         PRIVATE
         ${PROJECT_SOURCE_DIR}/inc
         )

 find_package(OpenSSL REQUIRED)
 if (OPENSSL_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})

  target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)

  message(STATUS "Znaleziono OpenSSL ${OPENSSL_VERSION}")

 else()

  message(STATUS "OpenSSL nieznaleziony")

 endif()

# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(ExternalProject)

add_subdirectory(libprocess)

# Downloads, configures, and compiles the third-party libraries for the mesos.

# Define sources of third-party dependencies.
#############################################
set(UPSTREAM_URL https://github.com/3rdparty/mesos-3rdparty/raw/master)
set(REBUNDLED_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (REBUNDLED)
  #set(LEVELDB_URL   ${REBUNDLED_DIR}/leveldb${LEVELDB_VERSION}.tar.gz)
  set(LEVELDB_URL   ${REBUNDLED_DIR}/leveldb-${LEVELDB_VERSION}.tar.gz)
  set(ZOOKEEPER_URL ${REBUNDLED_DIR}/zookeeper-${ZOOKEEPER_VERSION}.tar.gz)
else (REBUNDLED)
  #set(LEVELDB_URL   ${UPSTREAM_URL}/leveldb${LEVELDB_VERSION}.tar.gz)
  set(LEVELDB_URL   ${UPSTREAM_URL}/leveldb-${LEVELDB_VERSION}.tar.gz)
  set(ZOOKEEPER_URL ${UPSTREAM_URL}/zookeeper-${ZOOKEEPER_VERSION}.tar.gz)
endif (REBUNDLED)

# Define build/patch/configure commands for third-party libs.
#############################################################
if (NOT WIN32)
  set(LEVELDB_CONFIG_CMD    ${CMAKE_NOOP})
  set(LEVELDB_BUILD_CMD     cd ${LEVELDB_C_ROOT} && make)
  set(LEVELDB_INSTALL_CMD   ${CMAKE_NOOP})

  set(ZOOKEEPER_CONFIG_CMD  cd ${ZOOKEEPER_C_ROOT} && ./configure --enable-shared=no --with-pic --srcdir=. --prefix=${ZOOKEEPER_LIB})
  set(ZOOKEEPER_BUILD_CMD   cd ${ZOOKEEPER_C_ROOT} && make)
  set(ZOOKEEPER_INSTALL_CMD cd ${ZOOKEEPER_C_ROOT} && make install)

  PATCH_CMD(
    ${MESOS_3RDPARTY_SRC}/leveldb.patch
    LEVELDB_PATCH_CMD)

  PATCH_CMD(
    ${MESOS_3RDPARTY_SRC}/zookeeper-${ZOOKEEPER_VERSION}.patch
    ZOOKEEPER_PATCH_CMD)
elseif (WIN32)
  set(LEVELDB_PATCH_CMD     ${CMAKE_NOOP})
  set(LEVELDB_CONFIG_CMD    ${CMAKE_NOOP})
  set(LEVELDB_BUILD_CMD     ${CMAKE_NOOP})
  set(LEVELDB_INSTALL_CMD   ${CMAKE_NOOP})

  # Set the patch command which will utilize patch.exe in temp location for no elevation prompt
  set(
    ZOOKEEPER_PATCH_CMD
    ${PATCHEXE_LOCATION} --binary -p1 < ${MESOS_3RDPARTY_SRC}/zookeeper-${ZOOKEEPER_VERSION}.patch)

  VS_BUILD_CMD(
    ZOOKEEPER
    ${ZOOKEEPER_C_ROOT}/zookeeper-vs2015.sln
    ${CMAKE_BUILD_TYPE}
    "zookeeper")

  set(ZOOKEEPER_CONFIG_CMD  ${CMAKE_NOOP})
  set(ZOOKEEPER_INSTALL_CMD ${CMAKE_NOOP})
endif (NOT WIN32)

# Third-party libraries. Tell the build system how to pull in and build third-
# party libraries at compile time, using the ExternalProject_Add macro.
##############################################################################
ExternalProject_Add(
  ${LEVELDB_TARGET}
  PREFIX            ${LEVELDB_CMAKE_ROOT}
  PATCH_COMMAND     ${LEVELDB_PATCH_CMD}
  CONFIGURE_COMMAND ${LEVELDB_CONFIG_CMD}
  BUILD_COMMAND     ${LEVELDB_BUILD_CMD}
  INSTALL_COMMAND   ${LEVELDB_INSTALL_CMD}
  URL               ${LEVELDB_URL}
  )

ExternalProject_Add(
  ${ZOOKEEPER_TARGET}
  PREFIX            ${ZOOKEEPER_CMAKE_ROOT}
  PATCH_COMMAND     ${ZOOKEEPER_PATCH_CMD}
  CONFIGURE_COMMAND ${ZOOKEEPER_CONFIG_CMD}
  BUILD_COMMAND     ${ZOOKEEPER_BUILD_CMD}
  INSTALL_COMMAND   ${ZOOKEEPER_INSTALL_CMD}
  URL               ${ZOOKEEPER_URL}
  )

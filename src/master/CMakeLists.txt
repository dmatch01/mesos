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

# SOURCE FILES FOR THE MESOS MASTER EXECUTABLE.
##############################################
if (NOT WIN32)

  # THE PROCESS SLAVE EXECUTABLE (generates, e.g., mesos-master).
  ##############################################################
  add_executable(${MASTER_TARGET} main.cpp)
endif (NOT WIN32)

# ADD BINARY DEPENDENCIES (Make a top-level target depend on other
# top-level targets to ensure that they build before <target> does).
####################################################################
add_dependencies(${MASTER_TARGET} ${MESOS_TARGET} ${PROTOBUF_TARGET} ${PROCESS_TARGET})

# ADD LINKER FLAGS.
###################
target_link_libraries(${MASTER_TARGET} ${MESOS_TARGET} ${PROTOBUF_LFLAG})

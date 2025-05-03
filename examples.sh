#!/bin/zsh

case $1 in
  1)
    VERSION=$(java --version | head -n 1)

    if ! [[ $VERSION =~ (^openjdk 11*) ]]
    then
      echo "Not using java 11!"
      exit 1
    fi

    if [ -z $2 ]
    then
      echo "java -jar 1-npe/build/libs/1-npe-1.0-SNAPSHOT.jar"
      java -jar 1-npe/build/libs/1-npe-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/1-npe/libagent-1.so -jar 1-npe/build/libs/1-npe-1.0-SNAPSHOT.jar"
      java -agentpath:agent/1-npe/libagent-1.so -jar 1-npe/build/libs/1-npe-1.0-SNAPSHOT.jar
    fi
    ;;
  2)
    VERSION=$(java --version | head -n 1)

    if ! [[ $VERSION =~ (^openjdk 21*) ]]
    then
      echo "Still using java 11!"
      exit 1
    fi

    if [ -z $2 ]
    then
      echo "java -jar 2-simple/build/libs/2-simple-1.0-SNAPSHOT.jar"
      java -jar 2-simple/build/libs/2-simple-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/2-method-entry/libagent-2.so -jar 2-simple/build/libs/2-simple-1.0-SNAPSHOT.jar"
      java -agentpath:agent/2-method-entry/libagent-2.so -jar 2-simple/build/libs/2-simple-1.0-SNAPSHOT.jar
    fi
    ;;
  2-1)
    if [ -z $2 ]
    then
      echo "java -jar 2-simple-kotlin/build/libs/2-simple-kotlin-1.0-SNAPSHOT.jar"
      java -jar 2-simple-kotlin/build/libs/2-simple-kotlin-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/2-method-entry/libagent-2.so -jar 2-simple-kotlin/build/libs/2-simple-kotlin-1.0-SNAPSHOT.jar"
      java -agentpath:agent/2-method-entry/libagent-2.so -jar 2-simple-kotlin/build/libs/2-simple-kotlin-1.0-SNAPSHOT.jar
    fi
    ;;
  3)
    if [ -z $2 ]
    then
      echo "java -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar"
      java -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/3-stacktraces/libagent-3.so -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar"
      java -agentpath:agent/3-stacktraces/libagent-3.so -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar
    fi
    ;;
  4)
    echo "ulimit -u 7000"
    ulimit -u 7000
    if [ -z $2 ]
    then
      echo "java -jar 4-oom/build/libs/4-oom-1.0-SNAPSHOT.jar"
      java -jar 4-oom/build/libs/4-oom-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/4-jvmquake/libagent-4.so -jar 4-oom/build/libs/4-oom-1.0-SNAPSHOT.jar"
      java -agentpath:agent/4-jvmquake/libagent-4.so -jar 4-oom/build/libs/4-oom-1.0-SNAPSHOT.jar
    fi
    ulimit -u unlimited
    ;;
  5)
    if [ -z $2 ]
    then
      echo "java -jar 5-track-objects/build/libs/5-track-objects-1.0-SNAPSHOT.jar"
      java -jar 5-track-objects/build/libs/5-track-objects-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/5-track-objects/libagent-5.so -jar 5-track-objects/build/libs/5-track-objects-1.0-SNAPSHOT.jar"
      java -agentpath:agent/5-track-objects/libagent-5.so -jar 5-track-objects/build/libs/5-track-objects-1.0-SNAPSHOT.jar
    fi
    ;;
  7)
    cd 7-bypass-security
    if [ -z $2 ]
    then
      echo "java -jar build/libs/7-bypass-security-1.0-SNAPSHOT.jar"
      java -jar build/libs/7-bypass-security-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:../agent/7-bypass-security/libagent-7.so -jar build/libs/7-bypass-security-1.0-SNAPSHOT.jar"
      java -agentpath:../agent/7-bypass-security/libagent-7.so -jar build/libs/7-bypass-security-1.0-SNAPSHOT.jar
    fi
    cd ..
    ;;
  8)
    echo "java -agentpath:agent/8-watch-field/libagent-8.so -jar 8-quarkus-app/build/quarkus-app/quarkus-run.jar"
    java -agentpath:agent/8-watch-field/libagent-8.so -jar 8-quarkus-app/build/quarkus-app/quarkus-run.jar
    ;;
  9)
    echo "java -agentpath:agent/9-method-performance/libagent-9.so -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar"
    java -agentpath:agent/9-method-performance/libagent-9.so -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar
    ;;
  10)
    echo "java -agentpath:agent/10-debugging/libagent-10.so -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar"
    java -agentpath:agent/10-debugging/libagent-10.so -jar 3-stacktraces/build/libs/3-stacktraces-1.0-SNAPSHOT.jar
    ;;
  6)
    if [ -z $2 ]
    then
      echo "java -jar 6-track-threads/build/libs/6-track-threads-1.0-SNAPSHOT.jar"
      java -jar 6-track-threads/build/libs/6-track-threads-1.0-SNAPSHOT.jar
    else
      echo "java -agentpath:agent/6-track-threads/libagent-6.so -jar 6-track-threads/build/libs/6-track-threads-1.0-SNAPSHOT.jar"
      java -agentpath:agent/6-track-threads/libagent-6.so -jar 6-track-threads/build/libs/6-track-threads-1.0-SNAPSHOT.jar
    fi
    ;;
  *)
    echo "1 (with) 2 (with) 2-1 (with) 3 (with) 4 (with) 5 (with) 7 (with) 8 9 10"
    ;;
esac

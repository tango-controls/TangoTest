FROM debian:stretch

MAINTAINER info@tango-controls.org

RUN apt-get update && apt-get install -y apt-transport-https

RUN echo "deb https://dl.bintray.com/tango-controls/debian stretch main" | tee -a /etc/apt/sources.list

RUN apt-get update && apt-get install -y build-essential cmake

RUN apt-get update && apt-get install -y --allow-unauthenticated libtango9lts-dbg

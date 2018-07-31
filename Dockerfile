# docker pull i386/debian

FROM i386/debian:wheezy-slim

MAINTAINER Tango Controls <info@tango-controls.org>

ARG tango_host

# Install libtango7 & TangoTest
# TODO: Replace TANGO_HOST => argument to DockerFile
RUN apt-get update && echo "$tango_host" | apt-get install -y tango-test 

ENTRYPOINT ["/usr/lib/tango/TangoTest"]

# Start TangoTest
# docker run --network host wheezy-tangotest usr/lib/tango/TangoTest instance_name

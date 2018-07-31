# docker pull i386/debian

FROM i386/debian:wheezy-slim

MAINTAINER Tango Controls <info@tango-controls.org>

# Install libtango7 & TangoTest
RUN apt-get update && echo "$tango_host" | apt-get install -y tango-test 

ENTRYPOINT /usr/lib/tango/TangoTest test

# Start TangoTest
# docker run --network host -e TANGO_HOST=tango_host:10000 wheezy-tangotest

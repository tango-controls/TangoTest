FROM debian:stretch

MAINTAINER info@tango-controls.org

RUN apt-get update && apt-get install -y apt-transport-https supervisor

RUN echo "deb https://dl.bintray.com/tango-controls/debian stretch main" | tee -a /etc/apt/sources.list

RUN apt-get update && apt-get install -y --allow-unauthenticated libtango9lts-dbg

COPY TangoTest  /usr/bin/

COPY .docker/supervisord.conf      /etc/supervisord.conf

CMD /usr/bin/supervisord -c /etc/supervisord.conf


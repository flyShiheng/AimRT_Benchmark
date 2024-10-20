From mdsol/ubuntu22

ENV DEBIAN_FRONTEND=noninteractive

COPY ./set_up.sh /root/set_up.sh

RUN bash /root/set_up.sh
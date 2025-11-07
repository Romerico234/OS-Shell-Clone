FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y build-essential g++ make && \
    apt-get clean

WORKDIR /OS-Shell-Clone

COPY . .

RUN make all
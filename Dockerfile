FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y build-essential g++ make && \
    apt-get clean

WORKDIR /app

COPY . .

RUN make all

CMD ["/bin/bash"]

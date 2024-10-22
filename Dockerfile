FROM ubuntu:20.04

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN apt-get install -y git build-essential libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

RUN git clone https://github.com/raysan5/raylib.git
RUN cd raylib/src && make PLATFORM=PLATFORM_DESKTOP && cp raylib.h raymath.h /usr/include/

WORKDIR /app

RUN mkdir output/

COPY . /app

#CMD ["make" , "docker"]


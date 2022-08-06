FROM emscripten/emsdk:3.1.15

RUN pip install conan

RUN conan profile new default --detect

COPY default /root/.conan/profiles/default

WORKDIR /code

CMD ["/bin/sh", "entrypoint.sh"]

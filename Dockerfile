FROM conanio/gcc9:1.19.0

RUN pip install clang-format

USER root

WORKDIR absent

COPY . .

CMD ["/bin/bash"]

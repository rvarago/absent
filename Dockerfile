FROM conanio/gcc9:1.19.0

USER root

WORKDIR absent

COPY . .

CMD ["/bin/bash"]

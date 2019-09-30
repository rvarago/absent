FROM conanio/gcc8:1.19.0

USER root

WORKDIR absent

COPY . .

CMD ["/bin/bash"]

FROM conanio/gcc8:1.14.1

USER root

WORKDIR absent

COPY . .

CMD ["make", "test"]

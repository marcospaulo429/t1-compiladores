# Ambiente Linux para compilar e testar o trabalho com as mesmas versoes
# do enunciado (gcc 14, Flex 2.6.4 e Bison 3.8.2), mais o SPIM para rodar
# o assembly gerado.
#
#   docker build -t g-v1 .
#   docker run --rm g-v1              # compila e roda os testes
#   docker run --rm -it g-v1 bash     # abre um terminal dentro do container

FROM gcc:14

RUN apt-get update \
    && apt-get install -y --no-install-recommends flex bison spim \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /trabalho
COPY trabalho-final ./trabalho-final
COPY testes ./testes

WORKDIR /trabalho/trabalho-final
CMD make && make test

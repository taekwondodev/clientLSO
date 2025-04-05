# Usa una base image che include GCC
FROM gcc:latest

# Installa le dipendenze necessarie, inclusa OpenSSL
RUN apt-get update && apt-get install -y \
    libssl-dev \
    libcjson-dev \
    make \
    tree \
    && rm -rf /var/lib/apt/lists/*

# Imposta la directory di lavoro all'interno del container
WORKDIR /app

# Copia il codice sorgente nel container
COPY . /app

# Compila il programma
RUN make

CMD ["tail", "-f", "/dev/null"]
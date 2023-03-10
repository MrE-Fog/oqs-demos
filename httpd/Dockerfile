# Multi-stage build: First the full builder image:

# First: global build arguments: 

# liboqs build type variant; maximum portability of image:
ARG LIBOQS_BUILD_DEFINES="-DOQS_DIST_BUILD=ON"

# installation paths
ARG OPENSSL_PATH=/opt/openssl
ARG HTTPD_PATH=/opt/httpd

# defines the QSC signature algorithm used for the certificates:
ARG SIG_ALG="dilithium3"

# define the httpd version to include
ARG HTTPD_VERSION=2.4.55

# define the APR version to include
ARG APR_VERSION=1.7.2

# define the APR util version to include
ARG APRU_VERSION=1.6.3

# Define the degree of parallelism when building the image; leave the number away only if you know what you are doing
ARG MAKE_DEFINES="-j 2"


FROM alpine:3.13 as intermediate
# ToDo: Upgrade possible if https://wiki.alpinelinux.org/wiki/Release_Notes_for_Alpine_3.14.0#faccessat2 addressed

# Take in global args
ARG LIBOQS_BUILD_DEFINES
ARG OPENSSL_PATH
ARG HTTPD_PATH
ARG SIG_ALG
ARG HTTPD_VERSION
ARG APR_VERSION
ARG APRU_VERSION
ARG MAKE_DEFINES


# Get all software packages required for builing all components:
RUN apk add build-base linux-headers \
            libtool automake autoconf cmake ninja \
            make \
            openssl openssl-dev \
            git wget pcre-dev \
            expat-dev

# get sources
WORKDIR /opt
RUN git clone --depth 1 --branch main https://github.com/open-quantum-safe/liboqs && \
    git clone --depth 1 --branch OQS-OpenSSL_1_1_1-stable https://github.com/open-quantum-safe/openssl ossl-src && \
    wget http://mirror.easyname.ch/apache/apr/apr-${APR_VERSION}.tar.gz && tar xzvf apr-${APR_VERSION}.tar.gz && \
    wget http://mirror.easyname.ch/apache/apr/apr-util-${APRU_VERSION}.tar.gz && tar xzvf apr-util-${APRU_VERSION}.tar.gz && \
    wget --trust-server-names "https://archive.apache.org/dist/httpd/httpd-${HTTPD_VERSION}.tar.gz" && tar -zxvf httpd-${HTTPD_VERSION}.tar.gz;

# build liboqs (static linking only)
WORKDIR /opt/liboqs
RUN mkdir build-static && cd build-static && cmake -G"Ninja" .. ${LIBOQS_BUILD_DEFINES} -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/opt/ossl-src/oqs && ninja && ninja install

# build OQS-OpenSSL
WORKDIR /opt/ossl-src
RUN ./config no-shared --prefix=${OPENSSL_PATH} && \
    make ${MAKE_DEFINES} && make install_sw;

# build httpd
WORKDIR /opt
RUN sed -i "s/\$RM \"\$cfgfile\"/\$RM -f \"\$cfgfile\"/g" apr-${APR_VERSION}/configure && \
    cd apr-${APR_VERSION} && ./configure && make ${MAKE_DEFINES} && make install && cd .. && \
    cd apr-util-${APRU_VERSION} && ./configure x86_64-pc-linux-gnu --with-crypto --with-apr=/usr/local/apr && make ${MAKE_DEFINES} && make install

WORKDIR /opt/httpd-${HTTPD_VERSION}
RUN ./configure --prefix=${HTTPD_PATH} \
                    --enable-debugger-mode \
                    --enable-ssl --with-ssl=${OPENSSL_PATH} \
                    --enable-ssl-staticlib-deps \
                    --enable-mods-static=ssl && \
    sed -i "s/-lpthread/-lpthread -L\/opt\/openssl\/lib -loqs/g" build/config_vars.mk && \
    make ${MAKE_DEFINES} && make install;

# prepare to run httpd
ARG OPENSSL_CNF=/opt/ossl-src/apps/openssl.cnf

WORKDIR ${HTTPD_PATH}
    # generate CA key and cert
    # generate server CSR
    # generate server cert
RUN set -x && \
    mkdir pki && \
    mkdir cacert && \
    ${OPENSSL_PATH}/bin/openssl req -x509 -new -newkey ${SIG_ALG} -keyout cacert/CA.key -out cacert/CA.crt -nodes -subj "/CN=oqstest CA" -days 365 -config ${OPENSSL_CNF} && \
    ${OPENSSL_PATH}/bin/openssl req -new -newkey ${SIG_ALG} -keyout pki/server.key -out pki/server.csr -nodes -subj "/CN=oqs-httpd" -config ${OPENSSL_CNF} && \
    ${OPENSSL_PATH}/bin/openssl x509 -req -in pki/server.csr -out pki/server.crt -CA cacert/CA.crt -CAkey cacert/CA.key -CAcreateserial -days 365

# Some size optimization:
RUN rm -rf ${HTTPD_PATH}/bin/ab

# second stage: Only create minimal image without build tooling and intermediate build results generated above:
FROM alpine:3.13
# Take in global args
ARG HTTPD_PATH
ARG OPENSSL_PATH
#
RUN apk add pcre-dev expat-dev
#
# Only retain the ${*_PATH} contents in the final image
COPY --from=intermediate ${HTTPD_PATH} ${HTTPD_PATH}
# copy over manually build libapr{util}
COPY --from=intermediate /usr/local/apr/lib  /usr/local/apr/lib

#Enable this if you want to have openssl also in this image
#COPY --from=intermediate ${OPENSSL_PATH}/apps/openssl ${OPENSSL_PATH}

COPY httpd-conf/httpd-ssl.conf ${HTTPD_PATH}/httpd-conf/httpd-ssl.conf
COPY httpd-conf/httpd.conf ${HTTPD_PATH}/httpd-conf/httpd.conf
WORKDIR ${HTTPD_PATH}

# forward request and error logs to docker log collector
RUN ln -sf /dev/stdout ${HTTPD_PATH}/logs/access_log && \
    ln -sf /dev/stderr ${HTTPD_PATH}/logs/error_log;
#
RUN addgroup -g 1000 -S oqs && adduser --uid 1000 -S oqs -G oqs && chown -R oqs.oqs ${HTTPD_PATH}
USER oqs

# Ensure httpd just runs
ENV PATH ${HTTPD_PATH}/bin:$PATH

EXPOSE 4433
#
STOPSIGNAL SIGTERM

CMD ["httpd", "-f", "httpd-conf/httpd.conf", "-D", "FOREGROUND"]


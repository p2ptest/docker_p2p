FROM centos:6.8

COPY public /opt/public
COPY server /opt/server

RUN buildDeps='gcc-c++ openssl-devel mysql-devel libuuid-devel' \
	&& yum install -y $buildDeps \
	$$ yum clean all \
	&& cd /usr/lib64/mysql \
	&& ln -s libmysqlclient_r.so libmysqlclient_r.a

RUN cd /opt/server/framecommon && make \
	&& cd /opt/server/srvframe && sh build.sh \
	&& cd /opt/server/utils && make

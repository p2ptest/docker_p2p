FROM centos:6.8

RUN buildDeps='gcc-c++ openssl-devel mysql-devel libuuid-devel' \
	&& yum install -y $buildDeps \
	$$ yum clean all \
	&& cd /usr/lib64/mysql \
	&& ln -s libmysqlclient_r.so libmysqlclient_r.a

COPY public /opt/public
COPY server /opt/server

RUN cd /opt/server/framecommon && make \
	&& cd /opt/server/utils && make \
	&& cd /opt/server/srvframe && chmod +x build.sh
RUN cd /opt/server/srvframe && ./build.sh

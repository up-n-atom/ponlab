FROM debian:buster-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN \
    apt-get update && \
    apt-get install --yes --no-install-recommends \
	build-essential \
	curl \
	file \
	gawk \
	git-core \
	gosu \
	iproute2 \
	iputils-ping \
	libdw-dev \
	libelf-dev \
	libncurses5-dev \
	locales \
	nmap \
	openssh-client \
	pwgen \
	python-distlib \
	python-yaml \
	python3 \
	python3-pip \
	python3-setuptools \
	rsync \
	signify-openbsd \
	subversion \
	sudo \
	unzip \
	wget && \
    apt-get -y autoremove && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG=en_US.utf8

RUN pip3 install -U pip
RUN pip3 install \
	jira \
	pyyaml \
	pyjwt

RUN \
    echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers && \
    useradd -c "OpenWrt Builder" -m -d /home/builder -G sudo -s /bin/bash builder

USER builder
ENV HOME /home/builder
WORKDIR /home/builder/

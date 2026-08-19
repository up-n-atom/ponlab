ARG DESIGNATED_BRANCH=prplos

# FIXME: its overkill to use this container
FROM registry.gitlab.com/prpl-foundation/prplos/prplos/$DESIGNATED_BRANCH/builder:latest

COPY --chown=builder:builder . /home/builder/

WORKDIR /home/builder
RUN mkdir .ssh && ssh-keyscan gitlab.com > .ssh/known_hosts

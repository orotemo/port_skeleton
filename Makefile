PROJECT = project1
PROJECT_DESCRIPTION = Project 1 description
PROJECT_VERSION = 0.0.1
PROJECT_REGISTERED = project1

LOCAL_DEPS = sasl
DEPS = cowboy jiffy ej lager
dep_cowboy_commit = master

C_SRC_TYPE = executable

include ../../erlang.mk

ERLC_OPTS += +'{parse_transform, lager_transform}'

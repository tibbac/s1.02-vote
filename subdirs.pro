TEMPLATE = subdirs
SUBDIRS = \
          libvote \
          vote \
          vote_alternatif \
          vote_majoritaire \
          vote_preferentiel

vote.depends = libvote

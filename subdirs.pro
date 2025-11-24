TEMPLATE = subdirs
SUBDIRS = \
          vote \
          libvote

vote.depends = libvote

# mapson/cfg.mk

gnu_rel_host    := peti,mapson@frs.sourceforge.net
upload_dest_dir_:= /home/frs/project/m/ma/mapson
gpg_key_ID      := 99089D72
today           := $(date "+%Y-%m-%d")
TAR_OPTIONS     += --mtime=$(today)
old_NEWS_hash   := 2a20aa417e6a09dfbed7f08478ac2714
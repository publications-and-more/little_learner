# File: Makefile
# Author: Amlal El Mahrouss
# Purpose: Generate HTML and PDF papers from LaTex documents.
# (C) 2025-2026 Amlal El Mahrouss.
# Licensed under Apache 2.0.

.PHONY: all
all:
	@echo "Please specify a target: on_al, ..."

.PHONY: ack
ack:
	@latex notes/intro/content/p.tex

.PHONY: on_al
on_al:
	@latex notes/on_al/content/p.tex

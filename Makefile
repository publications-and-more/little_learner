# File: Makefile
# Author: Amlal El Mahrouss
# Purpose: Generate HTML and PDF papers from LaTex documents.
# (C) 2025-2026 Amlal El Mahrouss.
# Licensed under Apache 2.0.

ECHO=@echo
LATEX=@latex

.PHONY: all
all:
	${ECHO} "Please specify a target: on_al, ..."

.PHONY: intro_paper
intro_paper:
	${LATEX} research/notes/intro/content/p.tex

.PHONY: on_al_notes
on_al_notes:
	${LATEX} research/notes/on_al/content/p.tex

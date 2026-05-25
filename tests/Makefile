MAKEFLAGS += --no-print-directory

VENV=venv
REQUIREMENTS=requirements.txt
PIP=venv/bin/pip
PYTHON=venv/bin/python

RUN=run.py

$(VENV): $(REQUIREMENTS)
	python -m venv venv
	$(PIP) install --upgrade pip && $(PIP) install -r $(REQUIREMENTS)

run: $(VENV)
	$(PYTHON) $(RUN)

dev:
	c++ -g3 -I ../includes ./src/tester.cpp ./dev/main.cpp -L.. -lft_malloc -Wl,-rpath,.. && ./a.out && $(RM) a.out

.PHONY: run dev

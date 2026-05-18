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

.PHONY: all

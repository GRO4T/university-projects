FROM python:3.7-alpine

COPY requirements.txt /app/requirements.txt
RUN pip3 install -r /app/requirements.txt
COPY ./app /app
COPY ./test_triggers.py /test_triggers.py
COPY ./run.sh /run.sh

ENTRYPOINT ["/run.sh"]

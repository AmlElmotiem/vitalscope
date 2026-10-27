FROM python:3.12-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir --timeout=100 --retries=5 -r requirements.txt

COPY app ./app

# Writable location for the SQLite file when a volume is mounted here.
RUN mkdir -p /app/data

EXPOSE 8000
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000"]

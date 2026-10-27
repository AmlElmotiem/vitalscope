#!/usr/bin/env python3
"""Populate a running VitalScope api-service with realistic demo data.

Creates three patients that each tell a different clinical story:
  - a healthy patient with stable, normal vitals
  - a deteriorating patient whose vitals worsen over several measurements,
    so the trend detection (rising/falling) has something to show
  - a critical patient with a single measurement well outside normal ranges,
    to exercise the critical-severity anomalies and a high early warning score

Usage:
    python scripts/generate_test_data.py [base_url]

`base_url` defaults to http://localhost:8000 (the api-service, whether run
natively or via `docker compose up`).
"""

import json
import sys
import urllib.error
import urllib.request
from datetime import datetime, timedelta, timezone


def post_json(url: str, payload: dict) -> dict:
    data = json.dumps(payload).encode("utf-8")
    request = urllib.request.Request(
        url, data=data, headers={"Content-Type": "application/json"}, method="POST"
    )
    with urllib.request.urlopen(request) as response:
        return json.loads(response.read())


def get_json(url: str) -> dict:
    with urllib.request.urlopen(url) as response:
        return json.loads(response.read())


def create_patient(base_url: str, first_name: str, last_name: str, birth_date: str, gender: str) -> int:
    patient = post_json(
        f"{base_url}/patients",
        {
            "first_name": first_name,
            "last_name": last_name,
            "birth_date": birth_date,
            "gender": gender,
        },
    )
    print(f"Created patient {patient['id']}: {first_name} {last_name}")
    return patient["id"]


def add_measurement(base_url: str, patient_id: int, timestamp: datetime, vitals: dict) -> None:
    payload = {"timestamp": timestamp.isoformat(), **vitals}
    post_json(f"{base_url}/patients/{patient_id}/measurements", payload)


def show_analysis(base_url: str, patient_id: int) -> None:
    analysis = get_json(f"{base_url}/patients/{patient_id}/analysis")
    print(f"  early_warning_score: {analysis['early_warning_score']}")
    print(f"  anomalies: {[a['code'] for a in analysis['anomalies']] or 'none'}")
    print(f"  trends: {analysis['trends']}")


def main() -> None:
    base_url = sys.argv[1] if len(sys.argv) > 1 else "http://localhost:8000"
    now = datetime.now(timezone.utc)

    try:
        get_json(f"{base_url}/health")
    except (urllib.error.URLError, ConnectionError) as error:
        print(f"Could not reach {base_url}/health ({error}). Is the api-service running?")
        sys.exit(1)

    # Healthy patient: one normal measurement.
    healthy_id = create_patient(base_url, "Lena", "Hoffmann", "1988-03-14", "female")
    add_measurement(
        base_url,
        healthy_id,
        now,
        {
            "heart_rate": 72,
            "spo2": 98,
            "systolic_bp": 118,
            "diastolic_bp": 76,
            "temperature": 36.8,
            "respiratory_rate": 14,
        },
    )
    print("Analysis for healthy patient:")
    show_analysis(base_url, healthy_id)

    # Deteriorating patient: five measurements over five hours, vitals worsening.
    deteriorating_id = create_patient(base_url, "Markus", "Weber", "1965-11-02", "male")
    heart_rates = [78, 90, 102, 115, 128]
    spo2_values = [97, 95, 93, 91, 89]
    for i, (hr, spo2) in enumerate(zip(heart_rates, spo2_values)):
        add_measurement(
            base_url,
            deteriorating_id,
            now - timedelta(hours=4 - i),
            {
                "heart_rate": hr,
                "spo2": spo2,
                "systolic_bp": 125 + i * 4,
                "diastolic_bp": 80,
                "temperature": 37.0 + i * 0.2,
                "respiratory_rate": 16 + i,
            },
        )
    print("Analysis for deteriorating patient:")
    show_analysis(base_url, deteriorating_id)

    # Critical patient: a single severely abnormal measurement.
    critical_id = create_patient(base_url, "Sophie", "Krause", "1952-07-29", "female")
    add_measurement(
        base_url,
        critical_id,
        now,
        {
            "heart_rate": 145,
            "spo2": 86,
            "systolic_bp": 82,
            "diastolic_bp": 50,
            "temperature": 39.4,
            "respiratory_rate": 30,
        },
    )
    print("Analysis for critical patient:")
    show_analysis(base_url, critical_id)


if __name__ == "__main__":
    main()

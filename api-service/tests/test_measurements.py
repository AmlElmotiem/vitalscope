"""Tests for measurement endpoints, input validation, and the (mocked)
analysis-engine call — the real C++ service is never started for these tests.
"""

from fastapi.testclient import TestClient
from pytest_mock import MockerFixture

_VALID_MEASUREMENT = {
    "heart_rate": 75,
    "spo2": 98,
    "systolic_bp": 120,
    "diastolic_bp": 80,
    "temperature": 37.0,
    "respiratory_rate": 16,
}


def _create_patient(client: TestClient) -> int:
    response = client.post(
        "/patients",
        json={"first_name": "A", "last_name": "B", "birth_date": "1990-01-01", "gender": "male"},
    )
    return response.json()["id"]


def test_add_measurement(client: TestClient) -> None:
    patient_id = _create_patient(client)
    response = client.post(f"/patients/{patient_id}/measurements", json=_VALID_MEASUREMENT)
    assert response.status_code == 200
    assert response.json()["patient_id"] == patient_id


def test_add_measurement_rejects_out_of_range_heart_rate(client: TestClient) -> None:
    patient_id = _create_patient(client)
    invalid = {**_VALID_MEASUREMENT, "heart_rate": 500}  # outside the 20-300 range
    response = client.post(f"/patients/{patient_id}/measurements", json=invalid)
    assert response.status_code == 422


def test_add_measurement_to_missing_patient_returns_404(client: TestClient) -> None:
    response = client.post("/patients/999/measurements", json=_VALID_MEASUREMENT)
    assert response.status_code == 404


def test_list_measurements(client: TestClient) -> None:
    patient_id = _create_patient(client)
    client.post(f"/patients/{patient_id}/measurements", json=_VALID_MEASUREMENT)
    response = client.get(f"/patients/{patient_id}/measurements")
    assert response.status_code == 200
    assert len(response.json()) == 1


def test_analysis_calls_mocked_engine(client: TestClient, mocker: MockerFixture) -> None:
    patient_id = _create_patient(client)
    client.post(f"/patients/{patient_id}/measurements", json=_VALID_MEASUREMENT)

    fake_response = mocker.Mock()
    fake_response.json.return_value = {"early_warning_score": 0, "anomalies": []}
    fake_response.raise_for_status.return_value = None
    mocker.patch("app.routers.patients.httpx.post", return_value=fake_response)

    response = client.get(f"/patients/{patient_id}/analysis")
    assert response.status_code == 200
    assert response.json() == {"early_warning_score": 0, "anomalies": []}


def test_analysis_without_measurements_returns_400(client: TestClient) -> None:
    patient_id = _create_patient(client)
    response = client.get(f"/patients/{patient_id}/analysis")
    assert response.status_code == 400

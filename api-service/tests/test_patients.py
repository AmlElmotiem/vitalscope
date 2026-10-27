"""Tests for patient CRUD endpoints."""

from fastapi.testclient import TestClient


def test_create_and_get_patient(client: TestClient) -> None:
    response = client.post(
        "/patients",
        json={
            "first_name": "Ada",
            "last_name": "Lovelace",
            "birth_date": "1990-01-01",
            "gender": "female",
        },
    )
    assert response.status_code == 200
    patient_id = response.json()["id"]

    response = client.get(f"/patients/{patient_id}")
    assert response.status_code == 200
    assert response.json()["first_name"] == "Ada"


def test_list_patients(client: TestClient) -> None:
    client.post(
        "/patients",
        json={"first_name": "A", "last_name": "B", "birth_date": "1990-01-01", "gender": "male"},
    )
    response = client.get("/patients")
    assert response.status_code == 200
    assert len(response.json()) == 1


def test_get_missing_patient_returns_404(client: TestClient) -> None:
    response = client.get("/patients/999")
    assert response.status_code == 404


def test_delete_patient(client: TestClient) -> None:
    response = client.post(
        "/patients",
        json={"first_name": "A", "last_name": "B", "birth_date": "1990-01-01", "gender": "male"},
    )
    patient_id = response.json()["id"]

    response = client.delete(f"/patients/{patient_id}")
    assert response.status_code == 204

    response = client.get(f"/patients/{patient_id}")
    assert response.status_code == 404

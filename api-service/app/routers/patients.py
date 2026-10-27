"""Endpoints for patients, their measurements, and the C++ analysis call."""

import httpx
from fastapi import APIRouter, Depends, HTTPException
from sqlmodel import Session, select

from app.config import settings
from app.database import get_session
from app.models.measurement import Measurement, MeasurementCreate, MeasurementRead
from app.models.patient import Patient, PatientCreate, PatientRead

router = APIRouter()


def _get_patient_or_404(patient_id: int, session: Session) -> Patient:
    patient = session.get(Patient, patient_id)
    if patient is None:
        raise HTTPException(status_code=404, detail="Patient not found")
    return patient


@router.post("/patients", response_model=PatientRead)
def create_patient(patient: PatientCreate, session: Session = Depends(get_session)) -> Patient:
    db_patient = Patient.model_validate(patient)
    session.add(db_patient)
    session.commit()
    session.refresh(db_patient)
    return db_patient


@router.get("/patients", response_model=list[PatientRead])
def list_patients(session: Session = Depends(get_session)) -> list[Patient]:
    return list(session.exec(select(Patient)).all())


@router.get("/patients/{patient_id}", response_model=PatientRead)
def get_patient(patient_id: int, session: Session = Depends(get_session)) -> Patient:
    return _get_patient_or_404(patient_id, session)


@router.delete("/patients/{patient_id}", status_code=204)
def delete_patient(patient_id: int, session: Session = Depends(get_session)) -> None:
    patient = _get_patient_or_404(patient_id, session)
    session.delete(patient)
    session.commit()


@router.post("/patients/{patient_id}/measurements", response_model=MeasurementRead)
def add_measurement(
    patient_id: int,
    measurement: MeasurementCreate,
    session: Session = Depends(get_session),
) -> Measurement:
    _get_patient_or_404(patient_id, session)
    db_measurement = Measurement.model_validate(measurement, update={"patient_id": patient_id})
    session.add(db_measurement)
    session.commit()
    session.refresh(db_measurement)
    return db_measurement


@router.get("/patients/{patient_id}/measurements", response_model=list[MeasurementRead])
def list_measurements(
    patient_id: int, session: Session = Depends(get_session)
) -> list[Measurement]:
    _get_patient_or_404(patient_id, session)
    statement = select(Measurement).where(Measurement.patient_id == patient_id)
    return list(session.exec(statement).all())


@router.get("/patients/{patient_id}/analysis")
def get_analysis(patient_id: int, session: Session = Depends(get_session)) -> dict:
    _get_patient_or_404(patient_id, session)

    statement = select(Measurement).where(Measurement.patient_id == patient_id)
    measurements = list(session.exec(statement).all())
    if not measurements:
        raise HTTPException(status_code=400, detail="Patient has no measurements to analyze")

    payload = {
        "measurements": [
            {
                "timestamp": int(m.timestamp.timestamp()),
                "heart_rate": m.heart_rate,
                "spo2": m.spo2,
                "systolic_bp": m.systolic_bp,
                "diastolic_bp": m.diastolic_bp,
                "temperature": m.temperature,
                "respiratory_rate": m.respiratory_rate,
            }
            for m in measurements
        ]
    }

    try:
        response = httpx.post(
            f"{settings.analysis_engine_url}/analyze", json=payload, timeout=5.0
        )
        response.raise_for_status()
    except httpx.HTTPError as exc:
        raise HTTPException(
            status_code=502, detail=f"Analysis engine request failed: {exc}"
        ) from exc

    return response.json()

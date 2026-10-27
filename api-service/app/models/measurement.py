"""Measurement database model and request/response schemas.

Valid ranges are enforced here with Pydantic's Field(ge=..., le=...) so that
an out-of-range value is rejected with a 422 response before it ever reaches
the database or the analysis engine.
"""

from datetime import datetime, timezone
from typing import Optional

from sqlmodel import Field, SQLModel


class MeasurementBase(SQLModel):
    timestamp: datetime = Field(default_factory=lambda: datetime.now(timezone.utc))
    heart_rate: float = Field(ge=20, le=300, description="Beats per minute")
    spo2: float = Field(ge=50, le=100, description="Oxygen saturation, percent")
    systolic_bp: float = Field(ge=40, le=300, description="mmHg")
    diastolic_bp: float = Field(ge=20, le=200, description="mmHg")
    temperature: float = Field(ge=25, le=45, description="Degrees Celsius")
    respiratory_rate: float = Field(ge=4, le=60, description="Breaths per minute")


class Measurement(MeasurementBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    patient_id: int = Field(foreign_key="patient.id")


class MeasurementCreate(MeasurementBase):
    pass


class MeasurementRead(MeasurementBase):
    id: int
    patient_id: int

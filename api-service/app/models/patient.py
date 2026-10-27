"""Patient database model and request/response schemas."""

from datetime import date, datetime, timezone
from typing import Optional

from sqlmodel import Field, SQLModel


class PatientBase(SQLModel):
    first_name: str
    last_name: str
    birth_date: date
    gender: str


class Patient(PatientBase, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    created_at: datetime = Field(default_factory=lambda: datetime.now(timezone.utc))


class PatientCreate(PatientBase):
    pass


class PatientRead(PatientBase):
    id: int
    created_at: datetime

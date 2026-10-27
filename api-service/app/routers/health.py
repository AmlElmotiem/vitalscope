"""Health check, reporting the status of this service and the analysis engine."""

import httpx
from fastapi import APIRouter

from app.config import settings

router = APIRouter()


@router.get("/health")
def health() -> dict:
    analysis_engine_ok = False
    try:
        response = httpx.get(f"{settings.analysis_engine_url}/health", timeout=2.0)
        analysis_engine_ok = response.status_code == 200
    except httpx.HTTPError:
        analysis_engine_ok = False

    return {
        "api_service": "ok",
        "analysis_engine": "ok" if analysis_engine_ok else "unreachable",
    }

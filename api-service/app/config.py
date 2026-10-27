"""Application configuration, loaded from environment variables."""

import os


class Settings:
    database_url: str = os.environ.get("DATABASE_URL", "sqlite:///./vitalscope.db")
    analysis_engine_url: str = os.environ.get("ANALYSIS_ENGINE_URL", "http://localhost:8081")


settings = Settings()

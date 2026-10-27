# --- Build stage: compiler, CMake and all FetchContent dependencies live only here ---
FROM gcc:13 AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake ninja-build git ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DVITALSCOPE_BUILD_TESTS=OFF \
    && cmake --build build --target vitalscope_server

# --- Runtime stage: only the compiled binary and its shared-library dependencies ---
FROM debian:bookworm-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /src/build/server/vitalscope_server /usr/local/bin/vitalscope_server

EXPOSE 8081
CMD ["vitalscope_server"]

# Build & Run Instructions

## Requirements
- Docker Desktop 

## Build the Docker Image

From the project root, run:

```bash
docker compose build
```

## Run the Container
```bash
docker compose up -d
```

## Open up a Shell in the Container
```bash
docker exec -it custom-shell-container bash

# Can check Ubuntu image
cat /etc/os-release
```

## Run Custom Shell
```bash
./bin/custom-shell
```

## Rebuild & Rerun Custom Shell Inside Container
```bash
make clean && make
./bin/custom-shell
```

## Exit CLI and Stop Container
```bash
exit 
docker compose down
```
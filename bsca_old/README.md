# Binary SCA (Tradition method)

Recover library and function name by using unique fingerprint.

## Usage

```bash
# 1. Extract fingerprint from known binary file
$ uv run recover.py extract binary/curl --info 'curl/8.14.1'
# 2. Detect fingerprint from unknown binary file
$ uv run recover.py detect /usr/bin/curl
```

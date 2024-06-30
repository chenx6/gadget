from logging import DEBUG, getLogger, StreamHandler

_logger = None


def get_logger():
    global _logger
    if not _logger:
        _logger = getLogger("pydec")
        _logger.setLevel(DEBUG)
        _logger.addHandler(StreamHandler())
    return _logger

import logging


def configure_logging(filename: str = None):
    if not filename:
        filename = "default.log"
    logging.basicConfig(filename=filename,
                        format='%(asctime)s %(levelname)s %(message)s')

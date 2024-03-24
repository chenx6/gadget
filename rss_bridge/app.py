"""
load config => start timer => process subscribes
"""
from re import search
from tomllib import load as load_toml
from sqlite3 import connect
from typing import Any, NamedTuple
from datetime import datetime
from logging import getLogger, basicConfig, DEBUG
from time import sleep
from base64 import b64encode
from pathlib import Path
from traceback import format_exc
from functools import wraps
from datetime import datetime

from feedparser import parse as parse_feed
from schedule import every, run_pending, CancelJob
from requests import post, get


class Subscribe(NamedTuple):
    name: str
    time: datetime
    url: str
    on_air_range: int
    exclude: str | None = None
    include: str | None = None


class Shortcut(NamedTuple):
    name: str
    alias: str


class Config(NamedTuple):
    aria2_url: str
    shortcuts: list[Any]
    subscribes: list[Subscribe]
    exclude: str | None = None
    include: str | None = None


basicConfig(level=DEBUG)
logger = getLogger("bridge")
db = connect("./record.db")


def catch_exceptions(cancel_on_failure=False):
    def catch_exceptions_decorator(job_func):
        @wraps(job_func)
        def wrapper(*args, **kwargs):
            try:
                return job_func(*args, **kwargs)
            except:
                import traceback

                print(traceback.format_exc())
                if cancel_on_failure:
                    return CancelJob

        return wrapper

    return catch_exceptions_decorator


def init_db():
    cursor = db.cursor()
    cursor.execute(
        """CREATE TABLE IF NOT EXISTS download_record (
    id INTEGER PRIMARY KEY,
    torrent_name VARCHAR,
    torrent_url VARCHAR
    );"""
    )
    cursor.execute(
        "CREATE INDEX IF NOT EXISTS turl_idx ON download_record(torrent_name);"
    )
    db.commit()


def set_download(name: str, url: str):
    cursor = db.cursor()
    cursor.execute(
        "INSERT INTO download_record (torrent_name, torrent_url) VALUES (?, ?)",
        (
            name,
            url,
        ),
    )
    db.commit()


def is_downloaded(url: str) -> bool:
    cursor = db.cursor()
    res = cursor.execute("SELECT id FROM download_record WHERE torrent_url = ?", (url,))
    return res.fetchall() != []


def sendto_aria2(url: str, name: str):
    resp = get(url)
    encoded = b64encode(resp.content)
    download_dir = Path(conf["dir"]) / name
    if not download_dir.exists():
        download_dir.mkdir()
    params = [encoded.decode(), [], {"dir": str(download_dir)}]
    if token := conf.get("token", None):
        params.insert(0, f"token:{token}")
        logger.debug(f"Token: {token}")
    payload = {
        "jsonrpc": "2.0",
        "id": 1,
        "method": "aria2.addTorrent",
        "params": params,
    }
    resp = post(conf["aria2_url"], json=payload)
    logger.debug(f"Aria2 response code {resp.status_code}")
    if resp.status_code != 200:
        logger.debug(f"Aria2 response content: {resp.content}")
    return resp.status_code


def load_config() -> tuple[dict[str, Any], list[Subscribe]]:
    """Load config from config.toml file"""
    subs = []
    with open("config.toml", "rb") as f:
        conf = load_toml(f)
        # Load subscribers
        for sub in conf["subscribes"]:
            url: str = sub["url"]
            # Replace shortcut in url
            for shortcut in conf["shortcuts"]:
                if url.startswith(shortcut["name"]):
                    url = url.replace(f'{shortcut["name"]}://', shortcut["alias"], 1)
            # Load on air range
            if oar := sub.get("on_air_range"):
                on_air_range = int(oar)
            else:
                on_air_range = 90
            subs.append(
                Subscribe(
                    sub["name"],
                    sub["time"],
                    url,
                    on_air_range,
                    sub.get("exclude"),
                    sub.get("include"),
                )
            )
            logger.debug(f"Sub name {sub['name']}, {url}")
        logger.info(f"Loading {len(subs)} config")
        return conf, subs


@catch_exceptions(True)
def update_rss(subs: list[Subscribe], excl: str | None = None, incl: str | None = None):
    """Timer triggered rss updater"""
    curr_time = datetime.now()
    logger.info(f"Updating in {curr_time}...")
    for sub in subs:
        # If current time doesn't in on air time, skip it
        on_air_days = curr_time - sub.time
        if on_air_days.days > sub.on_air_range:
            logger.debug("Time expired, skip")
            continue
        feed = parse_feed(sub.url)
        for entry in feed.entries:
            ent_id = entry.id
            # Use subscribe specify exculde rule first
            curr_excl = excl
            if sub.exclude:
                curr_excl = sub.exclude
            # Check entry need should be excluded or not
            if curr_excl and (ns := search(curr_excl, ent_id)):
                continue
            if not incl or (s := search(incl, ent_id)):
                # Find torrent url
                for link in entry.links:
                    if link.type == "application/x-bittorrent" and not is_downloaded(
                        link.href
                    ):
                        logger.info(f"Downloading new {ent_id} from {link.href}")
                        if sendto_aria2(link.href, sub.name) == 200:
                            set_download(ent_id, link.href)
    logger.info("Update done")


test = False
init_db()
conf, subs = load_config()
if test:
    update_rss(subs, conf["exclude"])
    exit(0)
job = lambda: update_rss(subs, conf["exclude"])
every(1).hours.do(job)  # Execute update every 1 hour
job()  # There will be a long pending time
while True:
    run_pending()
    sleep(60)
